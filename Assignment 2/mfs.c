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
#include <pthread.h>

/*
  time calculation may be a nightware! 
  Beware of float, int, unsigned int conversion.
  you could use gettimeofday(...) to get down to microseconds!
*/

/* ---------- Typedefs ---------- */

typedef struct flow {
  int id;
  float arrivalTime;
  float transmissionTime;
  int priority;
} flow;

/* ---------- Constants and global variables ---------- */

#define MAXFLOW 16
#define MAX_INPUT_SIZE 1024
flow flows[MAXFLOW];        // parse input in an array of flow
flow* queues[MAXFLOW];      // stores waiting flows while transmission pipe is occupied
pthread_t threads[MAXFLOW]; // each thread executes one flow
pthread_mutex_t mutex;
pthread_cond_t convar;

/* ---------- Helper functions ---------- */

/*
  filePath: the path of the file to read
  fileContents: an array of strings to which to write the read file contents
  reads the file at filePath into the string array fileContents
*/
void readFlowsFile(char* filePath, char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE]) {
  FILE *fp = fopen(filePath, "r");
  if (fp != NULL) {
    int i = 0;
    while(fgets(fileContents[i], MAX_INPUT_SIZE, fp)) {
      i++;
    }
    fclose(fp);
  } else {
    printf("Error: could not read file\n");
  }
}

/*
  item: 
  returns 
*/
// void requestPipe(flow* item) {
//   lock mutex;

//   if transmission pipe available && queue is empty {
//     ...do some stuff..
//     unlock mutex;
//     return ;
//   }

//   // add item in queue, sort the queue according rules

//   // printf(Waiting...);
//   printf("Flow %2d waits for the finish of flow %2d. \n");
//   // key point here..
//   // wait till pipe to be available and be at the top of the queue
//   while (some condition) {
//     wait();
//   }

//   // update queue

//   // unlock mutex;
// }

/*
  Does something
*/
void releasePipe() {
  // use broadcast to ensure you get the right one
  // only about 5 lines
}

/*
  flowItem: a flow struct to schedule
  Entry point for each thread created. Handles flow scheduling.
*/
void* threadFunction(void* flowItem) {
  flow* f = (flow*)flowItem;

  printf("%d\n", f->id);
  printf("%f\n", f->arrivalTime);
  printf("%f\n", f->transmissionTime);
  printf("%d\n\n", f->priority);

  // // wait for arrival
  // usleep(...);

  // // printf(Arrive...);
  // printf("Flow %2d arrives: arrival time (%.2f), transmission time (%.1f), priority (%2d). \n", flowNum, arrivalTime, transmissionDuration, priority);

  // requestPipe(item);
  // // printf(Start...)
  // printf("Flow %2d starts its transmission at time %.2f. \n", flowNum, transmissionStartTime);

  // // sleep for transmission time
  // usleep(...);

  // releasePipe(item);
  // // printf(Finish..);
  // printf("Flow %2d finishes its transmission at time %d. \n", flowNum, transmissionFinishTime);
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

    flow* f = (flow*)malloc(sizeof(flow));
    f->id = flowVector[0];
    f->arrivalTime = flowVector[1];
    f->transmissionTime = flowVector[2];
    f->priority = flowVector[3];

    flows[i-1] = *f; 
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

  // Parse flows text file into array of *flow
  char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE];
  readFlowsFile(argv[1], fileContents);
  int numFlows = atoi(fileContents[0]);
  parseFlows(fileContents, numFlows);

  // Initialize mutex and conditional variable
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&convar, NULL);

  // Create threads in a joinable state
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // Create a thread for each flow
  int i;
  for (i = 0; i < numFlows; i++) {
    pthread_create(&threads[i], &attr, threadFunction, (void*)&flows[i]);
  }

  // Wait for all threads to terminate
  for (i = 0; i < numFlows; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destroy mutex and conditional variable
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&convar);
  pthread_exit(NULL);

  return 0;
}
