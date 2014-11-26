#include <heap.h>
#include <math.h>
#include <string.h>


static void reheap_up( heap_t* heap, int child_loc );
static void reheap_down( heap_t* heap, int root );

static void reheap_up( heap_t* heap, int child_loc )
{
    int parent;
    uint8_t* tmp;
    void* hold;

    // if not at root of heap -- index 0
    if( child_loc )
    {
        tmp = ( uint8_t* )heap->buffer;
        parent = ( child_loc - 1 ) / 2;
        // child is greater than parent -- swap
        if( heap->compare( tmp + ( child_loc * heap->data_size ),
                           tmp + ( parent * heap->data_size ) ) > 0 )
        {

        } // if heap[]
    } // if newNode

    return;
}

static void reheap_down( heap_t* heap, int root )
{


}



int heap_init( heap_t* heap,
               int16_t max,
               size_t data_size,
               int ( *compare )( void* const key, void* const key2 ),
               void* buffer )
{
    if( heap == NULL || compare == NULL || max == 0 || buffer == NULL 
        || data_size == 0 || ( max % 2 == 0 ) )
        return -1;
    
    heap->data_size = data_size;
    heap->max       = max - 1;
    heap->last      = -1;
    heap->compare   = compare;
    heap->buffer = buffer;
    
    // Force heap size to power of 2 -1
    //heap->max = (int) pow ( 2, ceil( log( max ) ) ) - 1;

    
    return 0;
}

int heap_insert( heap_t* heap, void* payload )
{
    uint8_t* tmp = ( uint8_t* )heap->buffer;
    // Heap full
    if ( heap->last == heap->max )
        return -1;
    // Heap empty
    tmp += ++( heap->last ) * heap->data_size;
    memcpy( tmp, payload, heap->data_size );

    reheap_up( heap, heap->last );
    
    return 0;
}

int heap_delete( heap_t* heap, void** dataOutPtr )
{
    return 0;
}

int heap_count( heap_t* heap )
{
    return 0;
}

int heap_full(heap_t* heap )
{
    return 0;
}

int heap_empty( heap_t* heap )
{
    return 0;
}

void heapDestroy( heap_t* heap )
{

}

