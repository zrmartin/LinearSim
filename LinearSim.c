/*
Zach Martin | 357-07 | PROJECT 3 | LinearSim.c | 5/4/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Report.h"
#include "LinearSim.h"

int main(int argc, char **argv) {
   Flags flags;
   int *reportCount;
   int *pids;
   int *fails;
   char **cellArgv;
   Pipes pipes;
   
   InitializeFlags(&flags);
   FillFlags(&flags, argv);
   CheckFlags(&flags);
   InitializeVariables(&flags, &reportCount, &pids, &cellArgv, &fails);

   SetupPipes(&pipes);
   StartCells(&flags, cellArgv, &pipes, pids);
   ClosePipes(&pipes);
   
   PrintCells(&pipes, reportCount);
   WaitCells(pids, fails, flags.cells);

   CheckErrors(fails, &flags, reportCount);

   free(pids);
   free(reportCount);
   free(fails);
   exit(EXIT_SUCCESS);  
   
}

void InitializeFlags(Flags *flags) {
   flags->maxStep = -1;
   flags->cells = -1;
   flags->left = -1;
   flags->right  = -1;
}

void FillFlags(Flags *flags, char **argv) {
   char letter; 
   double number;
   char *end;

   argv++;
   while (*argv) {
      letter = *(*argv)++;
      number = strtod(*argv, &end);
      switch (letter) {
      case 'C':
         if (flags->cells == -1) {
            if (number > 0 && number <= MAX_CELLS)
               flags->cells = number;
         }
         break;
      case 'S':
         if (flags->maxStep == -1 && number >= 0)
            flags->maxStep = number;
         break;
      case 'L':
         if (flags->left == -1) 
            flags->left = number;
         break;
      case 'R':
         if (flags->right == -1) 
            flags->right = number;
         break;
      default:
         break; 
      } 
      argv++;   
   }
}

void CheckFlags(Flags *flags) {
   char *usageError = "Usage: LinearSim C S L R (in any order)\n";

   if (flags->maxStep == -1 || flags->cells == -1) {
      fprintf(stderr, usageError);
      exit(EXIT_FAILURE);
   }
   if (flags->left != -1 && flags->right != -1 && flags->cells < 2) {
      fprintf(stderr, usageError);
      exit(EXIT_FAILURE);   
   }
}

void InitializeVariables(Flags *flags, int **reportCount,
 int **pids, char ***pcellArgv, int **pfails) {
   if (!(*reportCount = calloc(sizeof(int), flags->cells))) {
      fprintf(stderr, "Error allocating memory\n");
      exit(EXIT_FAILURE);
   }
   if (!(*pids = calloc(sizeof(int), flags->cells))) {
      fprintf(stderr, "Error allocating memory\n");
      exit(EXIT_FAILURE);
   }
   if (!(*pfails = calloc(sizeof(int), (flags->cells + 1) * 2))) {
      fprintf(stderr, "Error allocating memory\n");
      exit(EXIT_FAILURE);
   }
}


void StartCells(Flags *flags, char **cellArgv, Pipes *pipes, int *pids) {
   int id;
   int cellPID;
   int fds[MAX_FD];

   for (id = 0; id < flags->cells; id++) {  
      cellArgv = calloc(MAX_ARGS, sizeof(char *));
      *cellArgv = "./Cell";
      AddSD(cellArgv, flags, id, fds);
      if (id == 0) 
         AddLeft(cellArgv, flags, pipes->mainPipe[1], 
          pipes->nextPO[1], fds);
      else {
         ReversePipes(pipes);
         MovePipes(pipes);
         if (id == flags->cells - 1)
            AddRight(cellArgv, flags, pipes->mainPipe[1], 
             pipes->prevPO[1], fds);
         else if (id == 1) 
            AddFirst(cellArgv, flags, pipes->mainPipe[1], pipes->prevPI[0],
             pipes->nextPI[0], pipes->nextPO[1], fds);
         else if (id == flags->cells - 2)
            AddLast(cellArgv, flags, pipes->mainPipe[1], pipes->prevPI[0],
             pipes->nextPI[0], pipes->prevPO[1], fds);
         else 
            AddInterior(cellArgv, flags, pipes->mainPipe[1], pipes->prevPO[1],
             pipes->nextPO[1], pipes->prevPI[0], pipes->nextPI[0], fds);
      }

      if ((cellPID = fork()) < 0)  {
         fprintf(stderr, "fork failed\n");
         exit(EXIT_FAILURE);
      }
      else if (cellPID == 0) {
         CleanPipes(fds);
         execv("./Cell", cellArgv);
         fprintf(stderr, "Exec failure No such file or directory\n");
      }
      else {
         *(pids + id) = cellPID;
         FreeArgv(cellArgv);
		 free(cellArgv);
      }
   }
}

void FreeArgv(char **cellArgv) {
   char *temp;
   cellArgv++;	//Skipping ./Cell argument
   
   while (*cellArgv) {
      temp = *cellArgv;
      cellArgv++;
      free(temp);
	  temp = NULL;
   }
}

void PrintCells(Pipes *pipes, int *reportCount) {
   Report report;

   close(pipes->mainPipe[1]);   
   while ((read(pipes->mainPipe[0], &report, sizeof(Report))) != 0) {
      printf("Result from %d, step %d: %.3f\n", 
       report.id, report.step, report.value);
      *(reportCount + report.id) += 1;
   }
   close(pipes->mainPipe[0]);
}

void WaitCells(int *pids, int *fails, int cells) {
   int *temp = fails;
   int status;
   int ndx = 0;

   while (ndx < cells) {
      waitpid(*(pids + ndx), &status, 0);
      if (WIFEXITED(status)) {
         if (WEXITSTATUS(status) != 0) {
            *temp++ = WEXITSTATUS(status);
            *temp++ = ndx;
         }
      }
      ndx++;
   }
   *temp = 0;
}

void CheckErrors(int *fails, Flags *flags, int *reportCount) {
   int ndx = 0;
   int over = 0;
   int under = 0;
   int error = 0;
   int status;
   
   if (*fails != 0) {
      error = 1;
      while (*fails != 0)
         fprintf(stderr, "Error: Child %d exited with %d\n", 
          *fails++, *fails++);
   }

   while (ndx < flags->cells) {
      if (*(reportCount + ndx) > flags->maxStep + 1) {
         error = 1;
         over++;
      }
      if (*(reportCount + ndx) < flags->maxStep + 1) {
         error = 1;
         under++;
      }
      ndx++;
   }

   if (over)
      fprintf(stderr, "Error: %d cells reported too many reports\n", over);
   if (under)
      fprintf(stderr, "Error: %d cells reported too few reports\n", under); 
   if (error)
      exit(EXIT_FAILURE);
}
