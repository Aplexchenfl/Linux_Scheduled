#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int quantum_time = 0;

struct scheduled_jobs
{
    int sche_count;
    int burst_time;
    char sche_name[16];
    int start_time;
    int stop_time;
    int wait_time;
    int turn_around_time;
    int remain_time;
    struct scheduled_jobs *next;
    struct scheduled_jobs *pre;
};

struct rr_scheduled_jobs
{
    int sche_count;
    int burst_time;
    char sche_name[16];
    int start_time;
    int stop_time;
    int wait_time;
    int turn_around_time;
    struct rr_scheduled_jobs *next;
    struct rr_scheduled_jobs *pre;
};

void put_hint(void)
{
    putchar('\n');
    putchar('\n');
    printf("scheduled jobs\n\n");
}

void sche_display_burst(struct scheduled_jobs *all_jobs)
{
    printf("%s        %d\n", all_jobs->sche_name, all_jobs->burst_time);
}

void sche_display_start_stop(struct scheduled_jobs *all_jobs)
{
    printf("%s           %2d          %2d\n", all_jobs->sche_name, all_jobs->start_time, all_jobs->stop_time);
}

void sche_display_wait(struct scheduled_jobs *all_jobs)
{
    printf("%s          %d\n", all_jobs->sche_name, all_jobs->wait_time);
}

void sche_display_turn_around(struct scheduled_jobs *all_jobs)
{
    printf("%s          %d\n", all_jobs->sche_name, all_jobs->turn_around_time);
}

void sche_display_all(struct scheduled_jobs *all_jobs, int flag)
{
    struct scheduled_jobs *save_point;
    float count_num = 0, count_wait = 0, count_turn_around = 0;

    save_point = all_jobs;
    while(all_jobs)
    {
        count_num++;
        count_wait += all_jobs->wait_time;
        count_turn_around += all_jobs->turn_around_time;

        if (flag == 1)
            sche_display_burst(all_jobs);
        else if (flag == 2)
            sche_display_start_stop(all_jobs);
        else if (flag == 3)
            sche_display_wait(all_jobs);
        else if (flag == 4)
            sche_display_turn_around(all_jobs);

        all_jobs = all_jobs->next;
    }
    putchar('\n');

    if (flag == 3)
        printf("Average waiting time    :      %0.2f\n\n", count_wait / count_num);
    else if (flag == 4)
        printf("Average turnaround time :      %0.2f\n\n", count_turn_around / count_num);

    all_jobs = save_point;

    return;
}

void fcfs_sched(struct scheduled_jobs *all_jobs)
{
    int count = 0;
    struct scheduled_jobs *save_point;

    put_hint();

    save_point = all_jobs;
    while(all_jobs)
    {
        count++;
        if (count == 1)
        {
            all_jobs->start_time = 0;
        }
        else
        {
            all_jobs->start_time = all_jobs->pre->stop_time;
        }
        all_jobs->stop_time = all_jobs->start_time + all_jobs->burst_time;

        all_jobs->wait_time = all_jobs->start_time;
        all_jobs->turn_around_time = all_jobs->stop_time;

        all_jobs = all_jobs->next;
    }
    all_jobs = save_point;

    // display burst times
    printf("\nProcess   Burst Time\n");
    sche_display_all(all_jobs, 1);

    // display start and stop times
    printf("Start Time   Stop Time\n");
    sche_display_all(all_jobs, 2);

    // display wait times
    printf("Computing   Wait Time\n");
    sche_display_all(all_jobs, 3);

    // display turn around times
    printf("Computing   Turn around Time\n");
    sche_display_all(all_jobs, 4);

    return;
}

