#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct {
	uint32_t file_id;
	uint8_t escape;
	uint16_t columns;
	uint16_t lines;
	uint16_t entry_x;
	uint16_t entry_y;
	uint16_t exit_x;
	uint16_t exit_y;
	uint64_t reserved_1;
	uint32_t reserved_2;
	uint32_t counter;
	uint32_t solution_offset;
	uint8_t separator;
	uint8_t wall;
	uint8_t path;
} HeaderBin;

void OptimalValues(MazeData* maze) {
	int shorterSize = maze->sizeX < maze->sizeY ? maze->sizeX : maze->sizeY;
	if (shorterSize < 10) {
		maze->chunkSize = 2;
		maze->chunksCache = 2;
	}
	else if (shorterSize < 100) {
		maze->chunkSize = 5;
		maze->chunksCache = 3;
	}
	else if (shorterSize < 500) {
		maze->chunkSize = 15;
		maze->chunksCache = 5;
	}
	else if (shorterSize < 1000) {
		maze->chunkSize = 20;
		maze->chunksCache = 6;
	}
	else {
		maze->chunkSize = 25;
		maze->chunksCache = 7;
	}
}

int BinaryRead(char *fileName, char *outFileName, MazeData *maze)
{
	FILE* in = fopen(fileName, "rb");
	FILE* out = fopen(outFileName, "w");
	if (in == NULL || out == NULL) {
		return -1;
	}
	HeaderBin header;
	fread(&header.file_id, 4, 1, in);
	fread(&header.escape, 1, 1, in);
	fread(&header.columns, 2, 1, in);
	fread(&header.lines, 2, 1, in);
	fread(&header.entry_x, 2, 1, in);
	fread(&header.entry_y, 2, 1, in);
	fread(&header.exit_x, 2, 1, in);
	fread(&header.exit_y, 2, 1, in);
	fread(&header.reserved_1, 8, 1, in);
	fread(&header.reserved_2, 4, 1, in);
	fread(&header.counter, 4, 1, in);
	fread(&header.solution_offset, 4, 1, in);
	fread(&header.separator, 1, 1, in);
	fread(&header.wall, 1, 1, in);
	fread(&header.path, 1, 1, in);

	maze->sizeX = (header.columns - 1) / 2;
	maze->sizeY = (header.lines - 1) / 2;
	maze->start[0] = header.entry_y == header.lines ? (header.entry_y - 2) / 2 : (header.entry_y - 1) / 2;
	maze->start[1] = header.entry_x == header.columns ? (header.entry_x - 2) / 2 : (header.entry_x - 1) / 2;
	maze->end[0] = header.exit_y == header.lines ? (header.exit_y - 2) / 2 : (header.exit_y - 1) / 2;
	maze->end[1] = header.exit_x == header.columns ? (header.exit_x - 2) / 2 : (header.exit_x - 1) / 2;
	if (maze->debugMode == 1) {
		printf("File ID: 0x%X\n", header.file_id);
		printf("Columns: %u\n", header.columns);
		printf("Lines: %u\n", header.lines);
		printf("Entry X: %u\n", header.entry_x);
		printf("Entry Y: %u\n", header.entry_y);
		printf("Exit X: %u\n", header.exit_x);
		printf("Exit Y: %u\n", header.exit_y);
		printf("Counter: %u\n", header.counter);
		printf("Solution Offset: %u\n", header.solution_offset);
		printf("Separator: %u\n", header.separator);
		printf("Wall: %u\n", header.wall);
		printf("Path: %u\n", header.path);
	}

	maze->counter = header.counter;
	
	int y = 0, x = 0;
	char* line = malloc(sizeof(char) * (header.columns + 2));
	for (int i = 0; i < header.counter; i++) {
		uint8_t s;
		uint8_t v;
		uint8_t c;
		fread(&s, 1, 1, in);
		fread(&v, 1, 1, in);
		fread(&c, 1, 1, in);

		

		for (int i = 0; i < c + 1; i++) {
			if (v == header.wall) {
				line[x] = 'X';
			}
			else if (v == header.path) {
				line[x] = ' ';
			}
			x++;
		}
		if (x == header.columns) {
			line[x] = '\n';
			line[x + 1] = '\0';
			x = 0;
			y++;
			fprintf(out, "%s", line);
		}
	}
	free(line);

	fclose(in);
	fclose(out);
	return 0;
}

void PrintMaze(MazeData* maze) {
	Tile* tile = malloc(sizeof(Tile));

	printf("\n");
	for (int y = 0; y < maze->sizeY; y++)
	{
		if (y == 0)
		{
			printf("XX");
			for (int x = 0; x < maze->sizeX; x++)
			{
				LoadTile(maze, tile, y, x);
				printf("%c%cXX", tile->walls[0], tile->walls[0]);
			}
			printf("\n");
		}
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTile(maze, tile, y, x);
			if (x == 0) {
				if (tile->dist < 10)
				{
					printf("%c%c%d %c%c", tile->walls[2], tile->walls[2], tile->dist, tile->walls[3], tile->walls[3]);
				}
				else printf("%c%c%d%c%c", tile->walls[2], tile->walls[2], tile->dist, tile->walls[3], tile->walls[3]);
			}
			else
			{
				if (tile->dist < 10)
				{
					printf("%d %c%c", tile->dist, tile->walls[3], tile->walls[3]);
				}
				else printf("%d%c%c", tile->dist, tile->walls[3], tile->walls[3]);
			}
		}
		printf("\n");
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTile(maze, tile, y, x);
			if (x == 0) {
				printf("XX%c%cXX", tile->walls[1], tile->walls[1]);
			}
			else printf("%c%cXX", tile->walls[1], tile->walls[1]);
		}
		printf("\n");
	}
	free(tile);
}

