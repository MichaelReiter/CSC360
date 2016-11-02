/*
  University of Victoria
  Fall 2016
  CSC 360 Assignment 2
  Michael Reiter
  V00831568
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

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
flow flowList[MAXFLOW];     // parse input in an array of flow
flow* queueList[MAXFLOW];   // store waiting flows while transmission pipe is occupied.
pthread_t thrList[MAXFLOW]; // each thread executes one flow
pthread_mutex_t trans_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t trans_cvar = PTHREAD_COND_INITIALIZER;

/* ---------- Helper functions ---------- */

/*
  param: 
  returns 
*/
void requestPipe(flow* item) {
  lock mutex;

  if transmission pipe available && queue is empty {
    ...do some stuff..
    unlock mutex;
    return ;
  }

  add item in queue, sort the queue according rules

  printf(Waiting...);
  // key point here..
  // wait till pipe to be available and be at the top of the queue
  while (some condition) {
    wait()
  }

  // update queue

  unlock mutex;
}

/*
  Does something
*/
void releasePipe() {
  // I believe you genuis will figure this out! (only about 5 lines)
  // use broadcast to ensure you get the right one
}

/*
  flowItem: 
  Does something
*/
// entry point for each thread created
void* thrFunction(void* flowItem) {
  flow* item = (flow*)flowItem;

  // wait for arrival
  usleep(...)
  printf(Arrive...);

  requestPipe(item) ;
  printf(Start...)

  // sleep for transmission time
  usleep(...)

  releasePipe(item) ;
  printf(Finish..);
}

/* ---------- Main ---------- */

/*
  A multiflow scheduler
*/
int main() {
  // file handling

  FILE* fp = fopen(...) // read fild

  // read number of flows

  int i;
  for (i = 0; i < numFlows; i++) { // for each flow
    fgets(...) // read line by line
    parse line, using strtok() & atoi(), store them in flowList[i] ;
  }

  fclose(fp); // release file descriptor

  for (i = 0; i < numFlows; i++) { // for each flow
    // create a thread for each flow 
    pthread_create(&thrList[i], NULL, thrFunction, (void*)&flowList[i]) ;
  }

  // wait for all threads to terminate
  pthread_join(...)

  // destroy mutex & condition variable

  return 0;
}