void sjf_sched(struct scheduled_jobs *all_jobs)
{
    int count = 0, save_time, tmp;
    struct scheduled_jobs *save_point, *tmp_jobs, *cur_jobs, *tmp_sort, *tmp_save;

    put_hint();

    // sort
    cur_jobs = all_jobs;
    do {
        tmp_jobs = cur_jobs;
        while(tmp_jobs)
        {
            if(cur_jobs->burst_time > tmp_jobs->burst_time)
            {
                save_time = cur_jobs->burst_time;
                cur_jobs->burst_time = tmp_jobs->burst_time;
                tmp_jobs->burst_time = save_time;

                tmp = cur_jobs->sche_count;
                cur_jobs->sche_count = tmp_jobs->sche_count;
                tmp_jobs->sche_count = tmp;
            }
            tmp_jobs = tmp_jobs->next;
        }
        cur_jobs = cur_jobs->next;
    } while(cur_jobs->next);

    // calculate times
    save_point = all_jobs;
    while(all_jobs)
    {
        count++;
        if (count == 1)
        {
            all_jobs->start_time = 0;
        }
        else
        {
            all_jobs->start_time = all_jobs->pre->stop_time;
        }
        all_jobs->stop_time = all_jobs->start_time + all_jobs->burst_time;

        all_jobs->wait_time = all_jobs->start_time;
        all_jobs->turn_around_time = all_jobs->stop_time;

        all_jobs = all_jobs->next;
    }
    all_jobs = save_point;

    // back
    cur_jobs = all_jobs;
    do {
        tmp_jobs = cur_jobs;
        while(tmp_jobs)
        {
            if(cur_jobs->sche_count > tmp_jobs->sche_count)
            {
                save_time = cur_jobs->burst_time;
                cur_jobs->burst_time = tmp_jobs->burst_time;
                tmp_jobs->burst_time = save_time;

                tmp = cur_jobs->sche_count;
                cur_jobs->sche_count = tmp_jobs->sche_count;
                tmp_jobs->sche_count = tmp;

                tmp = cur_jobs->start_time;
                cur_jobs->start_time = tmp_jobs->start_time;
                tmp_jobs->start_time = tmp;

                tmp = cur_jobs->stop_time;
                cur_jobs->stop_time = tmp_jobs->stop_time;
                tmp_jobs->stop_time = tmp;

                tmp = cur_jobs->wait_time;
                cur_jobs->wait_time = tmp_jobs->wait_time;
                tmp_jobs->wait_time = tmp;

                tmp = cur_jobs->turn_around_time;
                cur_jobs->turn_around_time = tmp_jobs->turn_around_time;
                tmp_jobs->turn_around_time = tmp;
            }
            tmp_jobs = tmp_jobs->next;
        }
        cur_jobs = cur_jobs->next;
    } while(cur_jobs->next);

    // display burst times
    printf("\nProcess   Burst Time\n");
    sche_display_all(all_jobs, 1);

    // display start and stop times
    printf("Computing   Start Time   Stop Time\n");
    sche_display_all(all_jobs, 2);

    // display wait times
    printf("Computing   Wait Time\n");
    sche_display_all(all_jobs, 3);

    // display turn around times
    printf("Computing   Turn around Time\n");
    sche_display_all(all_jobs, 4);
}

void srt_sched(struct scheduled_jobs *all_jobs)
{
    int count = 0, save_time, tmp;
    struct scheduled_jobs *save_point, *tmp_jobs, *cur_jobs, *tmp_sort, *tmp_save;

    put_hint();

    // sort
    cur_jobs = all_jobs;
    do {
        tmp_jobs = cur_jobs;
        while(tmp_jobs)
        {
            if(cur_jobs->burst_time > tmp_jobs->burst_time)
            {
                save_time = cur_jobs->burst_time;
                cur_jobs->burst_time = tmp_jobs->burst_time;
                tmp_jobs->burst_time = save_time;

                tmp = cur_jobs->sche_count;
                cur_jobs->sche_count = tmp_jobs->sche_count;
                tmp_jobs->sche_count = tmp;
            }
            tmp_jobs = tmp_jobs->next;
        }
        cur_jobs = cur_jobs->next;
    } while(cur_jobs->next);

    // calculate times
    save_point = all_jobs;
    while(all_jobs)
    {
        count++;
        if (count == 1)
        {
            all_jobs->start_time = 0;
        }
        else
        {
            all_jobs->start_time = all_jobs->pre->stop_time;
        }
        all_jobs->stop_time = all_jobs->start_time + all_jobs->burst_time;

        all_jobs->wait_time = all_jobs->start_time;
        all_jobs->turn_around_time = all_jobs->stop_time;

        all_jobs = all_jobs->next;
    }
    all_jobs = save_point;

    // back
    cur_jobs = all_jobs;
    do {
        tmp_jobs = cur_jobs;
        while(tmp_jobs)
        {
            if(cur_jobs->sche_count > tmp_jobs->sche_count)
            {
                save_time = cur_jobs->burst_time;
                cur_jobs->burst_time = tmp_jobs->burst_time;
                tmp_jobs->burst_time = save_time;

                tmp = cur_jobs->sche_count;
                cur_jobs->sche_count = tmp_jobs->sche_count;
                tmp_jobs->sche_count = tmp;

                tmp = cur_jobs->start_time;
                cur_jobs->start_time = tmp_jobs->start_time;
                tmp_jobs->start_time = tmp;

                tmp = cur_jobs->stop_time;
                cur_jobs->stop_time = tmp_jobs->stop_time;
                tmp_jobs->stop_time = tmp;

                tmp = cur_jobs->wait_time;
                cur_jobs->wait_time = tmp_jobs->wait_time;
                tmp_jobs->wait_time = tmp;

                tmp = cur_jobs->turn_around_time;
                cur_jobs->turn_around_time = tmp_jobs->turn_around_time;
                tmp_jobs->turn_around_time = tmp;
            }
            tmp_jobs = tmp_jobs->next;
        }
        cur_jobs = cur_jobs->next;
    } while(cur_jobs->next);

    // display burst times
    printf("\nProcess   Burst Time\n");
    sche_display_all(all_jobs, 1);

    // display start and stop times
    printf("Computing   Start Time   Stop Time\n");
    sche_display_all(all_jobs, 2);

    // display wait times
    printf("Computing   Wait Time\n");
    sche_display_all(all_jobs, 3);

    // display turn around times
    printf("Computing   Turn around Time\n");
    sche_display_all(all_jobs, 4);
}

