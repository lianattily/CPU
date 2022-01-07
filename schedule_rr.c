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

    Task *task = malloc(sizeof(Task));
    task->name=name;
    task->priority=priority;
    task->burst=burst;
    task->tid= cnt;
    cnt++;
    insert(&head, task);
   
}
void REAR(struct node **head, Task *task){
    struct node* tailnode = malloc(sizeof(struct node));
    tailnode->task = task;
    tailnode->next = NULL;

    
    if ((*head) == NULL){
        *head = tailnode;
    }
    else{
        struct node *tempNode = *head;
        while (tempNode->next != NULL)
            tempNode = tempNode->next;
        tempNode->next = tailnode;
    }
}

void schedule()
{
    int count=0, i=1, sq=0;
    int waitingtime[100], tat[100], bt[100], rt[100], cpuservice[100]={0};
    struct node *temp = malloc(sizeof(struct node));
    temp = head;
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
    /********************************************************************/
    /*                initialize burst times                            */
    /********************************************************************/
    waitingtime[0]=0;
    rt[0]=0;
    int tempQ;
    temp = prev;
    for(i=0;i<cnt;i++){
        bt[i]=temp->task->burst;
        temp=temp->next;
    }
    head=prev;
    temp=prev;
    Task *cpuTask;
    while(head!=NULL){
        count=0;
        cpuTask=temp->task;
        if (cpuTask->burst > QUANTUM) {
                    tempQ=QUANTUM;
                    run(cpuTask, QUANTUM);
                    cpuTask->burst -= QUANTUM;
                    REAR(&head, cpuTask);
                }
                else {
                    tempQ=cpuTask->burst;
                    run(cpuTask, cpuTask->burst);
                    cpuTask->burst = 0;
                    
                }
                delete(&head, cpuTask);
            if(cpuservice[temp->task->tid]==0) {
                    rt[temp->task->tid]=sq;
                    cpuservice[temp->task->tid]=1;
        }
            sq+=tempQ; 
            tat[cpuTask->tid]=sq;
            temp=temp->next;
    }
    /********************************************************************/
    /*                        calculate averages                        */
    /********************************************************************/
    float avwt=0.0,avtat=0.0, respT=0.0;
    for(i=0;i<cnt;i++){ 
        waitingtime[i] = tat[i]-bt[i]; 
        avwt+=waitingtime[i]; 
        avtat+=tat[i];
        respT+=rt[i];

    }
    avwt/=(1.0*cnt);
    avtat/=(1.0*cnt);
    respT/=(1.0*cnt);
    printf("\n\nAverage Waiting Time:%0.3f",avwt); //107.5
    printf("\nAverage Turnaround Time:%0.3f\n",avtat); //128.75
    printf("Average response time = %0.3f\n",respT);   //35.000
}
