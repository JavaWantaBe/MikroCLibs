/**
 *  @file lwprintf.h
 *
 *
 *  @code void putchar (char c)
 *  {
 *      while (!SERIAL_PORT_EMPTY) ;
 *      SERIAL_PORT_TX_REGISTER = c;
 *  }
 */

#ifndef LWPRINTF_H
#define LWPRINTF_H

#include <stdarg.h>

void tfp_printf( char* fmt, ... );
void printfInit( void ( *printFunction )( char c ) );

#define printf tfp_printf

#endif