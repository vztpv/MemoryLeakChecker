
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FILE_NAME_SIZE 256

const char* INFO_FILE_NAME = "info.txt"; // to do
const char* RESULT_FILE_NAME = "info_result.txt";

typedef struct _Info
{
	char malloc_fileName[FILE_NAME_SIZE];
	long long malloc_line;
	void* ptr;
	char free_fileName[FILE_NAME_SIZE];
	long long free_line;
	int remove;
	int is_print;
} Info;


static Info* infoArr;
static long long count = 0;
static long long remove_count = 0;
static long long capacity = 1;
static int inited = 0;

static void init()
{
	capacity = 1024;
	infoArr = malloc(sizeof(Info)*capacity);
	count = 0;
	remove_count = 0;
}

void check_memory_leak();

static void remove_empty()
{
	long long i;
	long long k = 0;

	for (i = 0; i < count; ++i)
	{
		if (infoArr[i].remove) {
		}
		else {
			infoArr[k] = infoArr[i];
			k++;
		}
	}

	count = k;
	remove_count = 0;
}

static void add_info(void* ptr, char* fileName, size_t line)
{
	long long i;
	
	// chk existance!
	for (i = 0; i < count; ++i) {
		if (ptr == infoArr[i].ptr && infoArr[i].remove)
		{
			remove_empty();
			break;
		}
	}

	// expand!
	if (count >= capacity) {
		Info* temp;
		capacity = capacity * 2;
		temp = malloc(sizeof(Info)*capacity);

		for (i = 0; i < count; ++i) {
			temp[i] = infoArr[i];
		}
		free(infoArr);
	}
	
	// add
	{
		Info temp;
		temp.is_print = 0;
		temp.remove = 0;
		strncpy(temp.malloc_fileName, fileName, strlen(fileName)+1);
		temp.malloc_line = line;
		temp.ptr = ptr;
		
		infoArr[count] = temp;
		
		count++;
	}
}

static int remove_info(void* ptr, char* fileName, size_t line)
{
	int i;

	// chk existance!
	for (i = 0; i < count; ++i) {
		if (ptr == infoArr[i].ptr && !infoArr[i].remove)
		{
			break;
		}
	}
	if (i >= count) {
		printf("duplicate free!");
		{
			FILE* file = fopen(RESULT_FILE_NAME, "wt");
			
			fprintf(file, "fileName %s line %d", fileName, line);

			fclose(file);
		}

		exit(-2);
	}
	// remove?
	{
		infoArr[i].remove = 1;
		strncpy(infoArr[i].free_fileName, fileName, strlen(fileName) + 1);
		infoArr[i].free_line = line;

		remove_count++;
	}

	if (remove_count > 1024) {
		remove_empty();
	}

	return 1;
}

void* myMalloc(size_t size, char* fileName, size_t line)
{
	void* ptr = malloc(size);
	if (!inited) { init(); inited = 1; }

	add_info(ptr, fileName, line);
	
	return ptr;
}

void myFree(void* ptr, char* fileName, size_t line)
{
	if (!inited) { printf("error first free! no malloc! in file %s line %d", fileName, line); getchar(); exit(-1); }

	remove_info(ptr, fileName, line);
	
	free(ptr);
}

void check_memory_leak()
{
	remove_empty();
	{
		FILE* file = fopen(RESULT_FILE_NAME, "wt");
		int i;

		for (i = 0; i < count; ++i) {
			fprintf(file, "malloc_fileName : %s ", infoArr[i].malloc_fileName);
			fprintf(file, "malloc_fileLine : %lld\n", infoArr[i].malloc_line);
		}
		
		fclose(file);
	}
}

