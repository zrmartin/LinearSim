/*

Zach Martin | 357-07 | PROJECT 3 | LinearSim.h | 5/4/2016

*/

#ifndef LINEARSIM_H
#define LINEARSIM_H

#define MAX_CELLS 50
#define MAX_ARGS 9
#define MAX_FD 13

typedef struct Flags {
   int maxStep;
   int cells;
   double left;
   double right;
} Flags;

typedef struct Pipes {
   int mainPipe[2];
   int nextPI[2];
   int prevPI[2];
   int nextPO[2];
   int prevPO[2];
} Pipes;

void InitializeFlags(Flags *flags);

void FillFlags(Flags *flag, char **argv);

void CheckFlags(Flags *flags);

void InitializeVariables(Flags *flags, int **reportCount,
 int **pids, char ***pcellArgv, int **pfails);

void StartCells(Flags *flags, char **cellArgv, Pipes *pipes, int *pids);

void FreeArgv(char **cellArgv);

void PrintCells(Pipes *pipes, int *reportCount);

void WaitCells(int *pids, int *fails, int cells);

void CheckErrors(int *fails, Flags *flags, int *reportCount);

#endif

/*
InitializeFlags
Initializes struct containing LinearSim's commandline arguments to -1

FillFlags
Parses through commandline arguments to fill Flags struct

CheckFlags
Error checking of commandline arguments

InitializeVariables
Allocates space on RTH for variables needed throughout program.

StartCells
Starts the number of cells given by the commandline argument. 
Moves, and reverses pipes if necesarry
Creates the necessary argv needed for the Cell executable. 

FreeArgv
Free's allocated memory used for Cell executable argv

PrintCells
Prints report structs sent to parent process from Cell children 
(No particular order guarenteed)

WaitCells
Waits for each child proccess Cell executable to finish.
If a child returns a non 0 value, records Cell number and error code.

CheckErrors
Checks if any Cells returned a non 0 value, or if any Cell reported too few
or too many reports back to parent process.
*/



