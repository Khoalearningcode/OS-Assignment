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
#include "queue.h"

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
    struct queue_t* run_queue = caller->running_list;
    for (int j = 0; j < run_queue->size; j++)
    {
        char name[100];
        get_proc_name(run_queue->proc[j], name);
        if (strcmp(name, proc_name) == 0) 
        {
            libfree(run_queue->proc[j], memrg);
            for (int k = j; k < run_queue->size - 1; k++)
            {
                run_queue->proc[k] = run_queue->proc[k + 1];
            }
            run_queue->size--;
            j--;
        }
    }

#ifdef MLQ_SCHED
    struct queue_t * ready_queue = caller->mlq_ready_queue;
    for (int prio = 0; prio < MAX_PRIO; prio++)
    {
        struct queue_t * queue = &ready_queue[prio];
        for (int j = 0; j < queue->size; j++) 
        {
            char name[100];
            get_name(queue->proc[j], name);
            if (strcmp(name, proc_name) == 0) 
            {
                libfree(queue->proc[j], memrg);
                for (int k = j; k < queue->size - 1; k++) 
                {
                    queue->proc[k] = queue->proc[k + 1];
                }
                queue->size--;
                j--;
            }
        }
    }
#else
    struct queue_t * ready_queue = caller->ready_queue;
    for (int j = 0; j < ready_queue->size; j++) 
    {
        char name[100];
        get_name(ready_queue->proc[j], name);
        if (strcmp(name, proc_name) == 0) 
        { 
            libfree(ready_queue->proc[j], memrg);
            for (int k = j; k < ready_queue->size - 1; k++) 
            {
                ready_queue->proc[k] = ready_queue->proc[k + 1];
            }
            ready_queue->size--;
            j--;
        }
    }
#endif
    

    return 0;
    
}
