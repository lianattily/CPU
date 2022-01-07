
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "list.h"
#include "task.h"
#include "schedulers.h"

int cnt=0;
int pc=0;
struct node *head=NULL;
struct node *tmp;

void add(char *name, int priority, int burst)
{
    Task *task = malloc(sizeof(Task));
    task->name=name;
    task->priority=priority;
    task->burst=burst;
    task->tid= cnt;
    cnt++;
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
            if (temp->task->priority < temp->next->task->priority) 
            {  
                swap(temp, temp->next); 
                swapped = 1; 
            } 
            temp = temp->next;
        } 
        temp2=temp;
    } while(swapped);
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

Task *priority()
{

    struct node *temp;
    Task *cpuTask = head->task;
    int priority1, priority2;
    temp = head->next;
    
    /*******************
     * Priority counter * 
     * ****************/
    pc = 1;
    while (temp != NULL){
        priority1=temp->task->priority;
        priority2=cpuTask->priority;
        if (priority1 > priority2 ){
            cpuTask = temp->task;
            pc = 1;
        }
        else if (priority1 == priority2){
            pc++;
        }
        temp = temp->next;
    }

    return cpuTask;
}

void schedule() 
{
    Task *cpuTask;
    int cpuservice[100]={0};
    int i=1, sq=0;
    int waitingtime[100], tat[100], bt[100], rt[100];
    struct node *temp;
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
    tmp = head;
    head=prev;
    temp = prev;
    for(i=0;i<cnt;i++){
        bt[i]=temp->task->burst;
        temp=temp->next;
    }

    sort(head);
    int tempQ;
    while (head != NULL) {
        cpuTask = priority();
        if (pc > 1){
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
        }
        else{
            tempQ=cpuTask->burst;
            run(cpuTask, cpuTask->burst);
            cpuTask->burst = 0;  
        }
        delete(&head, cpuTask);
        if(cpuservice[cpuTask->tid]==0) {
                    rt[cpuTask->tid]=sq;
                    cpuservice[cpuTask->tid]=1;
        }
        sq+=tempQ; 
        tat[cpuTask->tid]=sq;
    }
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                                //calculate averages
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    float avwt=0.0,avtat=0.0, respT=0.0;
    waitingtime[0]=0;
    
    for(i=0;i<cnt;i++){ 
        waitingtime[i] = tat[i]-bt[i]; 
        avwt+=waitingtime[i]; 
        avtat+=tat[i];
        respT+=rt[i];
    }
    
    avwt/=(1.0*cnt);
    avtat/=(1.0*cnt);
    respT/=(1.0*cnt);
    printf("\n\nAverage Waiting Time:%0.3f",avwt); 
    printf("\nAverage Turnaround Time:%0.3f\n",avtat); 
    printf("Average response time = %0.3f\n",respT);  
}
