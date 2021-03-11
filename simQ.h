#ifndef __SIMQ__
#define __SIMQ__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
/*Include the queue program header file*/
#include "queue.h"

/*Function prototypes*/
void            getStartData            (char[], int*, int*, int*, int*, int*, int*, int*, int*, int*);
void            writeEndData            (char[], int*, int*, int*, int*, int*, int*, int*, int*, int*,\
                                        int, int, int, int, int, int);
void            runSimulation           (int, int, int, int, int, int, int, int, int, int, int*, int*,\
                                        int*, int*, int*, int, FILE*);
int             get_random_value        (int, int);
int             get_current_queue       (QUEUE*, int);
int             get_current_serving     (QUEUE*, int);
int             get_current_fulfilled   (QUEUE*, int);
int             get_current_unfulfilled (QUEUE*, int);
int             get_current_timed_out   (QUEUE*, int);
int             iterate                 (QUEUE*, int, int, FILE*);
void            moveForward             (QUEUE*);

#endif
