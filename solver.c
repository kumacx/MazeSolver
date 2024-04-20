#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void MoveToPositionCache(MazeData* maze, Chunk* chunks, int startPosition, int position)
{
	for (int i = startPosition; i > 0; i--) {
		SwapValues(maze, &chunks[i], &chunks[i - 1]);
	}
	return;
}

int ExistsInCache(MazeData* maze, Chunk* chunks, int targetIndex)
{
	for (int i = 0; i < maze->chunksCache; i++) {
		if (chunks[i].chunkIndex == targetIndex) {
			return i;
		}
	}
	return -1;
}

int GetChunkIndex(MazeData* maze, int y, int x)
{
	return maze->sizeX % maze->chunkSize == 0 ? (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize) + (x / maze->chunkSize) : (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize + 1) + (x / maze->chunkSize);
}

void SwapValues(MazeData *maze, Chunk *first, Chunk *second)
{
	int temp;
	char tempC;

	temp = first->chunkIndex;
	first->chunkIndex = second->chunkIndex;
	second->chunkIndex = temp;

	for (int y = 0; y < maze->chunkSize; y++)
	{
		for (int x = 0; x < maze->chunkSize; x++)
		{
			temp = first->tiles[y][x].dist;
			first->tiles[y][x].dist = second->tiles[y][x].dist;
			second->tiles[y][x].dist = temp;

			for (int i = 0; i < 4; i++)
			{
				tempC = first->tiles[y][x].walls[i];
				first->tiles[y][x].walls[i] = second->tiles[y][x].walls[i];
				second->tiles[y][x].walls[i] = tempC;
			}
		}
	}
}
void FillWithDistances(MazeData* maze)
{
	int y = maze->end[0], x = maze->end[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = 1, currentChunk = -1;
	//alloc cache
	int tempY = 0, tempX = 0;
	Chunk* chunks = malloc(sizeof(Chunk) * maze->chunksCache);
	for (int i = 0; i < maze->chunksCache; i++)
	{
		chunks[tempY * maze->chunksX + tempX].tiles = malloc(sizeof(Tile*) * maze->chunkSize);
		for (int j = 0; j < maze->chunkSize; j++)
		{
			chunks[tempY * maze->chunksX + tempX].tiles[j] = malloc(sizeof(Tile) * maze->chunkSize);
		}
		tempX++;
		if(tempX == maze->chunksX){
			tempY++;
			tempX = 0;
		}

	}
	//load first 10
	for (int i = 0; i < maze->chunksCache; i++) {
		LoadChunk(maze, &chunks[i], i);
	}
	//loop
	LoadChunk(maze, &chunks[maze->chunksCache - 1], GetChunkIndex(maze, y, x));
	//
	while (1)
	{
		int repeatLoop = 0;
		//move to index 0 actual chunk
		currentChunk = GetChunkIndex(maze, y, x);
		MoveToPositionCache(maze, chunks, ExistsInCache(maze, chunks, currentChunk), 0);
		//distance update
		if (distance < chunks[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist) {
			chunks[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist = distance;
		}
		//looking for higher values
		for (int i = 0; i < 4; i++)
		{
			if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				int tempIndex = GetChunkIndex(maze, y + modY[i], x + modX[i]);
				int tempPosition = ExistsInCache(maze, chunks, tempIndex);
				if (tempPosition != -1) {
					if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].dist + 1 < chunks[tempPosition].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist) {
						y += modY[i];
						x += modX[i];
						distance++;
						repeatLoop = 1;
						break;
					}
				}
				else {
					UpdateChunk(maze, &chunks[maze->chunksCache - 1]);
					LoadChunk(maze, &chunks[maze->chunksCache - 1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].dist + 1 < chunks[maze->chunksCache - 1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist) {
						y += modY[i];
						x += modX[i];
						distance++;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1) {
			continue;
		}
		//looking for lower values
		for (int i = 0; i < 4; i++)
		{
			if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				int tempIndex = GetChunkIndex(maze, y + modY[i], x + modX[i]);
				int tempPosition = ExistsInCache(maze, chunks, tempIndex);
				if (tempPosition != -1) {
					if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].dist - 1 == chunks[tempPosition].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist) {
						y += modY[i];
						x += modX[i];
						distance--;
						repeatLoop = 1;
						break;
					}
				}
				else {
					UpdateChunk(maze, &chunks[maze->chunksCache - 1]);
					LoadChunk(maze, &chunks[maze->chunksCache - 1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunks->tiles[y % maze->chunkSize][x % maze->chunkSize].dist - 1 == chunks[maze->chunksCache - 1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist) {
						y += modY[i];
						x += modX[i];
						distance--;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1) {
			continue;
		}
		//
		for (int i = 0; i < maze->chunksCache; i++) {
			UpdateChunk(maze, &chunks[i]);
		}

		break;
	}
	//dealloc
	for (int i = 0; i < maze->chunksCache; i++)
	{
		for (int j = 0; j < maze->chunkSize; j++)
		{
			free(chunks[i].tiles[j]);
		}
		free(chunks[i].tiles);
	}
	free(chunks);
}
