/*
  University of Victoria
  Fall 2016
  CSC 360 Assignment 2
  Michael Reiter
  V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

/* ---------- Typedefs ---------- */

typedef struct flow {
  int id;
  float arrivalTime;
  float transmissionTime;
  int priority;
} flow;

/* ---------- Constants and global variables ---------- */

#define TRUE 1
#define FALSE 0
#define MAXFLOW 256
#define MAX_INPUT_SIZE 1024
#define DECISECONDS_TO_MICROSECONDS 100000
flow flows[MAXFLOW];        // The input is parsed into an array of flows
flow* queue[MAXFLOW];       // Stores waiting flows while transmission pipe is occupied
pthread_t threads[MAXFLOW]; // Each thread executes one flow
pthread_mutex_t mutex;
pthread_cond_t convar;
flow* transmittingFlow;
struct timeval start;
int queueLength = 0;
int isTransmitting = FALSE;

/* ---------- Helper functions ---------- */

/*
  filePath: the path of the file to read
  fileContents: an array of strings to which to write the read file contents
  Reads the file at filePath into the string array fileContents
  Returns 0 if successful, 1 otherwise
*/
int readFlowsFile(char* filePath, char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE]) {
  FILE *fp = fopen(filePath, "r");
  if (fp != NULL) {
    int i = 0;
    while(fgets(fileContents[i], MAX_INPUT_SIZE, fp)) {
      i++;
    }
    fclose(fp);
    return 0;
  } else {
    return 1;
  }
}

/*
  f1: the first flow to compare
  f2: the second flow to compare

  The one with the highest priority start its transmission first.

  If there is a tie at the highest priority, the one whose arrival time
  is the earliest start its transmission first.

  If there is still a tie, the one that has the smallest transmission
  time starts its transmission first.

  If there is still a tie, the one that appears first in the input file
  starts its transmission first.

  Returns 1 if f1 should come before f2
         -1 if f2 should come before f1
          0 if there was an error
*/
int compareFlows(flow* f1, flow* f2) {
  if (f1->priority > f2->priority) {
    return 1;
  } else if (f1->priority < f2->priority) {
    return -1;
  }

  else if (f1->arrivalTime > f2->arrivalTime) {
    return 1;
  } else if (f1->arrivalTime < f2->arrivalTime) {
    return -1;
  }

  else if (f1->transmissionTime > f2->transmissionTime) {
    return 1;
  } else if (f1->transmissionTime < f2->transmissionTime) {
    return -1;
  }

  else if (f1->id > f2->id) {
    return 1;
  } else if (f1->id < f2->id) {
    return -1;
  }

  else {
    // Should never get here
    printf("Error: failed to sort\n");
    return 0;
  }
}

/*
  Sorts queue in place using Bubblesort
  (sometimes it's worth sacrificing running time to save programmer time)
*/
void sortQueue() {
  int i;
  int j;
  for (i = transmitting; i < queueLength; i++) {
    for (j = transmitting; j < queueLength-1; j++) {
      if (compareFlows(queue[j], queue[j+1]) == 1) {
        flow* temp = queue[j+1];
        queue[j+1] = queue[j];
        queue[j] = temp;
      }
    }
  }
}

/*
  f: a flow* to insert
  Inserts f into queue
*/
void insertIntoQueue(flow* f) {
  queue[queueLength] = f;
  queueLength += 1;
}

/*
  Removes the least recently inserted flow from the queue
*/
void removeFromQueue() {
  int i;
  for (i = 0; i < queueLength-1; i++) {
    queue[i] = queue[i+1];
  }
  queueLength -= 1;
}

/*
  f: a flow
  Acquires the transmission pipe
*/
void requestPipe(flow* f) {
  pthread_mutex_lock(&mutex);

  if (isTransmitting == FALSE && queueLength == 0) {
    transmittingFlow = f;
    isTransmitting = TRUE;
    insertIntoQueue(f);
  } else if (isTransmitting == FALSE && queueLength > 0) {
    insertIntoQueue(f);
    sortQueue();
    transmittingFlow = queue[0];
    isTransmitting = TRUE;
  } else {
    insertIntoQueue(f);
    sortQueue();
  }

  pthread_mutex_unlock(&mutex);
}

