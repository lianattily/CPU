#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "list.h"
#include "task.h"
#include "schedulers.h"

struct node *head=NULL;
int cnt = 0;

void add(char *name, int priority, int burst){

    Task *task = malloc(sizeof(Task));//={ .name=name, .priority=priority, .burst=burst};
    task->name=name;
    task->priority=priority;
    task->burst=burst;
    task->tid= cnt; //arrival time
    cnt++;
    insert(&head, task);
   
}


void schedule()
{
    int counter=0, i=1,j=0;
    int waitingtime[100], tat[100], bt[100];
    struct node *temp = malloc(sizeof(struct node));
    temp = head;
    Task *task =malloc(sizeof(Task));
    struct node *prev = NULL;
    struct node *curr = head;
    /********************************************************************/
    /*                             sort FIFO                            */
    /********************************************************************/
    while(curr!=NULL){
        temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = temp;
    }
    waitingtime[0]=0;
    temp = prev;
    while (temp != NULL) {
        counter++;
        task = temp->task;
        run(task, task->burst);
        temp = temp->next;
        bt[j]=task->burst;
        
        j++;
    }
    /********************************************************************/
    /*                 calculate WT, TAT, RT                            */
    /********************************************************************/
    for(i=0;i<counter;i++){
        waitingtime[i]=0;
        for(j=0;j<i;j++)
            waitingtime[i]+=bt[j];
    }
    float avwt=0.0,avtat=0.0;
    for(i=0;i<counter;i++)
    {
        tat[i]=bt[i]+waitingtime[i];
        avwt+=waitingtime[i];
        avtat+=tat[i];
    }
    avwt/=(1.0*cnt);
    avtat/=(1.0*cnt);
    printf("\nAverage Turnaround Time:%0.3f\n",avtat);
    printf("Average Waiting Time:%0.3f\n",avwt);
    printf("Average response time = %0.3f\n",avwt);   //
}
