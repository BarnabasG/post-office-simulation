/*Include the queue program header file*/
#include "queue.h"

/*Create a new node (customer) in the QUEUE struct*/
static QUEUE *newNode(int pos, int id, int minPatience, int maxPatience, int minTaskTime, int maxTaskTime)
{
    QUEUE *q = (QUEUE*)malloc(sizeof(QUEUE));

    if (q ==NULL)
    {
        printf("Memory allocation error in malloc");
        fprintf(stderr, "Error number: %d: %s\n", errno, strerror(errno));
        exit(-1);
    }
    q->data.id = id;
    q->data.position = pos;
    q->data.waitTime = 0;
    q->data.patience = get_random_value(minPatience, maxPatience);
    q->data.taskTime = get_random_value(minTaskTime, maxTaskTime);
    strcpy(q->data.status, "Waiting");

    return q;
}

/*Append node (cutomer) to the end of the queue*/
extern void insertEnd (QUEUE **head, int id, int pos, int minPatience, int maxPatience, int minTaskTime, int maxTaskTime, int numSims)
{
    QUEUE *new = newNode(pos, id, minPatience, maxPatience, minTaskTime, maxTaskTime);
    QUEUE *last = *head;

    new->nextNode = NULL;
    if (*head == NULL)
    {
        new->previousNode = NULL;
        *head = new;
        return;
    }
    while (last->nextNode != NULL)
    {
        last = last->nextNode;
    }    
    last->nextNode = new;
    
    new->previousNode = last;

    if (new->data.position == 101)
    {
        if (numSims == 1)
            printf("Customer ID %d was turned away from a full queue\n", new->data.id);
        strcpy(new->data.status, "Unfulfilled");
        new->data.position = 102;
    }

    return;
    
}

/*Free malloced memory*/
extern void freeData (QUEUE *q)
{
    if ( q==NULL )
        return;
    else
    {
       free(q);
       freeData(q->nextNode);
    }
}

/*Display queue information at given time interval*/
extern void printListInfo(QUEUE *q, int interval, int total, FILE *fp) 
{ 

    int serving = get_current_count(q,0,100);
    int in_queue = get_current_queue(q,0);
    int fulfilled = get_current_count(q,0,999);
    int unfulfilled = get_current_count(q,0,102);
    int timed_out = get_current_count(q,0,404);

    printf("========================\n");
    printf("Time Interval: %d\n", interval);
    printf("Total Customers: %d\n", total);
    printf("Customers Being Served: %d\n", serving);
    printf("Customers in Queue: %d\n", in_queue);
    printf("Customers Fulfilled: %d\n", fulfilled);
    printf("Customers Unfulfilled: %d\n", unfulfilled);
    printf("Customers Timed Out: %d\n", timed_out);
    printf("========================\n\n");

    fprintf(fp,"========================\n");
    fprintf(fp,"Time Interval: %d\n", interval);
    fprintf(fp,"Total Customers: %d\n", total);
    fprintf(fp,"Customers Being Served: %d\n", serving);
    fprintf(fp,"Customers in Queue: %d\n", in_queue);
    fprintf(fp,"Customers Fulfilled: %d\n", fulfilled);
    fprintf(fp,"Customers Unfulfilled: %d\n", unfulfilled);
    fprintf(fp,"Customers Timed Out: %d\n", timed_out);
    fprintf(fp,"========================\n\n");

}