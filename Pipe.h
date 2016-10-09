/* 

Zach Martin | 357-07 | PROJECT 3 | Pipe.h | 5/4/2016

*/

#ifndef PIPE_H
#define PIPE_H 

#define MAX_FD 13

typedef struct Pipes {
   int mainPipe[2];
   int nextPI[2];
   int prevPI[2];
   int nextPO[2];
   int prevPO[2];
} Pipes;

void SetupPipes(Pipes *pipes);

void ReversePipes(Pipes *pipes);

void MovePipes(Pipes *pipes);

void CleanPipes(int fds[MAX_FD]);

void ClosePipes(Pipes *pipes);

#endif

/*
SetupPipes
initializes the necessary pipes for each cell to communicate back 
to the parent process, LinearSim, as well as it's neighboring cells. 

MovePipes
In order to keep the number of file descriptors in use to a minnimum,
old FD's are closed and re-opened once they are no longer necessary.

ReversePipes
In order to use each pipe by the correct name, pipes are reversed at
each subsequent cell. (Ouput to the next cell becomes input from previous cell)

CleanPipes
Closes unecesarry FD's in child process before executing Cell program

ClosePipes
Closes unecesarry FD's in parent process once all Cell's have been started.
*/