#include "map.h"

Map::Map(int mapSize){
  this->mapSize = mapSize;
  chunks = new Chunk[mapSize * mapSize];
  for(int i = 0 ; i < mapSize * mapSize ; i++){
  chunks[i] = *(Chunk*) calloc(1, sizeof(Chunk));
  }
//  memset(chunks, 0, (mapSize * mapSize) / 4);
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

Chunk* Map::getChunks(){
  return chunks;
}

int Map::getMapSize(){
  return mapSize;
}

String Map::toString(){
  Map m = *this;
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
  EEPROM.write(0, m.getMapSize());
  Chunk* chunks = m.getChunks();
  int length = m.getMapSize() * m.getMapSize();
  byte curr = 0;
  for(int i = 0 ; i < length; i++){
    Chunk c = chunks[i];
    curr |= (c.processed ? 1 : 0 | c.obstacle ? 2 : 0) >> (i % 4);
    if((i + 1) % 4 == 0){
      EEPROM.write(1 + i / 4, curr);
      curr = 0;
    }
  }
}

Map readFromEEPROM(){
  int length = EEPROM.read(0);
  Map m = *new Map(length);
  byte curr = 0;
  for(int i = 0 ; i < length * length ; i++){
     Chunk c = *m.chunkAt(i * length, i);
     c.processed = curr >> ((i % 4) * 2) & 1 == 1;
     c.obstacle = curr >> ((i % 4) * 2 + 1) & 1 == 1;
     if((i + 1) % 4 == 0){
       curr = EEPROM.read((i + 1) / 4);
     }
  }
  return m;
}
