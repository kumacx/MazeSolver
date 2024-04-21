#include "solver.h"
#include "file_handler.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
 
int GenerateBinaryInstructons(MazeData* maze, FILE* out) {
	int y = maze->start[0], x = maze->start[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = -1;

	Chunk* chunk = malloc(sizeof(Chunk) * 2);
	for (int k = 0; k < 2; k++)
	{
		chunk[k].tiles = malloc(sizeof(Tile*) * maze->chunkSize);
		for (int i = 0; i < maze->chunkSize; i++) {
			chunk[k].tiles[i] = malloc(sizeof(Tile) * maze->chunkSize);
		}
	}

	uint32_t stepsCounter = 0;


	int currentChunk = -1;

	int currentDir = -1;
	int lastDir = -1;
	int currentStreak = 1;

	int repeatLoop = 0;

	uint32_t tempValue = 0;
	fwrite(&tempValue, 4, 1, out);
	
	while (1) {
		repeatLoop = 0;
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, &chunk[0], currentChunk);
			if (distance == -1) {
				distance = chunk[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist;
			}
			
		}
		for (int i = 0; i < 4; i++)
		{
			if (chunk->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[0].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
				else {
					LoadChunk(maze, &chunk[1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunk[1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1){
			if (lastDir == -1) {
				lastDir = currentDir;
				continue;
			}

			if (lastDir == currentDir) {
				currentStreak++;
			}
			else {
				char printDir;
				if (lastDir == 0) {
					printDir = 'N';
				}
				else if (lastDir == 1) {
					printDir = 'S';
				}
				else if (lastDir == 2) {
					printDir = 'W';
				}
				else if (lastDir == 3) {
					printDir = 'E';
				}
				uint8_t temp = (int)currentStreak;
				fwrite(&printDir, 1, 1, out);
				fwrite(&temp, 1, 1, out);

				stepsCounter++;

				lastDir = currentDir;
				currentStreak = 1;
			}
			
			continue;
		}
		break;
	}
	char printDir;
	if (lastDir == 0) {
		printDir = 'N';
	}
	else if (lastDir == 1) {
		printDir = 'S';
	}
	else if (lastDir == 2) {
		printDir = 'W';
	}
	else if (lastDir == 3) {
		printDir = 'E';
	}
	uint8_t temp = (int)currentStreak;
	fwrite(&printDir, 1, 1, out);
	fwrite(&temp, 1, 1, out);
	
	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < maze->chunkSize; i++) {
			free(chunk[k].tiles[i]);
		}
		free(chunk[k].tiles);
	}
	free(chunk);
	
	return stepsCounter;
}

int GenerateInstructions(MazeData* maze, FILE* out)
{
	int y = maze->start[0], x = maze->start[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = -1;
	
	int answer = 0;

	Chunk* chunk = malloc(sizeof(Chunk) * 2);
	for (int k = 0; k < 2; k++)
	{
		chunk[k].tiles = malloc(sizeof(Tile*) * maze->chunkSize);
		for (int i = 0; i < maze->chunkSize; i++) {
			chunk[k].tiles[i] = malloc(sizeof(Tile) * maze->chunkSize);
		}
	}
	int currentChunk = -1;

	int dir = 3, currentDir;
	int currentForward = 0;

	int repeatLoop = 0;
	while (1)
	{
		repeatLoop = 0;
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, &chunk[0], currentChunk);
			if (distance == -1) {
				distance = chunk[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (chunk->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[0].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
				else {
					LoadChunk(maze, &chunk[1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunk[1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1) {
			if (dir == currentDir) {
				currentForward++;
			}
			else {
				if (currentForward > 0) {
					fprintf(out, "FORWARD %d\n", currentForward);
					answer++;
				}
				//up
				if (dir == 0 && currentDir == 3) {
					fprintf(out, "RIGHT\n");
					answer++;
				}
				else if (dir == 0 && currentDir == 2) {
					fprintf(out, "LEFT\n");
					answer++;
				}
				//down
				else if (dir == 1 && currentDir == 2) {
					fprintf(out, "RIGHT\n");
					answer++;
				}
				else if (dir == 1 && currentDir == 3) {
					fprintf(out, "LEFT\n");
					answer++;
				}
				//left
				else if (dir == 2 && currentDir == 0) {
					fprintf(out, "RIGHT\n");
					answer++;
				}
				else if (dir == 2 && currentDir == 1) {
					fprintf(out, "LEFT\n");
					answer++;
				}
				//right
				else if (dir == 3 && currentDir == 1) {
					fprintf(out, "RIGHT\n");
					answer++;
				}
				else if (dir == 3 && currentDir == 0) {
					fprintf(out, "LEFT\n");
					answer++;
				}
				dir = currentDir;
				currentForward = 1;
			}
			continue;
		}
		if (currentForward > 0) {
			fprintf(out, "FORWARD %d\n", currentForward);
			answer++;
		}
		break;
	}
	

	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < maze->chunkSize; i++) {
			free(chunk[k].tiles[i]);
		}
		free(chunk[k].tiles);
	}
	free(chunk);
	return answer;
}
