
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "list.h"
#include "task.h"
#include "schedulers.h"

struct node *head=NULL;
void add(char *name, int priority, int burst)
{
    Task *task = malloc(sizeof(Task));//={ .name=name, .priority=priority, .burst=burst};
    task->name=name;
    task->priority=priority;
    task->burst=burst;
    insert(&head, task);
   
}

void swap(struct node* node1, struct node* node2){
    Task *swap = node1->task;
    node1->task = node2->task;
    node2->task = swap;
}

void sort(struct node* start){
    struct node *temp ;
    struct node *temp2=NULL;
    int swapped ;
    do{
        swapped=0;
        temp = start;
        while (temp->next != temp2) 
        { 
            if (temp->task->burst > temp->next->task->burst) 
            {  
                swap(temp, temp->next); 
                swapped = 1; 
            } 
            temp = temp->next;
        } 
        temp2=temp;
    } while(swapped);
}

void schedule()
{
    int counter=0, i=1,j=0, total=0;
    float avwt=0.0,avtat=0.0;
    int waitingtime[100], tat[100], bt[100];
    struct node *temp ;
    /********************************************************************/
    /*                             sort SJF                             */
    /********************************************************************/
    sort(head);
    temp = head;
    while (temp != NULL){
        counter++;
        run(temp->task, temp->task->burst);
        bt[j]=temp->task->burst;
        j++;
        temp = temp->next;
    }
    /********************************************************************/
    /*                 calculate WT, TAT, RT                            */
    /********************************************************************/
    waitingtime[0]=0;
    for(i=1;i<counter;i++)
    {
        waitingtime[i]=0;
        for(j=0;j<i;j++)
            waitingtime[i]+=bt[j];
  
        total+=waitingtime[i];
    }
  
    avwt=(float)total/counter;      
    total=0;
  
    for(i=0;i<counter;i++)
    {
        tat[i]=bt[i]+waitingtime[i];   
        total+=tat[i];
    }
    
    avtat=(float)total/counter;  
    printf("\nAverage Turnaround Time=%0.3f\n",avtat);  
    printf("Average Waiting Time=%0.3f\n",avwt);
    printf("Average response time = %0.3f\n",avwt);

}
