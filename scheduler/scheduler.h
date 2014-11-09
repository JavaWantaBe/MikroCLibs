/**
 * @file scheduler.h
 *
 * @brief Simple Round Robin Task Scheduler
 *
 * @author Richard Lowe, 3/19/2013
 *
 * Test configuration:
 *   MCU:             %DEVICE%
 *   Dev.Board:       x
 *   Oscillator:      %DEVICE_CLOCK%
 *   Ext. Modules:    x
 *   SW:              %COMPILER%
 * NOTES:
 *
 */

/**
 * @code main.c
 *
 *
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

// task states
#define MAX_TASKS 7

#define SCH_SECONDS_1   1
#define SCH_SECONDS_5   5
#define SCH_SECONDS_10  10
#define SCH_SECONDS_15  15
#define SCH_SECONDS_30  30
#define SCH_MINUTES_1   60
#define SCH_MINUTES_15  60*15
#define SCH_MINUTES_30  60*30
#define SCH_HOURS_1     60*60
#define SCH_HOURS_12    60*60*12
#define SCH_DAY_1       60*60*24

// a task "type"
// pointer to a void function with no arguments
typedef void ( *task_t ) ( void );

typedef enum
{
    TASK_RUNNABLE = 0,
    TASK_RUNNING,
    TASK_STOPPED,
    TASK_ERROR
} task_status_e;


/**
 *  @brief Initializes RR Scheduler
 *
 *  NOTES:
 *   Sets all tasks to NULL
 */
void task_scheduler_init ( uint16_t clock );

/**
 *  @brief Function called by clock
 *
 *  @pre Clock needs to be initialized
 *
 *  @note
 *   <notes>
 */
void task_scheduler_clock ( void );

/**
 *  @brief Adds a task to the scheduler
 *
 *  @pre Scheduler must be initialized first
 *
 *  @param unsigned char id - ID of task, must be unique
 *  @param task_t task- Function that will be called when scheduler executes
 *  @param unsigned int period - how many nx100ms of delay the task requires
 *
 */
void task_add ( uint8_t id, task_t task, uint32_t period );

/**
 *  @brief Deletes task from scheduler
 *
 *  @pre Scheduler must be initialized first
 *
 *  @param unsigned char id - ID of task
 *
 */
void task_delete ( uint8_t id );

/**
 *  @brief Get Task Status
 *
 *  @pre Scheduler must be initialized first
 *
 *  @param unsigned char id - ID of task
 *
 *  @return enum taskStatus_e
 *    @retval TASK_RUNNABLE
 *    @retval TASK_RUNNING
 *    @retval TASK_STOPPED
 *    @retval TASK_ERROR
 */
task_status_e task_get_status ( uint8_t id );

/**
 *  @brief Calls Ready Tasks
 *
 *  @pre Scheduler must be initialized first
 *
 *  NOTES:
 *   Needs to be called in loop
 */
void task_dispatch ( void );

/**
 *  @brief Starts the task scheduler
 *
 *  \note
 *   <notes>
 */
void task_scheduler_start ( void );

/**
 *  @brief Stops the task scheduler
 *
 *  \note
 *   <notes>
 */
void task_scheduler_stop ( void );

#endif