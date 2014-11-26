/**
 * @file sstack.h
 *
 * @brief Static Stack Implimentation
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 23/11/2014
 *
 * @version 1.0 - Initial
 *
 * @details
 *
 * Status: 90% Complete
 *
 * @note
 * Test configuration:
 *   MCU:             ATMega32
 *   Dev.Board:       EasyAVR v7
 *   Oscillator:      8Mhz
 *   Ext. Modules:
 *   SW:              MikroC v6.0
 *
 * @par
 *   <all that matters>
 */
 
#ifndef _SSTACK_H
#define _SSTACK_H

#include <stdint.h>
#include <stddef.h>

#define STACK_MAX 50

/**
 *  @struct
 */
typedef struct
{
    int8_t size;      /**< Size of stack, also used as top */
    uint8_t max;      /**< Maximum size of stack */
    size_t data_size; /**< Size of data stored on each node of stack */
    void* buffer;     /**< Pointer to array used as buffer */
} sstack_t;

#define sstack_size( stack )  ( ( *stack ).size + 1 )
#define sstack_full( stack )  ( ( *stack ).size == ( *stack ).max )

/**
 *  @brief <Basic Description>
 *
 *  @pre Array used as buffer needs to be available
 *
 *  @param[in] stack - pointer to sstack_t
 *  @param[in] max - max size of array
 *  @param[in] data_size - size of each element in array
 *  @param[in] buffer - pointer to array cast as void*
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 error
 */
int sstack_init( sstack_t* stack, uint8_t max, size_t data_size, void* buffer );

/**
 *  @brief Pops value from stack
 *
 *  @param[in] stack - pointer to sstack_t
 *  @param[out] payload - pointer to data to be populated from stack
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 Underflow
 */
int sstack_pop( sstack_t* stack, void* payload );

/**
 *  @brief Push value onto stack
 *
 *  @param[in] stack - pointer to sstack_t
 *  @param[in] payload - value to be copied into stack
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 Overflow
 */
int sstack_push( sstack_t* stack, void* payload );

/**
 *  @brief Stack Top
 *
 *  @param[in] stack - pointer to sstack_t
 *
 *  @return void* - pointer to data on stack
 */
void* sstack_top( sstack_t* stack );

#endif