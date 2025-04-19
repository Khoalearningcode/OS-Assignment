/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "common.h"
#include "syscall.h"
#include "stdio.h"
#include "libmem.h"

int __sys_killall(struct pcb_t *caller, struct sc_regs* regs)
{
    char proc_name[100];
    uint32_t data;

    //hardcode for demo only
    uint32_t memrg = regs->a1;
    
    /* TODO: Get name of the target proc */
    //proc_name = libread..
    int i = 0;
    data = 0;
    while(data != -1){
        libread(caller, memrg, i, &data);
        proc_name[i]= data;
        if(data == -1) proc_name[i]='\0';
        i++;
    }
    printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

    /* TODO: Traverse proclist to terminate the proc
     *       stcmp to check the process match proc_name
     */
     

    //caller->running_list
    //caller->mlq_ready_queu

    /* TODO Maching and terminating 
     *       all processes with given
     *        name in var proc_name
     */
    struct queue_t *queue = caller->mlq_ready_queue;
    struct queue_t temp_queue;
    temp_queue.size = 0;
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) 
    {
        temp_queue.proc[i] = NULL;  
    }
    if (queue != NULL) 
    {
        struct pcb_t *proc;
        while(!empty(queue))
        {
            proc = dequeue(queue);
            if (strcmp(proc->path, proc_name) == 0) 
            {
                libfree(proc, memrg);
            } 
            else 
            {
                enqueue(&temp_queue, proc);
            }
        }
    }
    while (!empty(&temp_queue)) 
    {
        struct pcb_t *proc = dequeue(&temp_queue);
        enqueue(queue, proc);
    }

    struct queue_t *rqueue = caller->running_list;
    if (rqueue != NULL) 
    {
        struct pcb_t *proc;
        while(!empty(rqueue))
        {
            proc = dequeue(rqueue);
            if (strcmp(proc->path, proc_name) == 0) 
            {
                libfree(proc, memrg);
            } 
            else 
            {
                enqueue(&temp_queue, proc);
            }
        }
    }
    while (!empty(&temp_queue)) 
    {
        struct pcb_t *proc = dequeue(&temp_queue);
        enqueue(rqueue, proc);
    }

    return 0;
    
}
