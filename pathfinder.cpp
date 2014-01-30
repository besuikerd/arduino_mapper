#include "pathfinder.h"
#include "map.h"
#include "config.h"

Pathfinder::Pathfinder(Map* m){
    this->m = m;
}

void Pathfinder::run(int currentX, int currentY, int currentHeading){
    int x,y,current,n,e,w,s;
    bool changed = true;
    
    // Reset target
    this->currentX = currentX;
    this->currentY = currentY;
    this->targetX = currentX;
    this->targetY = currentY;
    // Reset all costs and headings
    for(x=0; x<this->m->getMapSize(); x++){
        for(y=0; y<this->m->getMapSize(); y++){
            targetCosts[x][y].cost = DEFAULT_COST;
            targetCosts[x][y].heading = NORTH;
        }
    }
    
    // Current position
    targetCosts[currentX][currentY].cost = 0;
    targetCosts[currentX][currentY].heading = currentHeading;
    
    // Calculate path lengths
    while(changed){
        changed = false;
        for(x=0; x<MAP_SIZE; x++){
            for(y=0; y<MAP_SIZE; y++){
                if(!m->isObstacle(x,y)){
                    current = targetCosts[x][y].cost;
                    // Calculate distance by adding distance to neighbour to 1 and the number of turns needed to good
                    // from the neighbour to this point
                    n = this->getCostTo(x, y-1)+1+this->calculateTurningCost(this->getTargetHeading(x, y-1), SOUTH);
                    e = this->getCostTo(x+1, y)+1+this->calculateTurningCost(this->getTargetHeading(x+1, y), WEST);
                    w = this->getCostTo(x-1, y)+1+this->calculateTurningCost(this->getTargetHeading(x-1, y), EAST);
                    s = this->getCostTo(x, y+1)+1+this->calculateTurningCost(this->getTargetHeading(x, y+1), NORTH);
                    
                    // Determine shortest route
                    if(n<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = n;
                        // You're coming from the north, so the final heading is south
                        targetCosts[x][y].heading = SOUTH;
                    }
                    if(s<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = s;
                        targetCosts[x][y].heading = NORTH;
                    }
                    if(e<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = e;
                        targetCosts[x][y].heading = WEST;
                    }
                    if(w<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = w;
                        targetCosts[x][y].heading = EAST;
                    }
                    
                    if(current!=targetCosts[x][y].cost){
                        // Set changed to true to indicate that the loop should continue
                        changed=true;
                        if(PATHFINDER_DEBUG){
                            Serial.print("Found new cost to ");
                            Serial.print(x);
                            Serial.print(",");
                            Serial.print(y);
                            Serial.print(" old=");
                            Serial.print(current);
                            Serial.print(" new=");
                            Serial.println(targetCosts[x][y].cost);
                        }
                    } 
                }
            }
        }
    }
    
    // Find shortest path
    current = 1000;
    for(y=0; y<MAP_SIZE; y++){
        for(x=0; x<MAP_SIZE; x++){
            if(PATHFINDER_DEBUG){
                Serial.print(targetCosts[x][y].cost);
                Serial.print('|');
                Serial.print(targetCosts[x][y].heading);
                Serial.print('\t');
            }
            if(targetCosts[x][y].cost<current && (x!=currentX || y!=currentY)){
                // This one is shorter
                if(!m->isProcessed(x,y)){
                    current = targetCosts[x][y].cost;
                    this->targetX = x;
                    this->targetY = y;
                }
            }
        }
        if(PATHFINDER_DEBUG) Serial.println();
    }
}

bool Pathfinder::isTargetReachable(){
    return this->getCostTo(this->getTargetX(), this->getTargetY()) < DEFAULT_COST;
}

int Pathfinder::getTargetX(){
    return this->targetX;
}

int Pathfinder::getTargetY(){
    return this->targetY;
}

path Pathfinder::getPath(){
    path result;
    int length = 0;
    int i,x,y,heading;
    
    // Determine length of path
    x = this->targetX;
    y = this->targetY;
    while(x!=this->currentX || y!=this->currentY){
        heading = this->getTargetHeading(x,y);
        if(heading==NORTH) y+=1;
        if(heading==SOUTH) y-=1;
        if(heading==EAST)  x-=1;
        if(heading==WEST)  x+=1;
        length+=1;
    }
    
    x = this->targetX;
    y = this->targetY;
    result.length = length;
    result.directions = new int[length];
    
    for(i=length-1; i>=0; i--){
        heading = this->getTargetHeading(x,y);
        if(PATHFINDER_DEBUG){
            Serial.print("Next chunk in path (x,y,d): ");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.print(",");
            Serial.println(heading);
        }
        if(heading==NORTH) y+=1;
        if(heading==SOUTH) y-=1;
        if(heading==EAST)  x-=1;
        if(heading==WEST)  x+=1;
        result.directions[i] = heading;
    }
    
    return result;
}

int Pathfinder::getTargetHeading(int x, int y){
    if(x>=0 && x<MAP_SIZE && y>=0 && y<MAP_SIZE){
        return targetCosts[x][y].heading;
    }
    return NORTH;
}

int Pathfinder::getCostTo(int x, int y){
    if(x>=0 && x<MAP_SIZE && y>=0 && y<MAP_SIZE){
        return targetCosts[x][y].cost;
    }
    return 2 * DEFAULT_COST;
}

/** Calculate the amount of turns of 90 degrees needed to go from one heading to the other */
int Pathfinder::calculateTurningCost(int from, int to){
    int r = abs((to - from) % 4);
    return r==3 ? 1 : r;
}

int Pathfinder::calculateTurningDirection(int from, int to){
	return (from - to + 4) % 4 == 3 ? LEFT : RIGHT;
}
