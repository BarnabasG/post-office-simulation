#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/*Include the main program header file*/
#include "simQ.h"

int main(int argc, char **argv)
{

    int numSims;
    char fileIn[21], fileOut[21];
    if (argc > 3)
    {
        strcpy(fileIn, argv[1]);
        numSims = atoi(argv[2]);
        strcpy(fileOut, argv[3]);
    }
    else        /*Not enough arguments were included*/
    {
        printf("Enter an input filename, loop count, output filename\n");
    }
    
    int maxQueueLength,servicePoints,closingTime,maxNewCustomers,minNewCustomers,\
        maxPatience,minPatience,maxTaskTime,minTaskTime;

    getStartData(   fileIn, &maxQueueLength, &servicePoints, &closingTime, &maxNewCustomers, &minNewCustomers,\
                    &maxPatience, &minPatience, &maxTaskTime, &minTaskTime);

    /*Print data read from file*/
    printf("\nMax queue: %d\nService points: %d\nClose time: %d\nMax new customers: %d\nMin new customers: %d\n",\
            maxQueueLength,servicePoints,closingTime,maxNewCustomers,minNewCustomers);
    printf("Max patience: %d\nMin patience: %d\nMax task time: %d\nMin task time: %d\n",\
            maxPatience,minPatience,maxTaskTime,minTaskTime);

    srand((unsigned) time(NULL));

    int i;
    static int id = 0;    
    int fulfilled=0, unfulfilled=0, timed_out=0, waiting_time=0, clear_time=0;

    /*Open file for write access to write output (for numSims=1)*/
    FILE *fp;
    if ( (fp = fopen(fileOut, "w")) == NULL )
    {
        printf("Cannot open file\n");
        fprintf(stderr, "Error number: %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    /*Loop running of the simulation the given number of times*/
    for (i=0;i<numSims;i++)
        /*Pass some variables by reference so they can be edited and the value kept*/
        runSimulation(  id, maxQueueLength, servicePoints, closingTime, maxNewCustomers, minNewCustomers,\
                        maxPatience, minPatience, maxTaskTime, minTaskTime, &fulfilled, &unfulfilled, &timed_out,\
                        &waiting_time, &clear_time, numSims, fp);
        id = 0;

    /*Close the file*/
    fclose(fp);

    /*The information to be writen to the output file is a summary of averages if the number
      of simulations is more than 1*/
    if (numSims != 1)
    {
        /*Calculate averages with total/simulation count*/
        fulfilled /= numSims;
        unfulfilled /= numSims;
        timed_out /= numSims;
        waiting_time /= numSims;
        clear_time /= numSims;
        printf("\n---Average results across %d simulations---\n", numSims);
        printf("Fulfilled customers: %d\n", fulfilled);
        printf("Unulfilled customers: %d\n", unfulfilled);
        printf("Timed-out customers: %d\n", timed_out);
        printf("Average waiting time for fulfilled customers: %d\n", waiting_time);
        printf("Average time to clear customers after closing: %d\n", clear_time);

        writeEndData(   fileOut, &maxQueueLength, &servicePoints, &closingTime, &maxNewCustomers, &minNewCustomers,\
                        &maxPatience, &minPatience, &maxTaskTime, &minTaskTime, fulfilled, unfulfilled, timed_out,\
                        waiting_time, clear_time, numSims);
    }

    return 0;
}

void getStartData(  char fileIn[], int* maxQueueLength, int* servicePoints, int* closingTime, int* maxNewCustomers,\
                    int* minNewCustomers, int* maxPatience, int* minPatience, int* maxTaskTime, int* minTaskTime)
{
    /*Open file and error trap*/
    FILE *fp;
    if ( (fp = fopen(fileIn, "r")) == NULL )
    {
        printf("Cannot open file\n");
        fprintf(stderr, "Error number: %d: %s\n", errno, strerror(errno));
        exit(1);
    }

    /*File procesing*/
    fscanf(fp,  "maxQueueLength=%d\nservicePoints=%d\nclosingTime=%d\n\
                maxNewCustomers=%d\nminNewCustomers=%d\nmaxPatience=%d\n\
                minPatience=%d\nmaxTaskTime=%d\nminTaskTime=%d",\
                maxQueueLength, servicePoints, closingTime, maxNewCustomers,minNewCustomers,\
                maxPatience,minPatience,maxTaskTime,minTaskTime);

    fclose(fp);
    return;
}

void writeEndData(  char fileOut[], int* maxQueueLength, int* servicePoints, int* closingTime, int* maxNewCustomers,\
                    int* minNewCustomers, int* maxPatience, int* minPatience, int* maxTaskTime, int* minTaskTime,\
                    int fulfilled, int unfulfilled, int timed_out, int waiting_time, int clear_time, int numSims)
{
    /*Open file and error trap*/
    FILE *fp;
    if ( (fp = fopen(fileOut, "w")) == NULL )
    {
        printf("Cannot open file\n");
        fprintf(stderr, "Error number: %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    /*File procesing*/
    fprintf(fp, "\nMax queue: %d\nService points: %d\nClose time: %d\nMax new customers: %d\nMin new customers: %d\n",\
            *maxQueueLength,*servicePoints,*closingTime,*maxNewCustomers,*minNewCustomers);
    fprintf(fp, "Max patience: %d\nMin patience: %d\nMax task time: %d\nMin task time: %d\n",\
            *maxPatience,*minPatience,*maxTaskTime,*minTaskTime);

    /*Write summary to file*/
    fprintf(fp, "\n---Average results across %d simulations---\n", numSims);
    fprintf(fp, "Fulfilled customers: %d\n", fulfilled);
    fprintf(fp, "Unulfilled customers: %d\n", unfulfilled);
    fprintf(fp, "Timed-out customers: %d\n", timed_out);
    fprintf(fp, "Average waiting time for fulfilled customers: %d\n", waiting_time);
    fprintf(fp, "Average time to clear customers after closing: %d\n", clear_time);

    fclose(fp);
    return;
}

int get_random_value(int min, int max)
{
    return (min+rand()%max);
}

/*Get how many customers are waiting in the queue right now*/
int get_current_queue(QUEUE *q, int count)
{
    if ( q==NULL )
    {
        return count;
    }
    else
    {
        if (q->data.position < 100)
        {
            count++;
        }
    }
    get_current_queue(q->nextNode, count);
}

/*Get count of fulfilled/unfulfilled/timed-out/being served depending on input position*/
int get_current_count(QUEUE *q, int count, int pos)
{
    if ( q==NULL )
    {
        return count;
    }
    else
    {
        if (q->data.position == pos)
        {
            count++;
        }
    }
    get_current_count(q->nextNode, count, pos);
}

/*Get the average wait time of fulfilled customers*/
int get_avr_wait(QUEUE *q, int count, int customers)
{
    if ( q==NULL )
    {
        return (count/customers);
    }
    else
    {
        if (q->data.position == 999)
        {
            count += q->data.waitTime;
        }
    }
    get_avr_wait(q->nextNode, count, ++customers);
}

/*Iterate through one time interval*/
int iterate(QUEUE *q, int stations, int numSims, FILE *fp)
{
    if ( q==NULL )
    {
        return stations;
    }
    else
    {
        if (q->data.position == 100)
        {
            q->data.taskTime--;
            if (q->data.taskTime == 0)
            {
                strcpy(q->data.status, "Fulfilled");
                q->data.position = 999;
                stations++;
            }
        }

        if (stations > 0 && q->data.position < 100)
        {
            q->data.position = 100;
            strcpy(q->data.status, "Being Served");
            stations--;
            moveForward(q);
        }

        if (q->data.position < 100)
        {
            q->data.waitTime++;
            if (q->data.waitTime >= q->data.patience)
            {
                if (numSims == 1)
                {
                    printf("Customer ID %d in the queue got bored and left\n", q->data.id);
                    fprintf(fp,"Customer ID %d in the queue got bored and left\n", q->data.id);
                }
                strcpy(q->data.status, "Timed-out");
                q->data.position = 404;
                moveForward(q);
            }
        }
        iterate(q->nextNode, stations, numSims, fp);
    }
}

/*Move people forward in the queue when spaces open*/
void moveForward(QUEUE *q)
{
    if ( q==NULL )
    {
        return;
    }
    else
    {
        if (q->data.position < 100)
        {
            q->data.position--;
        }
        moveForward(q->nextNode);
    }
     
}

/*Run the processes of an individual simulation*/
void runSimulation( int id, int max_queue, int service_points, int closingTime, int maxNewCustomers, int minNewCustomers,\
                    int maxPatience, int minPatience, int maxTaskTime, int minTaskTime, int* fulfilled, int* unfulfilled,\
                    int* timed_out, int* waiting_time, int* clear_time, int numSims, FILE *fp)
{
    int i = 1;
    QUEUE *head = NULL;
    int new_arrival_count = get_random_value(minNewCustomers, maxNewCustomers);

    while (id < new_arrival_count)
    {
        insertEnd(&head, ++id, i++, minPatience, maxPatience, minTaskTime, maxTaskTime, numSims);
    }

    if (numSims == 1)
        printListInfo(head, 0, id, fp);

    int j, k;
    int current_queue = get_current_queue(head, 0);
    int serving = get_current_count(head,0,100);
    int total_intervals = closingTime;
    for (j=1; (j<=total_intervals)||(current_queue>0)||(serving>0); j++)
    {
        service_points = iterate(head, service_points, numSims, fp);
        current_queue = get_current_queue(head, 0);
        serving = get_current_count(head,0,100);
        
        if (j<=total_intervals)
        {
            new_arrival_count = get_random_value(minNewCustomers, maxNewCustomers);
            for (k=0; k<new_arrival_count; k++)
            {
                int pos;
                if (current_queue == max_queue) /*Queue is full*/
                    pos = 101;
                else                            /*Queue is not full*/
                {
                    pos  = ++current_queue;
                }
                insertEnd(&head, ++id, pos, minPatience, maxPatience, minTaskTime, maxTaskTime, numSims);
            }
        }
        else if (j == total_intervals+1 && numSims == 1)        /*Closing time reached*/
        {
            printf("\n##### Post office closing!\n##### Remaining customers are allowed to finish\n");
            fprintf(fp, "\n##### Post office closing!\n##### Remaining customers are allowed to finish\n");
        }
        if (numSims == 1)
            printListInfo(head, j, id, fp);
    }

    int this_clear_time = j - total_intervals - 1;
    if (numSims == 1)
    {
        printf("Time from closing to clear customers: %d\n", this_clear_time);
        fprintf(fp,"Time from closing to clear customers: %d\n", this_clear_time);
    }
    else
    {
        *clear_time += this_clear_time;
        *fulfilled += get_current_count(head,0,999);
        *unfulfilled += get_current_count(head,0,102);
        *timed_out += get_current_count(head,0,404);
        *waiting_time += get_avr_wait(head,0,0);
    }
    
    /*Free malloced memory*/
    freeData(head);
    
}