void AddNumberToText(char* text, int number)
{
	char tempNum[20];
	sprintf(tempNum, "%d", number);
	strcat(text, tempNum);
	return;
}

void ClearAllChunks(int max, int StopAfterError, int showMessage)
{
	if(showMessage == 1) printf("Removing Data, successfully removed:\n");
	int count = 0;
	for (int i = 0; i <= max; i++) {
		char fileName[30] = "chunk_";
		AddNumberToText(fileName, i);

		if (remove(fileName) == 0)
		count++;
		else if (StopAfterError == 1) {
			break;
		}
	}
	if (showMessage == 1) printf("Removed %d files\n\n", count);
	return;
}

void UpdateChunk(MazeData *maze, Chunk *chunk)
{
	int horizontalNumber = chunk->chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunk->chunkIndex / maze->chunksX == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunk->chunkIndex);
	FILE* out = fopen(fileName, "wb");

	char* data = malloc(sizeof(char) * (horizontalNumber * verticalNumber * maze->recordSize + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	for (int y = 0; y < verticalNumber; y++)
	{
		for (int x = 0; x < horizontalNumber; x++)
		{
			for (int i = 0; i < 4; i++)
			{
				data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = chunk->tiles[y][x].walls[i];
			}
			data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + 4] = ' ';
			sprintf(tempNum, "%d", chunk->tiles[y][x].dist);
			int temp = strlen(tempNum);
			for (int i = 5; i < maze->recordSize; i++)
			{
				if (i < temp + 5)
					data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = tempNum[i - 5];
				else
					data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = ' ';
			}
		}
	}
	data[horizontalNumber * verticalNumber * maze->recordSize] = '\0';
	fwrite(data, sizeof(char), horizontalNumber * verticalNumber * maze->recordSize, out);

	fclose(out);
	free(tempNum);
	free(data);
}

void LoadTile(MazeData* maze, Tile* tile, int y, int x)
{
	int chunkIndex = GetChunkIndex(maze, y, x);
	int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunkIndex / maze->chunksX == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	char* data = malloc(sizeof(char) * (maze->recordSize + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));

	fseek(in, (horizontalNumber * (y % maze->chunkSize) + x % maze->chunkSize) * maze->recordSize, SEEK_SET);
	fread(data, sizeof(char), maze->recordSize, in);
	for (int i = 0; i < 4; i++)
		tile->walls[i] = data[i];
	tile->walls[4] = '\0';
	for (int i = 5; i < maze->recordSize; i++)
	{
		tempNum[i - 5] = data[i];
	}
	tempNum[maze->recordSize - 5] = '\0';
	tile->dist = atoi(tempNum);

	fclose(in);
	free(data);
	free(tempNum);
}

int VerifyFile(char* fileName, MazeData* maze)
{
	FILE* in = fopen(fileName, "rb");
	if(in == NULL){
		return 1;
	}
	int initialCounter = 0;

	char c;
	while (1)
	{
		c = fgetc(in);
		
		if (c == '\n' || c == '\r') {
			maze->terminatorSize++;	
		}
		else {
			if (maze->terminatorSize > 0) {
				break;
			}
			initialCounter++;
		}
	}
	//configure based on initial counter
	if (initialCounter % 2 != 1) {
		return 1;
	}

	maze->sizeX = (initialCounter - 1) / 2;
	

	int tempX = 0;
	int tempY = 0;
	int tempTerm = 0;

	fseek(in, 0, SEEK_SET);

	while (1)
	{
		c = fgetc(in);
		if (c == '\n' || c == '\r')
		{
			tempTerm++;
			if (tempTerm == maze->terminatorSize) {
				tempTerm = 0;
				tempX = 0;
				tempY++;
			}
			continue;
		}
		else if(c == 'X' || c == ' ' || c == 'K' || c == 'P'){
			tempX++;
		}
		else if (c == -1) {
			break;
		}
		else {
			return 1;
		}
	}

	maze->sizeY = (tempY - 1) / 2;

	if (maze->chunkSize == -1 && maze->chunksCache == -1) {
		OptimalValues(maze);
	}
	maze->chunksY = maze->sizeY % maze->chunkSize == 0 ? maze->sizeY / maze->chunkSize : maze->sizeY / maze->chunkSize + 1;
	maze->chunksX = maze->sizeX % maze->chunkSize == 0 ? maze->sizeX / maze->chunkSize : maze->sizeX / maze->chunkSize + 1;

	//start-end
	maze->start[0] = 0; maze->start[1] = 0;
	maze->end[0] = maze->sizeY - 1; maze->end[1] = maze->sizeX - 1;

	//mins
	maze->minInChunkY = maze->sizeY % maze->chunkSize == 0 ? maze->chunkSize : maze->sizeY % maze->chunkSize;
	maze->minInChunkX = maze->sizeX % maze->chunkSize == 0 ? maze->chunkSize : maze->sizeX % maze->chunkSize;

	fclose(in);
	return 0;
}

