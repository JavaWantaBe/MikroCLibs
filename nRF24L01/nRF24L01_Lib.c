
#include <stdint.h>
#include "radio.h"

void system_init()
{
    radio_pl_init( HAL_NRF_PRX );
}


void main()
{
    system_init();
    
    while(1)
    {
    
    }
}