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
	int t;
	int counter;
} MazeData;

typedef struct TileStruct {
	char walls[5];
	int dist;
}Tile;

typedef struct ChunkStruct {
	Tile** tiles;
	int chunkIndex;
}Chunk;

void MoveToPositionCache(MazeData* maze, Chunk* chunks, int startPosition, int position);

int ExistsInCache(MazeData* maze, Chunk* chunks, int targetIndex);

int GetChunkIndex(MazeData* maze, int y, int x);

void SwapValues(MazeData* maze, Chunk* first, Chunk* second);

void FillWithDistances(MazeData* maze);

#endif
