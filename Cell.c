/* 
   
Zach Martin | 357-07 | PROJECT 3 | Cell.c | 5/2/2016

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Cell.h"
#include "Report.h"


int main(int argc, char **argv) {
   Cell cell; 
   Report myReport;

   InitializeCell(&cell);

   FillCell(&cell, argv);

   InitializeReport(&cell, &myReport);
   
   RunSimulation(&cell, &myReport);
   
   exit(EXIT_SUCCESS);
   
}

void InitializeCell(Cell *cell) {
   cell->fixed = 0;
   cell->value = 0;
   cell->maxStep = -1;
   
   cell->id = -1;
   cell->outFds[0] = cell->outFds[1] = cell->outFds[2] = -1;
   cell->inFds[0] = cell->inFds[1] = -1;
}

void FillCell(Cell *cell, char **argv) {
   char letter;
   double number;
   char *end;  

   argv++;
   while (*argv) {
      letter = *(*argv)++;
      number = strtod(*argv, &end);
      switch (letter) {
      case 'V':
         if (cell->value == 0) {
            cell->value = number;
            cell->fixed = 1;
         }
         break;
      case 'S':
         if (cell->maxStep == -1 && number >= 0)
            cell->maxStep = number;	
         break;
      case 'O':
         SetOutFd(cell, argv, number);
         break;
      case 'I':
         SetInFd(cell, argv, number);
         break;
      case 'D':
         if (number >= 0 && cell->id == -1)
            cell->id = number;
         break;
      default:
         break;
      }   
      argv++; 
   }
}

void SetOutFd(Cell *cell, char **argv, int number) {
   int ndx = 0;

   if (number <= MAX_FDS) {
      while ((ndx < MAX_OUT_FDS) && (cell->outFds[ndx] != -1))
         ndx++;
      if (ndx < MAX_OUT_FDS) 
         cell->outFds[ndx] = number;
   }
   else {
      fprintf(stderr, "Error: out-fd %d is greater" 
       "allowed maximum %d\n", number, MAX_FDS);
      exit(EXIT_FAILURE);
   }
}

void SetInFd(Cell *cell, char **argv, int number) {
   int ndx = 0;

   if (number <= MAX_FDS) {
      while ((ndx < MAX_IN_FDS) && (cell->inFds[ndx] != -1))
         ndx++;
      if (ndx < MAX_IN_FDS) 
         cell->inFds[ndx] = number;
   }  
   else {
      fprintf(stderr, "Error: in-fd %d is greater" 
       "allowed maximum %d\n", number, MAX_FDS);
      exit(EXIT_FAILURE);
   }
}

void InitializeReport(Cell *cell, Report *report) {
   report->step = 0;
   report->id = cell->id;
   
   if (cell->fixed)
      report->value = cell->value;
   else 
      report->value = 0;
}

void RunSimulation(Cell *cell, Report *myReport) {
   Report left;
   Report right;
   
   while (myReport->step <= cell->maxStep) {
      WriteOut(cell, myReport);
      if (!cell->fixed) {
         left.report = 0;
         right.report = 0;
         ReadIn(cell, &left, &right, myReport);
         myReport->value = AverageReports(&left, &right);
      }
      myReport->step++;
   }
}

void WriteOut(Cell *cell, Report *report) {
   int ndx = 0;
   int size = sizeof(Report);

   while (ndx < MAX_OUT_FDS) {
      if (cell->outFds[ndx] != -1) {
         if (write(cell->outFds[ndx], report, size) != size) {
            fprintf(stderr, "Write Failed\n");
            exit(EXIT_FAILURE);
         }
      }
      ndx++;
   }
}

void ReadIn(Cell *cell, Report *left, Report *right, Report *myReport) {
   int size = sizeof(Report);

   if (cell->inFds[0] != -1) {
      if (read(cell->inFds[0], left, size) != size) {
         fprintf(stderr, "read Left failed:\n");
         exit(EXIT_FAILURE);
      }
      if (left->step != myReport->step) {
         fprintf(stderr, "read Left id bad:\n");
         exit(EXIT_FAILURE);
      }
   }
   if (cell->inFds[1] != -1) {
      if (read(cell->inFds[1], right, size) != size) {
         fprintf(stderr, "read Right failed:\n");
         exit(EXIT_FAILURE);
      }
      if (right->step != myReport->step) {
         fprintf(stderr, "read Right id bad:\n");
         exit(EXIT_FAILURE);
      }
   }

}

double AverageReports(Report *left, Report *right) {
   double ret = 0;

   ret += left->value;
   ret += right->value;
   ret /= 2;
   return ret;
}

/******************************* FOR TESTING *******************************/

void PrintCell(Cell *cell) {
   printf("ID %d\n", cell->id);
   printf("MaxStep %d\n", cell->maxStep);
   printf("OutFds %d %d %d\n", cell->outFds[0], cell->outFds[1], cell->outFds[2]);
   printf("InFds %d %d\n", cell->inFds[0], cell->inFds[1]);
   printf("Value: %f\n\n", cell->value);
}

void PrintReport(Report *report) {
   printf("Step: %d\n", report->step);
   printf("id: %d\n", report->id);
   printf("Value: %f\n\n", report->value);
}