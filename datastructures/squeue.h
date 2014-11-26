/*
 * squeue.h
 *
 *  Created on: Nov 25, 2014
 *      Author: richard
 */

#ifndef SQUEUE_H_
#define SQUEUE_H_

#include <stddef.h>
#include <stdint.h>


typedef struct
{
        int8_t size;     /**< Size of queue, also used as top */
        int8_t max;      /**< Maximum size of queue */
        uint8_t read;
        uint8_t write;
        size_t data_size; /**< Size of data stored on each node of queue */
        void* buffer;     /**< Pointer to array used as buffer */
} squeue_t;


#define squeue_size( queue )  ( ( *queue ).size + 1 )
#define squeue_full( queue )  ( ( *queue ).size + 1 == ( *queue ).max )

/**
 *  @brief <Basic Description>
 *
 *  @pre Array used as buffer needs to be available
 *
 *  @param[in] queue - pointer to squeue_t
 *  @param[in] max - max size of array
 *  @param[in] data_size - size of each element in array
 *  @param[in] buffer - pointer to array cast as void*
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 error
 */
int squeue_init( squeue_t* queue, uint8_t max, size_t data_size, void* buffer );

/**
 *  @brief Pops value from queue
 *
 *  @param[in] queue - pointer to squeue_t
 *  @param[out] payload - pointer to data to be populated from queue
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 Underflow
 */
int squeue_dequeue( squeue_t* queue, void* payload );

/**
 *  @brief Push value onto queue
 *
 *  @param[in] queue - pointer to squeue_t
 *  @param[in] payload - value to be copied into queue
 *
 *  @return int
 *    @retval 0 OK
 *    @retval -1 Overflow
 */
int squeue_enqueue( squeue_t* queue, void* payload );

/**
 *  @brief queue Top
 *
 *  @param[in] queue - pointer to squeue_t
 *
 *  @return void* - pointer to data on queue
 */
void* squeue_front( squeue_t* queue );


#endif /* SQUEUE_H_ */