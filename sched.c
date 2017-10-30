#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct scheduled_jobs
{
    int burst_time;
    char sche_name[16];
    struct scheduled_jobs *next;
    struct scheduled_jobs *pre;
};

void fcfs_sched(void)
{
    printf("fcfs\n");
}

void sjf_sched(void)
{
    printf("sjf\n");
}

srt_sched(void)
{
    printf("srt\n");
}

rr_sched()
{
    printf("rr\n");
}

int main(void)
{
    struct scheduled_jobs *all_jobs, *new_jobs, *cur_jobs;
    char sche_name[16];
    char sel_cmp[4][32] = {"FCFS", "SJF", "SRT", "RR"};
    int sche_count = 0, scanf_ret = 0, sche_time;
    char sel[32];

    // alloc cur_job struct
    cur_jobs = (struct scheduled_jobs *)malloc(sizeof(struct scheduled_jobs));
    if (cur_jobs == NULL)
        perror("malloc ");

    // end flag of cur_jobs
    cur_jobs->next = NULL;

    printf("Welcome to Job Scheduler Computer\nWhat algorithm would you like to use: \n> ");
    scanf("%s", sel);

    while(1)
    {
        sche_count++;
        sprintf(sche_name, "P%d", sche_count);
        printf("Process 1 (%s) – What is the burst time?\n> ", sche_name);
        scanf_ret = scanf("%d", &sche_time);
        if (scanf_ret == -1)
            break;

        if (sche_count == 1)
        {
            all_jobs = cur_jobs;
            all_jobs->next = cur_jobs;
            all_jobs->pre = cur_jobs;

            // save first jobs
            cur_jobs->burst_time = sche_time;
            strcpy(cur_jobs->sche_name, sche_name);
        }
        else
        {
            // alloc new jobs;
            new_jobs = (struct scheduled_jobs *)malloc(sizeof(struct scheduled_jobs));
            cur_jobs->next = new_jobs;
            new_jobs->next = NULL;
            new_jobs->pre = cur_jobs;

            // save second and next...
            new_jobs->burst_time = sche_time;
            strcpy(new_jobs->sche_name, sche_name);

            // cur_jobs --> new_jobs;
            cur_jobs = new_jobs;
            // free new_jobs;
            new_jobs = NULL;
        }
    }

    while(all_jobs)
    {
        printf("name : %s     brust_time:   %d\n ", all_jobs->sche_name, all_jobs->burst_time);
        all_jobs = all_jobs->next;
    }

    if (!strcmp(sel, sel_cmp[0]))
    {
        fcfs_sched();
    }
    else if (!strcmp(sel, sel_cmp[1]))
    {
        sjf_sched();
    }
    else if (!strcmp(sel, sel_cmp[2]))
    {
        srt_sched();
    }
    else if (!strcmp(sel, sel_cmp[3]))
    {
        rr_sched();
    }
    else
        perror("Input selections error");

    return 0;
}
