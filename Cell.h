/* 
   
Zach Martin | 357-07 | PROJECT 3 | Cell.h | 5/2/2016

*/

#ifndef CELL_H
#define CELL_H

#define MAX_FDS 12
#define MAX_OUT_FDS 3
#define MAX_IN_FDS 2

#include "Report.h"

typedef struct Cell {
   int id;
   int maxStep;
   int outFds[MAX_OUT_FDS];
   int inFds[MAX_IN_FDS];
   double value;
   int fixed;
} Cell;

void InitializeCell(Cell *cell);

void FillCell(Cell *cell, char **argv);

void SetOutFd(Cell *cell, char **argv, int number);

void SetInFd(Cell *cell, char **argv, int number);

void InitializeReport(Cell *cell, Report *report);

void RunSimulation(Cell *cell, Report *myReport);

void WriteOut(Cell *cell, Report *report);

void ReadIn(Cell *cell, Report *left, Report *right, Report *myReport);

double AverageReports(Report *left, Report *right);

/*FOR TESTING*/

void PrintCell(Cell *cell);

void PrintReport(Report *report);

#endif

/*
InitializeCell
Initializes values for Cell struct

FillCell
Parses through commandline arguments to fill Cell struct

SetOutFd
If 'O' argument found in commandline, add following 
file desctiptor to OutFds arrary in Cell struct

SetInFd
If 'I' argument found in commandline, add following 
file desctiptor to InFds arrary in Cell struct

InitializeReport
Initalize this Cell's report struct.
ID and fixedValue (if given) from commandline argv are set. 

RunSimulation
Cell writes report sturct to all given OutFds.
If Cell does not have a fixedValue, reports are read in from 
InFds, the values are averaged and the current Cell's report is
set to this value.
Repeat this process for number of steps given by maxStep from argv

WriteOut
Writes Cell's report struct to given FD's in outFds

ReadIn
Reads in Cell reports from given FD's in InFds

AverageReports
Averages values given from neighboring Cell's reports,
and sets current Cell's value to the result
*/

