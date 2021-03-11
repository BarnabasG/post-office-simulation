#ifndef __QUEUE__
#define __QUEUE__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>


/*This structure (struct) holds the information relevant to each individual customer*/
typedef struct info   
{
    int id;
    int waitTime;
    int patience;
    int position;
    int taskTime;
    char status[21];
} INFO;

/*This structure hold the pointers for traversing the doubly linked list,
as well as a reference of type INFO refereing to the structure used to
store cutomer information*/
typedef struct queue
{
    INFO data;
    struct queue *nextNode;
    struct queue *previousNode;
} QUEUE;

/*Function prototypes*/
static QUEUE    *newNode                (int, int, int, int, int, int);
extern void     insertEnd               (QUEUE**, int, int, int, int, int, int, int);
extern void     printListInfo           (QUEUE*, int, int, FILE*);
extern void     freeData                (QUEUE*);

#endif
