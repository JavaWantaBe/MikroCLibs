/**
 * @file scheduler.h
 *
 * @brief Simple round robin task scheduler
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 08/11/2014
 *
 * @version .1 - Initial
 *
 * @details
 *
 * Status: <XX% completed.>
 *
 * @note
 * Test configuration:
 *   MCU:             %DEVICE%
 *   Dev.Board:       x
 *   Oscillator:      %DEVICE_CLOCK%
 *   Ext. Modules:    x
 *   SW:              %COMPILER%
 *
 * \par
 *   <all that matters>
 */


/**
 * @code
 *  void main()
 *  {
 *      initTimer();
 *
 *      task_scheduler_init( 1000 );
 *      task_add( 0, task1, SCH_SECONDS_1 );
 *      task_add( 1, task2, SCH_SECONDS_5 );
 *
 *      task_scheduler_start();
 *
 *      while(1)
 *      {
 *          task_dispatch();
 *      }
 *
 *  #endcode
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

/* pointer to a void function with no arguments */
typedef void ( *task_t ) ( void );

/**
 * @enum Status of tasks in scheduler
 *
 */
typedef enum
{
    TASK_RUNNABLE = 0,      /**< Task is ready to be ran   */
    TASK_RUNNING,           /**< Task is currently running */
    TASK_STOPPED,           /**< Task is stopped           */
    TASK_ERROR              /**< Error has occurred        */
} task_status_e;


/**
 *  @brief Initializes RR Scheduler
 *
 *  Initialization only requires a clock parameter.  Clock represents the
 *  time that expires between calles to task_scheduler_clock().
 *
 *  @param uint16_t clock -
 *
 *  @code
 *    task_scheduler_init( 1000 ); // 1000 ms between calls
 *  @endcode
 *
 *  @note
 *   Sets all tasks to NULL
 */
void task_scheduler_init( uint16_t clock );

/**
 *  @brief Function called by timer
 *
 *  @pre Clock needs to be initialized
 *
 */
void task_scheduler_clock( void );

/**
 *  @brief Adds a task to the scheduler
 *
 *  @pre Scheduler must be initialized first
 *
 *  @param uint8_t id - ID of task, must be unique
 *  @param task_t task- Function that will be called when scheduler executes
 *  @param uint32_t period - how many nx100ms of delay the task requires
 *
 */
void task_add( uint8_t id, task_t task, uint32_t period );

/**
 *  @brief Deletes task from scheduler
 *
 *  @param uint8_t id - ID of task
 *
 */
void task_delete( uint8_t id );

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
task_status_e task_get_status( uint8_t id );

/**
 *  @brief Calls Ready Tasks
 *
 *  @pre Scheduler must be initialized first
 *
 *  @note
 *   Needs to be called in main while loop
 */
void task_dispatch( void );

/**
 *  @brief Starts the task scheduler
 *
 *  @note
 *       At system initialization, tasks are undesirable to be running.
 *       Scheduler will not run until this function is called.
 */
void task_scheduler_start ( void );

/**
 *  @brief Stops the task scheduler
 *
 *  @note
 *
 */
void task_scheduler_stop ( void );

#endif