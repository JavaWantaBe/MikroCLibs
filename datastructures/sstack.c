#include "sstack.h"
#include <string.h>

int sstack_init( sstack_t* stack, int16_t max, size_t data_size, void* buffer )
{
    if( stack == NULL || max == 0 || data_size == 0 || buffer == NULL )
        return -1;

    stack->size = -1;
    stack->max = max - 1;
    stack->buffer = buffer;
    stack->data_size = data_size;

    return 0;
}

int sstack_pop( sstack_t* stack, void* payload )
{
    uint8_t* tmp = ( uint8_t* )stack->buffer;

    if( stack->size < 0 )
        return -1;

    tmp += stack->size * stack->data_size;
    ( stack->size )--;
    memcpy( payload, tmp, stack->data_size );

    return 0;
}

int sstack_push( sstack_t* stack, void* payload )
{
    uint8_t* tmp = ( uint8_t* )stack->buffer;

    if( stack->size == stack->max )
        return -1;

    ++( stack->size );
    tmp += stack->size * stack->data_size;
    memcpy( tmp, payload, stack->data_size );

    return 0;
}

void* sstack_top( sstack_t* stack )
{
    uint8_t* tmp = ( uint8_t* )stack->buffer;

    return tmp + ( stack->size * stack->data_size );
}