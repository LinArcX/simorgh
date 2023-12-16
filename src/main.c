#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

/*
 .q : quit
 .p l: shows content of the current scope.(if it's global scrop --> execute: .p g)
 .w : write the content to current directory not /tmp.

 .p h: shows all the headers included in the current file.
 .p s: show all static variables.
 .p g: show all global variables.
 .p ?: print any variable or evaluate any function without putting it into the file.
 .s <FuncName>/<File>/<global> : switch to either a function name in current file, a file in whole project or global scope.
 .t : trace from the beggining until this function.

 !!.d : debug from current line(without gdb) --> Do you need a debugger??
 */

char lineBuffer[300];
bool isExit = false;
FILE* tmpFile = NULL;
char tmpFileName[50];

void
writeToTmpFile()
{
  fprintf(tmpFile, "%s\n", lineBuffer);

  // Force flushing the data to the file
  if (fflush(tmpFile) != 0) {
    perror("Error flushing file");
  }
}

void
writeToCurrentDirectory()
{

}

void
showCurrentScope()
{

}

int
parseLine()
{
  if(0 == strcmp(lineBuffer, ".q"))
  {
    isExit = true;
    return 0;
  }

  if(0 == strcmp(lineBuffer, ".l"))
  {
    showCurrentScope();
  }

  if(0 == strcmp(lineBuffer, ".w"))
  {
    writeToCurrentDirectory();
  }

  writeToTmpFile();

  return -1;
}

int
generateTempFile()
{
  // Get the current time
  time_t currentTime;
  time(&currentTime);

  // Format the date and time
  struct tm *timeInfo = localtime(&currentTime);
  char timestamp[20];  // Adjust the size based on your needs
  strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeInfo);


  char filename[50];
  // Create the filename based on the timestamp
  snprintf(filename, sizeof(filename), "/tmp/simorgh_%s.c", timestamp);

  if (tmpFileName[0] == '\0') {
    strcpy(tmpFileName, filename); 
    // Open the file for writing
    tmpFile = fopen(filename, "w");

    // Check if the file was opened successfully
    if (tmpFile == NULL) {
      perror("Error opening file");
      return -1;
    } 
  }

  return 0;
}

void
prepareLineBuffer()
{
  memset(lineBuffer, 0, sizeof(lineBuffer));

  printf(">> ");
  fgets(lineBuffer, sizeof(lineBuffer), stdin);

  // Remove the newline character if it exists
  size_t len = strlen(lineBuffer);
  if (len > 0 && lineBuffer[len - 1] == '\n') 
  {
    lineBuffer[len - 1] = '\0';
  }
}

int
main(int argc, char** argv)
{
  memset(tmpFileName, 0, sizeof(tmpFileName));
  while(false == isExit)
  {
    prepareLineBuffer();

    if(-1 == generateTempFile())
      perror("Error creating tmp file");

    if(0 == parseLine())
      continue;
  }

  fclose(tmpFile);
  return 0;
}
