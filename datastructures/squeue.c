/*
 * squeue.c
 *
 *  Created on: Nov 25, 2014
 *      Author: richard
 */

#include "squeue.h"
#include <string.h>


int squeue_init( squeue_t* queue, int16_t max, size_t data_size, void* buffer )
{
    if( queue == NULL || max == 0 || data_size == 0 || buffer == NULL )
        return -1;

    queue->size = -1;
    queue->max = max;
    queue->buffer = buffer;
    queue->data_size = data_size;
    queue->read = 0;
    queue->write = 0;

    return 0;
}


int squeue_dequeue( squeue_t* queue, void* payload )
{
    uint8_t* tmp = ( uint8_t* )queue->buffer;

    if( queue->size < 0 )
        return -1;

    tmp += ( queue->read ) * queue->data_size;
    memcpy( payload, tmp, queue->data_size );
    ( queue->size )--;

    if( queue->read == queue->max )
        queue->read = queue->read % queue->max;
    else
        queue->read = ( queue->read + 1 ) % queue->max;

    return 0;
}


int squeue_enqueue( squeue_t* queue, void* payload )
{
    uint8_t* tmp = ( uint8_t* )queue->buffer;

    if( queue->size == queue->max - 1 )
        return -1;

    tmp += ( queue->write ) * queue->data_size;
    memcpy( tmp, payload, queue->data_size );
    queue->size++;

    if( queue->write == queue->max )
        queue->write = queue->write % queue->max;
    else
        queue->write = ( queue->write + 1 ) % queue->max;

    return 0;
}


void* squeue_front( squeue_t* queue )
{
    uint8_t* tmp = ( uint8_t* )queue->buffer;
    return tmp + ( queue->read * queue->data_size );
}