   /* 
   
   Zach Martin | 357-07 | PROJECT 3 | Pipe.c | 5/4/2016

   */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Pipe.h"

void SetupPipes(Pipes *pipes) {
   if (pipe(pipes->mainPipe)) {
      fprintf(stderr, "Error creating pipe\n");
      exit(EXIT_FAILURE);
   }
   if (pipe(pipes->nextPI)) {
      fprintf(stderr, "Error creating pipe\n");
      exit(EXIT_FAILURE);
   }

   if (pipe(pipes->prevPI)) {
      fprintf(stderr, "Error creating pipe\n");
      exit(EXIT_FAILURE);
   }
   if (pipe(pipes->nextPO)) {
      fprintf(stderr, "Error creating pipe\n");
      exit(EXIT_FAILURE);
   }

   if (pipe(pipes->prevPO)) {
      fprintf(stderr, "Error creating pipe\n");
      exit(EXIT_FAILURE);
   }
   
}

void ReversePipes(Pipes *pipes) {
   int temp[2];

   temp[0] = pipes->nextPI[0];
   temp[1] = pipes->nextPI[1];
   pipes->nextPI[0] = pipes->prevPO[0];
   pipes->nextPI[1] = pipes->prevPO[1];
   pipes->prevPO[0] = temp[0];
   pipes->prevPO[1] = temp[1];
   
   temp[0] = pipes->prevPI[0];
   temp[1] = pipes->prevPI[1];
   pipes->prevPI[0] = pipes->nextPO[0];
   pipes->prevPI[1] = pipes->nextPO[1];
   pipes->nextPO[0] = temp[0];
   pipes->nextPO[1] = temp[1];
}

void MovePipes(Pipes *pipes) {
   close(pipes->nextPO[0]);
   close(pipes->nextPO[1]);
   pipe(pipes->nextPO);
   
   close(pipes->nextPI[0]);
   close(pipes->nextPI[1]);
   pipe(pipes->nextPI);
}

void CleanPipes(int fds[MAX_FD]) {
   int ndx = 0;

   while (ndx < MAX_FD) {
      if (fds[ndx] != -1)
         close(fds[ndx]);
      ndx++;
   }
}

void ClosePipes(Pipes *pipes) {
   close(pipes->nextPI[0]);  
   close(pipes->nextPI[1]);
   
   close(pipes->prevPI[0]);
   close(pipes->prevPI[1]);
   
   close(pipes->nextPO[0]);   
   close(pipes->nextPO[1]);
   
   close(pipes->prevPO[0]);  
   close(pipes->prevPO[1]);
}