void rr_sche_display_all(struct rr_scheduled_jobs *cur_rr_jobs)
{
    struct rr_scheduled_jobs *save_point;
    save_point = cur_rr_jobs;
    while(save_point)
    {
        printf("%s           %2d          %2d\n", save_point->sche_name, save_point->start_time, save_point->stop_time);
        save_point = save_point->next;
    }
    putchar('\n');
    return;
}

void rr_free_sched(struct rr_scheduled_jobs *all_jobs)
{
    struct rr_scheduled_jobs *save_point;
    while(all_jobs)
    {
        save_point = all_jobs->next;
        free(all_jobs);
        all_jobs = save_point;
    }
}

void rr_sched(struct scheduled_jobs *all_jobs)
{
    struct rr_scheduled_jobs *rr_jobs, *new_rr_jobs, *cur_rr_jobs;
    struct scheduled_jobs *cur_sche_jobs;
    int sche_count = 0, count_num = 0;

    rr_jobs = (struct rr_scheduled_jobs *)malloc(sizeof(struct rr_scheduled_jobs));
    if (!rr_jobs)
        perror("malloc");

    // init rr_jobs head
    rr_jobs->next = NULL;
    rr_jobs->pre = rr_jobs;
    rr_jobs->start_time = 0;

    // all burst time count
    cur_sche_jobs = all_jobs;
    while(cur_sche_jobs)
    {
        sche_count += cur_sche_jobs->burst_time;
        cur_sche_jobs->remain_time = cur_sche_jobs->burst_time;
        cur_sche_jobs = cur_sche_jobs->next;
    }

    cur_sche_jobs = all_jobs;
    cur_rr_jobs = rr_jobs;
    while(sche_count > 0)
    {
        count_num++;

        // if remain time is greater than zero , run it
        if (cur_sche_jobs->remain_time > 0)
        {
            // run it over,  update remain time
            cur_sche_jobs->remain_time -= quantum_time;

            // if remain_time is greater than zero
            if (cur_sche_jobs->remain_time >= 0)
                sche_count -= quantum_time;
            else
                sche_count -= (quantum_time + cur_sche_jobs->remain_time);

            if (count_num == 1)
            {
                // first time rr
                cur_rr_jobs->burst_time = cur_sche_jobs->burst_time;
                strcpy(cur_rr_jobs->sche_name, cur_sche_jobs->sche_name);
                cur_rr_jobs->next = NULL;
                cur_rr_jobs->pre = cur_rr_jobs;

                // star , stop, turn around time
                cur_rr_jobs->start_time = 0;
                if (cur_sche_jobs->remain_time >= 0)
                    cur_rr_jobs->stop_time = cur_rr_jobs->start_time + quantum_time;
                else
                    cur_rr_jobs->stop_time = cur_rr_jobs->start_time + quantum_time + cur_sche_jobs->remain_time;

                cur_sche_jobs->turn_around_time = cur_rr_jobs->stop_time;
            }
            else
            {
                // and then time rr
                new_rr_jobs = (struct rr_scheduled_jobs *)malloc(sizeof(struct rr_scheduled_jobs));
                if (!new_rr_jobs)
                    perror("malloc");
                new_rr_jobs->next = NULL;

                new_rr_jobs->burst_time = cur_sche_jobs->burst_time;
                strcpy(new_rr_jobs->sche_name, cur_sche_jobs->sche_name);

                // link
                cur_rr_jobs->next = new_rr_jobs;
                new_rr_jobs->pre = cur_rr_jobs;

                // turn to next cur_rr_jobs
                cur_rr_jobs = cur_rr_jobs->next;

                // star , stop, turn around time
                cur_rr_jobs->start_time = cur_rr_jobs->pre->stop_time;
                if (cur_sche_jobs->remain_time >= 0)
                    cur_rr_jobs->stop_time = cur_rr_jobs->start_time + quantum_time;
                else
                    cur_rr_jobs->stop_time = cur_rr_jobs->start_time + quantum_time + cur_sche_jobs->remain_time;

                cur_sche_jobs->turn_around_time = cur_rr_jobs->stop_time;
            }

            /*  debug
            printf("name : %s,  start: %d,  stop: %d, turn around: %d\n", \
                   cur_rr_jobs->sche_name, cur_rr_jobs->start_time, \
                   cur_rr_jobs->stop_time, cur_sche_jobs->turn_around_time);
            printf("DEBUG ------ sche_count, : %d   remain_time:%d\n\n", \
                   sche_count, cur_sche_jobs->remain_time );
            */
        }

        // backrun
        if (cur_sche_jobs->next)
            cur_sche_jobs = cur_sche_jobs->next;
        else
            cur_sche_jobs = all_jobs;
    }


    cur_sche_jobs = all_jobs;
    // display burst times
    printf("\nProcess   Burst Time\n");
    sche_display_all(cur_sche_jobs, 1);

    cur_rr_jobs = rr_jobs;
    printf("Computing   Start Time   Stop Time\n");
    rr_sche_display_all(cur_rr_jobs);

    cur_sche_jobs = all_jobs;
    // display turn around times
    printf("Computing   Turn around Time\n");
    sche_display_all(cur_sche_jobs, 4);

    //free
    rr_free_sched(cur_rr_jobs);
}

