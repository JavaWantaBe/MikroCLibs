/**
 * @file heap.h
 *
 * @brief <Brief Description>
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date
 *
 * @version <versionNo> - <change_description>
 *
 * @details
 *
 * Status: <XX% completed.>
 *
 * \note
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
 
#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
   size_t data_size;
   int16_t last;
   int16_t max;
   int ( *compare ) ( void* const key, void* const key2 );
   void* buffer;
} heap_t;

int heap_init( heap_t* heap,
               int16_t max,
               size_t data_size,
               int ( *compare )( void* const key, void* const key2 ),
               void* buffer );

int heap_insert(heap_t* heap, void* payload );

int heap_delete(heap_t* heap, void** dataOutPtr);

int heap_count(heap_t* heap);

int heap_full(heap_t* heap);

int heap_empty( heap_t* heap );

void heapDestroy( heap_t* heap );


#endif