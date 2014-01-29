#include "map.h"

Map::Map(int mapSize){
  this->mapSize = mapSize;
  chunks = new Chunk[mapSize * mapSize];
  for(int i = 0 ; i < mapSize ; i++){
    for(int j = 0 ; j < mapSize ; j++){
      Chunk* c = chunkAt(i, j);
      c->processed = false;
      c->obstacle = false;
    }
  }
}

Chunk* Map::chunkAt(int x, int y){
  return chunks + x + y * mapSize;
}

void Map::obstacle(int x, int y){
  chunkAt(x, y)->obstacle = true;
}

void Map::processed(int x, int y){
  chunkAt(x, y)->processed = true;
}

boolean Map::isProcessed(int x, int y){
    return chunkAt(x, y)->processed;
}

boolean Map::isObstacle(int x, int y){
    return chunkAt(x, y)->obstacle;
}

Chunk* Map::getChunks(){
  return chunks;
}

int Map::getMapSize(){
  return mapSize;
}

String Map::toString(){
  Chunk* chunks = getChunks();
  int length = mapSize * mapSize;
  int counter = 0;
  char result[length + mapSize];
  for(int i = 0 ; i < length ; i++){
    Chunk c = chunks[i];
    result[counter++] = c.obstacle ? 'o' : c.processed ? 'x' : '-';
    if((i + 1) % mapSize == 0){
      result[counter++] = '\n';
    }
  }
  result[counter - 1] = '\0';
  return result;
}

void writeToEEPROM(Map m){
  Chunk* chunks = m.getChunks();
  int length = m.getMapSize() * m.getMapSize();
  byte curr = 0;
  for(int i = 0 ; i < length; i++){
    Chunk c = chunks[i];
    curr |= ((c.processed ? 1 : 0) | (c.obstacle ? 2 : 0)) << ((i % 4) * 2);
    //Serial.println(((c.processed ? 1 : 0) | (c.obstacle ? 2 : 0)), BIN);
    if(i%4 == 3 || i == length-1){
      //Serial.println(curr, BIN);
      EEPROM.write(i / 4, curr);
      curr = 0;
    }
  }
}

void readFromEEPROM(Map* m){
  int length = m->getMapSize() * m->getMapSize();
  byte curr = 0;
  for(int i = 0 ; i < length ; i++){
    if(i % 4 == 0){
      curr = EEPROM.read(i / 4);
    }
    Chunk* c = m->chunkAt(i % m->getMapSize(), i / m->getMapSize());
    c->processed = (curr >> ((i % 4) * 2) & 1) == 1;
    c->obstacle = (curr >> ((i % 4) * 2 + 1) & 1) == 1;
  }
}