void free_sched(struct scheduled_jobs *all_jobs)
{
    struct scheduled_jobs *save_point;
    while(all_jobs)
    {
        save_point = all_jobs->next;
        free(all_jobs);
        all_jobs = save_point;
    }
}

int main(void)
{
    struct scheduled_jobs *all_jobs, *new_jobs, *cur_jobs;
    char sel_cmp[4][32] = {"FCFS", "SJF", "SRT", "RR"};
    int sche_count = 0, scanf_ret = 0, sche_time;
    char sche_name[16];
    char sel[32];

    printf("Welcome to Job Scheduler Computer\nWhat algorithm would you like to use: \n> ");
    scanf("%s", sel);

    // check input error
    if (!strcmp(sel, sel_cmp[0]) || !strcmp(sel, sel_cmp[1]) || !strcmp(sel, sel_cmp[2]))
    {
        ;
    }
    else if (!strcmp(sel, sel_cmp[3]))
    {
        printf("What is the time quantum for RR scheduling:\n> ");
        scanf("%d", &quantum_time);
    }
    else
    {
        perror("Input selections error,  please input: FCFS, SJF SRT, RR");
        return 0;
    }

    // alloc cur_job struct
    cur_jobs = (struct scheduled_jobs *)malloc(sizeof(struct scheduled_jobs));
    if (cur_jobs == NULL)
        perror("malloc ");

    // end flag of cur_jobs
    cur_jobs->next = NULL;

    while(1)
    {
        sche_count++;
        sprintf(sche_name, "P%d", sche_count);
        printf("Process %d (%s) – What is the burst time?\n> ",sche_count, sche_name);
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
            cur_jobs->sche_count = sche_count;
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
            new_jobs->sche_count = sche_count;

            // cur_jobs --> new_jobs;
            cur_jobs = new_jobs;
            // free new_jobs;
            new_jobs = NULL;
        }
    }

    if (!strcmp(sel, sel_cmp[0]))
    {
        fcfs_sched(all_jobs);
    }
    else if (!strcmp(sel, sel_cmp[1]))
    {
        sjf_sched(all_jobs);
    }
    else if (!strcmp(sel, sel_cmp[2]))
    {
        srt_sched(all_jobs);
    }
    else if (!strcmp(sel, sel_cmp[3]))
    {
        rr_sched(all_jobs);
    }
    else
        perror("Input selections error,  please input: FCFS, SJF SRT, RR");

    free_sched(all_jobs);

    return 0;
}
