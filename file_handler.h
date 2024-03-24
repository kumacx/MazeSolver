#include "solver.h"
#include <stdio.h>

#ifndef FILE_HANDLER
#define FILE_HANDLER

void PrintMaze(MazeData* maze);

void AddNumberToText(char* text, int number);

void ClearAllChunks(int max, int StopAfterError);

void UpdateChunk(MazeData* maze, Chunk* chunk);

void LoadChunk(MazeData* maze, Chunk* chunk, int chunkIndex);

void LoadTile(MazeData* maze, Tile* tile, int y, int x);

int VerifyFile(char* fileName, MazeData* maze);

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue);

#endif