void LoadChunk(MazeData* maze, Chunk* chunk, int chunkIndex)
{
	chunk->chunkIndex = chunkIndex;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunkIndex / maze->chunksX == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	int length = horizontalNumber * verticalNumber * maze->recordSize;
	char* data = malloc(sizeof(char) * (length + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	fread(data, sizeof(char), length, in);

	for (int y = 0; y < maze->chunkSize; y++)
	{
		for (int x = 0; x < maze->chunkSize; x++)
		{
			if (y < verticalNumber && x < horizontalNumber)
			{
				for (int i = 0; i < 4; i++)
				{
					chunk->tiles[y][x].walls[i] = data[y * maze->recordSize * horizontalNumber + x * maze->recordSize + i];
					//printf("%d ", y * maze->recordSize * maze->chunkSize + x * maze->recordSize + i);
				}
				chunk->tiles[y][x].walls[4] = '\0';
				for (int i = 5; i < maze->recordSize; i++)
				{
					tempNum[i - 5] = data[y * maze->recordSize * horizontalNumber + x * maze->recordSize + i];
				}
				tempNum[maze->recordSize - 5] = '\0';
				chunk->tiles[y][x].dist = atoi(tempNum);
			}
			else {
				strcpy(chunk->tiles[y][x].walls, "NNNN");
				chunk->tiles[y][x].dist = -1;
			}
		}
	}
	fclose(in);
	free(tempNum);
	free(data);
}

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue)
{
	FILE* in = fopen(fileName, "rb");
	FILE* out = NULL;

	char* additionalFill = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	sprintf(additionalFill, "%d", fillValue);
	int tempLen = strlen(additionalFill);
	for (int i = tempLen; i < maze->recordSize - 5; i++) {
		additionalFill[i] = ' ';
	}
	additionalFill[maze->recordSize - 5] = '\0';

	for (int y = 0; y < maze->chunksY; y++)
	{
		for (int x = 0; x < maze->chunksX; x++)
		{
			int chunkIndex = y * maze->chunksX + x;
			int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
			int verticalNumber = chunkIndex / maze->chunksX == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

			char tempFileName[30] = "chunk_";
			AddNumberToText(tempFileName, chunkIndex);
			out = fopen(tempFileName, "w");

			int lineLength = horizontalNumber * 2 + 1;
			char** chunkText = malloc(sizeof(char*) * (verticalNumber * 2 + 1));
			for(int i = 0; i < verticalNumber * 2 + 1; i++) chunkText[i] = malloc(sizeof(char) * (lineLength + 1));
			//readchunk
			for (int i = 0; i < verticalNumber * 2 + 1; i++)
			{
				int seekIndex = (y * 2 * maze->chunkSize + i) * (maze->sizeX * 2 + 1 + maze->terminatorSize) + (x * maze->chunkSize) * 2;
				fseek(in, seekIndex, SEEK_SET);
				fread(chunkText[i], sizeof(char), lineLength, in);
				chunkText[i][lineLength] = '\0';
				//printf("%s %d\n", line[i], seekIndex);
			}
			//processchunk
			char* record = malloc(sizeof(char) * (maze->recordSize + 1));
			char* sumRecord = malloc(sizeof(char) * (maze->recordSize * verticalNumber * horizontalNumber + 1));
			int sumCounter = 0;
			for (int tempY = 1; tempY < verticalNumber * 2 + 1; tempY+=2)
			{
				for (int tempX = 1; tempX < horizontalNumber * 2 + 1; tempX+=2)
				{
					record[0] = chunkText[tempY - 1][tempX];
					record[1] = chunkText[tempY + 1][tempX];
					record[2] = chunkText[tempY][tempX - 1];
					record[3] = chunkText[tempY][tempX + 1];
					record[4] = ' ';

					for (int i = 5; i < maze->recordSize; i++)
					{
						record[i] = additionalFill[i - 5];
					}
					record[maze->recordSize] = '\0';

					for (int i = 0; i < maze->recordSize; i++) {
						sumRecord[i + sumCounter] = record[i];
					}
					sumCounter += maze->recordSize;

				}
			}
			//save to file and free
			sumRecord[maze->recordSize * verticalNumber * horizontalNumber] = '\0';
			fprintf(out, "%s", sumRecord);

			for (int i = 0; i < verticalNumber * 2 + 1; i++) free(chunkText[i]);
			free(chunkText);
			free(record);
			free(sumRecord);
			fclose(out);
		}
	}

	fclose(in);
	free(additionalFill);
}
