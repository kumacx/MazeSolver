#include <stdio.h>

#ifndef SOLVER
#define SOLVER

typedef struct MazeDataStruct {
	int sizeY, sizeX;
	int start[2], end[2]; // {y, x}
	int chunksY, chunksX;
	int minInChunkY, minInChunkX;
	int recordSize;
	int chunksCache;
	int chunkSize;
	int terminatorSize;
	int debugMode;
}MazeData;

typedef struct TileStruct {
	char walls[5];
	int dist;
}Tile;

typedef struct ChunkStruct {
	Tile** tiles;
	int chunkIndex;
}Chunk;

#endif