/*
  f: a flow
  Releases the transmission pipe
*/
void releasePipe(flow* f) {
  pthread_mutex_lock(&mutex);

  removeFromQueue();

  if (queueLength > 0) {
    transmittingFlow = queue[0];
  } else {
    transmittingFlow = NULL;
    isTransmitting = FALSE;
  }

  pthread_mutex_unlock(&mutex);
}

/*
  Returns the time difference in microseconds between now and start
*/
float getTimeDiff() {
  struct timeval now;
  gettimeofday(&now, NULL);
  long nowMicroseconds = (now.tv_sec * 10 * DECISECONDS_TO_MICROSECONDS) + now.tv_usec;
  long startMicroseconds = (start.tv_sec * 10 * DECISECONDS_TO_MICROSECONDS) + start.tv_usec;
  return (float)(nowMicroseconds - startMicroseconds) / (10 * DECISECONDS_TO_MICROSECONDS);
}

/*
  flowItem: a flow struct to schedule
  Entry point for each thread created. Handles flow scheduling.
*/
void* threadFunction(void* flowItem) {
  flow* f = (flow*)flowItem;

  // Wait for arrival (converted from deciseconds to microseconds)
  usleep(f->arrivalTime * DECISECONDS_TO_MICROSECONDS);

  printf("Flow %2d arrives: arrival time (%.2f), transmission time (%.1f), priority (%2d). \n", f->id, getTimeDiff(), f->transmissionTime * 0.1, f->priority);

  requestPipe(f);

  if (f->id != transmittingFlow->id) {
    printf("Flow %2d waits for the finish of flow %2d. \n", f->id, transmittingFlow->id);
  }
  while (transmittingFlow->id != f->id);

  printf("Flow %2d starts its transmission at time %.2f. \n", f->id, getTimeDiff());

  // Sleep for transmission time (converted from deciseconds to microseconds)
  usleep(f->transmissionTime * DECISECONDS_TO_MICROSECONDS);

  printf("Flow %2d finishes its transmission at time %.1f. \n", f->id, getTimeDiff());

  releasePipe(f);

  pthread_exit(NULL);
}

/*
  s: a string
  Replaces any ':' in s with ','
*/
void replaceColon(char s[]) {
  int i = 0;
  while (s[i] != '\0') {
    if (s[i] == ':') {
      s[i] = ',';
    }
    i++;
  }
}

/*
  fileContents: an array of strings containing user input
  numFlows: the number of flows
  Parses input into flow objects
*/
void parseFlows(char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE], int numFlows) {
  int i;
  for (i = 1; i <= numFlows; i++) {
    replaceColon(fileContents[i]);

    int j = 0;
    int flowVector[4];
    char* token = strtok(fileContents[i], ",");
    while (token != NULL) {
      flowVector[j] = atoi(token);
      token = strtok(NULL, ",");
      j++;
    }

    flow f = {
      flowVector[0],  // id
      flowVector[1],  // arrivalTime
      flowVector[2],  // transmissionTime
      flowVector[3]   // priority
    };

    flows[i-1] = f;
  }
}

/* ---------- Main ---------- */

/*
  A router simulator that concurrently schedules the transmission of flows
*/
int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: MFS <flows text file>\n");
    return 1;
  }

  // Read flows text file
  char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE];
  if (readFlowsFile(argv[1], fileContents) != 0) {
    printf("Error: Failed to read flows file\n");
    return 1;
  }

  // Parse input into array of flow*
  int numFlows = atoi(fileContents[0]);
  parseFlows(fileContents, numFlows);

  // Initialize mutex and conditional variable
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&convar, NULL);

  // Create threads in a joinable state
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  gettimeofday(&start, NULL);

  // Create a thread for each flow
  int i;
  for (i = 0; i < numFlows; i++) {
    if (pthread_create(&threads[i], &attr, threadFunction, (void*)&flows[i]) != 0){
      printf("Error: failed to create pthread.\n");
      return 1;
    }
  }

  // Wait for all threads to terminate
  for (i = 0; i < numFlows; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      printf("Error: failed to join pthread.\n");
      return 1;
    }
  }

  // Destroy mutex and conditional variable
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&convar);
  pthread_exit(NULL);

  return 0;
}
