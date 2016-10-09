   /* 
   
   Zach Martin | 357-07 | PROJECT 3 | CreateArgv.c | 5/2/2016

   */
   
#include <stdio.h>
#include <stdlib.h>
#include "Report.h"
#include "CreateArgv.h"

void NewArg(char **pArg) {
   if (!(*pArg = calloc(1, ARG_BUF))) {
      fprintf(stderr, "Error allocating memory\n");
      (EXIT_FAILURE);
   }
}
   
void AddSD(char **cellArgv, Flags *flags, int id, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   int ndx;
   
   temp++;
   NewArg(&arg);
   *arg = 'S';
   sprintf(arg + 1, "%d", flags->maxStep);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'D';
   sprintf(arg + 1, "%d", id);
   *temp++ = arg;  
   *temp = NULL;
   
   for (ndx = 0; ndx < MAX_FD; ndx++) {
      fds[ndx] = ndx;
   }
}


void AddLeft(char **cellArgv, Flags *flags, int mainPipe, 
 int nextPO, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   
   fds[0] = fds[1] = fds[2] = fds[mainPipe] = -1;
   while (*temp)
      temp++;

   NewArg(&arg);
   *arg = 'V';
   if (flags->left != -1) 
      sprintf(arg + 1, "%f", flags->left);
   else
      sprintf(arg + 1, "%f", 0.0);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", mainPipe);
   *temp++ = arg;
   
   if (flags->cells > 2) {
      NewArg(&arg);
      *arg = 'O';
      sprintf(arg + 1, "%d", nextPO);
      fds[nextPO] = -1;
      *temp++ = arg;
   }


   *temp = NULL;
}

void AddRight(char **cellArgv, Flags *flags, int mainPipe, 
 int prevPO, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   
   fds[0] = fds[1] = fds[2] = -1;
   fds[mainPipe] = -1;
   while (*temp)
      temp++;

   NewArg(&arg);
   *arg = 'V';
   if (flags->right != -1) 
      sprintf(arg + 1, "%f", flags->right);
   else
      sprintf(arg + 1, "%f", 0.0);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", mainPipe);
   *temp++ = arg;
   
   if (flags->cells > 2) {
      NewArg(&arg);
      *arg = 'O';
      sprintf(arg + 1, "%d", prevPO);
      fds[prevPO] = -1;
      *temp++ = arg;
   }

   *temp = NULL;
}

void AddFirst(char **cellArgv, Flags *flags, int mainPipe, int prevPI,
 int nextPI, int nextPO, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   
   fds[0] = fds[1] = fds[2] = fds[mainPipe] = -1;
   while (*temp)
      temp++;
  
   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", mainPipe);
   *temp++ = arg;
   if (flags->cells > 2) {
      fds[prevPI] = fds[nextPI] = -1; 
      NewArg(&arg);
      *arg = 'I';
      sprintf(arg + 1, "%d", prevPI);
      *temp++ = arg;

      NewArg(&arg);
      *arg = 'I';
      sprintf(arg + 1, "%d", nextPI);
      *temp++ = arg;

      if (flags->cells > ONE_INTERIOR) {
         fds[nextPO] = -1;
         NewArg(&arg);
         *arg = 'O';
         sprintf(arg + 1, "%d", nextPO);
         *temp++ = arg;
      }
   }

   *temp = NULL;
}

void AddLast(char **cellArgv, Flags *flags, int mainPipe, int prevPI,
 int nextPI, int prevPO, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   
   fds[0] = fds[1] = fds[2] = -1;
   fds[mainPipe] = fds[prevPI] = fds[nextPI] = fds[prevPO] = -1;
   while (*temp)
      temp++;

   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", mainPipe);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'I';
   sprintf(arg + 1, "%d", prevPI);
   *temp++ = arg;

   NewArg(&arg);
   *arg = 'I';
   sprintf(arg + 1, "%d", nextPI);
   *temp++ = arg;

   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", prevPO);
   *temp++ = arg;
   
   *temp = NULL;
}



void AddInterior(char **cellArgv, Flags *flags, int mainPipe, int prevPO,
 int nextPO, int prevPI, int nextPI, int fds[MAX_FD]) {
   char **temp = cellArgv;
   char *arg;
   
   fds[0] = fds[1] = fds[2] = -1;
   fds[mainPipe] = fds[prevPO] = fds[nextPO] = -1;
   fds[prevPI] = fds[nextPI] = -1;
   while (*temp)
      temp++;

   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", mainPipe);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'I';
   sprintf(arg + 1, "%d", prevPI);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'I';
   sprintf(arg + 1, "%d", nextPI);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", prevPO);
   *temp++ = arg;
   
   NewArg(&arg);
   *arg = 'O';
   sprintf(arg + 1, "%d", nextPO);
   *temp++ = arg;
   
   *temp = NULL;
}

