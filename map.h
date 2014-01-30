#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

#ifndef MAP_H
#define MAP_H
struct Chunk{
	/**
	* wheter or not this Chunk has been processed
	*/
	boolean processed;
	
	/**
	* wheter or not this Chunk is an obstacle
	*/
	boolean obstacle;
};

class Map{
  public:
    Map(int mapSize);
    Chunk* chunkAt(int x, int y);
    void obstacle(int x, int y);
    void processed(int x, int y);
    boolean isProcessed(int x, int y);
    boolean isObstacle(int x, int y);
    Chunk* getChunks();
    int getMapSize();
    String toString();
    
  private:
    Chunk* chunks;
    int mapSize;
};

void writeToEEPROM(Map m);
void readFromEEPROM(Map* m);
#endif