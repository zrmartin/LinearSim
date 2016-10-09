/* 
   
Zach Martin | 357-07 | PROJECT 3 | CreateArgv.h | 5/2/2016

*/

#ifndef CREATEARGV_H
#define CREATEARGV_H

#define ONE_INTERIOR 3
#define ARG_BUF 10 
#define MAX_FD 13

typedef struct Flags {
   int maxStep;
   int cells;
   double left;
   double right;
} Flags;

void NewArg(char **pArg);

void AddSD(char **cellArgv, Flags *flags, int id, int fds[MAX_FD]);

void AddLeft(char **cellArgv, Flags *flags, int mainPipe, 
int nextPO, int fds[MAX_FD]);

void AddRight(char **cellArgv, Flags *flags, int mainPipe, 
 int prevPO, int fds[MAX_FD]);

void AddFirst(char **cellArgv, Flags *flags, int mainPipe, int prevPI,
 int nextPI, int nextPO, int fds[MAX_FD]);
 
void AddLast(char **cellArgv, Flags *flags, int mainPipe, int prevPI,
 int nextPI, int prevPO, int fds[MAX_FD]);

void AddInterior(char **cellArgv, Flags *flags, int mainPipe, int prevPO,
 int nextPO, int prevPI, int nextPI, int fds[MAX_FD]);

#endif

/*
NewArg
Allocates space on RTH for a new commandline argument for Cell program

AddSD
Adds arguments maxSteps (S) and id (D) to Cell's argv

AddLeft
Adds arguments for the left, fixed-value Cell.

AddRight
Adds arguments for the right, fixed-value Cell.

AddFirst
Adds arguments for the first interior Cell.

AddLast 
Adds arguments for the last interior Cell.

AddInterior
Adds arguments for any other interior Cell.
*/