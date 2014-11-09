#include "scheduler.h"


// basic task control block (TCB)
typedef struct
{
    uint8_t   id;                // task ID
    task_t    task;              // pointer to the task
    uint32_t  delay;             // delay before execution
    uint32_t  period;
    task_status_e task_status;   // status of task
} task_control_block_t;

// Array of tasks
static task_control_block_t task_list[MAX_TASKS];
// counter in clock
static volatile uint16_t count;
// used to calculate delay based on user input
static uint16_t count_per_second;
// flag for enabling / disabling scheduler
static volatile uint8_t task_scheduler_running;


// initialises the task list
void task_scheduler_init( uint16_t clock )
{
    int i = 0;

    task_scheduler_running = 0;

    if( clock <= 1000 )
    {
        count_per_second = 1000 / clock;
    }
    else
    {
         return;
    }

    for( i = 0; i < MAX_TASKS; i++ )
    {
        task_list[i].id          = 0;
        task_list[i].task        = ( task_t ) 0x00;
        task_list[i].delay       = 0;
        task_list[i].period      = 0;
        task_list[i].task_status = TASK_STOPPED;
    }
}

// clock
void task_scheduler_clock()
{
    if( task_scheduler_running == 1 )
    {
        count++;

        if( count >= count_per_second )
        {
            int i = 0;
            count = 0;

            // cycle through available tasks
            for( i = 0; i < MAX_TASKS; i++ )
            {
                if( task_list[i].task_status == TASK_RUNNABLE )
                {
                    if( task_list[i].delay > 0 )
                    {
                        task_list[i].delay--;
                    }
                }
            }
        }
    }
}

// adds a new task to the task list
// scans through the list and
// places the new task data where
// it finds free space
void task_add( uint8_t id, task_t task, uint32_t period )
{
    uint8_t task_id = 0;

    while( task_id < MAX_TASKS )
    {
        if( task_list[task_id].task_status == TASK_STOPPED )
        {
            task_list[task_id].id          = id;
            task_list[task_id].task        = task;
            task_list[task_id].delay       = period;
            task_list[task_id].period      = period;
            task_list[task_id].task_status = TASK_RUNNABLE;
            break;
        }
        
        task_id++;
    }
}

// remove task from task list
// note STOPPED is equivalent
// to removing a task
void task_delete( uint8_t id )
{
    int i;

    for( i = 0; i < MAX_TASKS; i++ )
    {
        if( task_list[i].id == id )
        {
            task_list[i].task = ( task_t ) 0x00;
            task_list[i].task_status = TASK_STOPPED;
            break;
        }
    }
}

// gets the task status
// returns ERROR if id is invalid
task_status_e task_get_status( uint8_t id )
{
    int i;

    for( i = 0; i < MAX_TASKS; i++ )
    {
        if( task_list[i].id == id )
        {
            return task_list[i].task_status;
        }
    }

    return TASK_ERROR;
}

// dispatches tasks when they are ready to run
void task_dispatch()
{
    if( task_scheduler_running == 1 )
    {
        int i;
        
        for( i = 0; i < MAX_TASKS; i++ )
        {
            // check for a valid task ready to run
            if( ( task_list[i].delay == 0 ) && ( task_list[i].task_status == TASK_RUNNABLE ) )
            {
                task_list[i].task_status = TASK_RUNNING;  // task is now running
                ( *task_list[i].task )();                 // call the task
                task_list[i].delay = task_list[i].period; // reset the delay
                task_list[i].task_status = TASK_RUNNABLE; // task is runnable again
            }
        }
    }
}

// Starts the scheduler
void task_scheduler_start()
{
    task_scheduler_running = 1;
}

// Stops the scheduler
void task_scheduler_stop()
{
    task_scheduler_running = 0;
}