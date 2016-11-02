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
  float arrivalTime;
  float transTime;
  int priority;
  int id;
} flow;

/* ---------- Constants and global variables ---------- */

#define MAXFLOW 5
#define MAX_INPUT_SIZE 128
flow flows[MAXFLOW];        // parse input in an array of flow
flow* queues[MAXFLOW];      // stores waiting flows while transmission pipe is occupied
pthread_t threads[MAXFLOW]; // each thread executes one flow
pthread_mutex_t trans_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t trans_convar = PTHREAD_COND_INITIALIZER;

/* ---------- Helper functions ---------- */

/*
  filePath: the path of the file to read
  fileContents: an array of strings to which to write the read file contents
  reads the file at filePath into the string array fileContents
*/
void readFlows(char* filePath, char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE]) {
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
  flowItem: 
  Entry point for each thread created. Handles flow scheduling
*/
// void* threadFunction(void* flowItem) {
//   flow* item = (flow*)flowItem;

//   // wait for arrival
//   usleep(...);

//   // printf(Arrive...);
//   printf("Flow %2d arrives: arrival time (%.2f), transmission time (%.1f), priority (%2d). \n", flowNum, arrivalTime, transmissionDuration, priority);

//   requestPipe(item);
//   // printf(Start...)
//   printf("Flow %2d starts its transmission at time %.2f. \n", flowNum, transmissionStartTime);

//   // sleep for transmission time
//   usleep(...);

//   releasePipe(item);
//   // printf(Finish..);
//   printf("Flow %2d finishes its transmission at time %d. \n", flowNum, transmissionFinishTime);
// }

/* ---------- Main ---------- */

/*
  A multiflow scheduler
*/
int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: MFS <flows text file>\n");
    return 1;
  }

  char fileContents[MAX_INPUT_SIZE][MAX_INPUT_SIZE];
  readFlows(argv[1], fileContents);

  int i;
  for (i = 0; i < 11; i++) {
    printf("%s", fileContents[i]);
  }

  // for (i = 0; i < numFlows; i++) {
  //   // create a thread for each flow
  //   pthread_create(&threads[i], NULL, threadFunction, (void*)&flows[i]);
  // }

  // // wait for all threads to terminate
  // pthread_join(...);

  // // destroy mutex & condition variable

  return 0;
}
