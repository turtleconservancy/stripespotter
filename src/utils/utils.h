/* utils.h
 *
 * General purpose, reusable routines for Linux (mainly)
 *
 * Copyright (c) 2007-09 Mayank Lahiri
 *
 */
#ifndef __UTILS_H
#define __UTILS_H


/* 
 * COMMAND LINE PARSING
 */
enum { CMDLINE_BINARY, CMDLINE_INTEGER, CMDLINE_DOUBLE, CMDLINE_STRING, CMDLINE_HELP };	/* arg types */
typedef struct CMDLINE_PARAMETERS	{
	const char *option;
	int arg_type;
	void *where_to;
	int min, max;
	int required;
	const char *description;
} CMDLINE_PARAMETERS;

int ParseCommandLine(int argc, char **argv, CMDLINE_PARAMETERS *parameters);


/*
 * MEMORY USAGE QUERYING FOR LINUX
 */

int getSystemMemory();
int getSystemFreeMemory();
int getMemoryPeak();
int getMemoryCurrent();



/*
 * USERSPACE CPU USAGE IN SECONDS (LINUX ONLY)
 */
void startUserClock();
double stopUserClock();
void startKernelClock();
double stopKernelClock();
void startClocks();
double stopClocks();

/*
 * TEXT ROUTINES
 */
void chomp(char *s);


#endif
