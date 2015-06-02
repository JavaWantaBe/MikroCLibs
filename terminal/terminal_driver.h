/**
 * @file terminaldriver.h
 *
 * @brief ANSI Terminal Driver for UART communications
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 22/02/2015
 *
 * @version .01 - Adapted Atmel driver to MicroC compatible
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
 * @par
 *   <all that matters>
 */

#ifndef TERMINAL_DRIVER_H
#define TERMINAL_DRIVER_H

#include <stdint.h>
//***************************
// Misc definitions
//***************************

/*** Text modes ***/
#define MODE_NONE         '0'
#define MODE_BOLD         '1'
#define MODE_DIM          '2'
#define MODE_UNDERLINE    '4'
#define MODE_BLINK        '5'
#define MODE_REVERSED     '7'
#define MODE_CONCEALED    '8'


/*** Text colours ***/
#define COL_FOREGROUND    '3'
#define COL_BACKGROUND    '4'

#define COL_BLACK         '0'
#define COL_RED           '1'
#define COL_GREEN         '2'
#define COL_YELLOW        '3'
#define COL_BLUE          '4'
#define COL_MAGENTA       '5'
#define COL_CYAN          '6'
#define COL_WHITE         '7'


/*** Cursor move direction ***/
#define MOVE_UP           'A'
#define MOVE_DOWN         'B'
#define MOVE_RIGHT        'C'
#define MOVE_LEFT         'D'

//***************************
// Function prototypes
//***************************

void term_initialise( void( *send_c )( uint8_t ), uint8_t( *get_c )( void ) );

uint8_t term_get();

uint8_t term_handle_menu( const uint8_t* menu,
                                uint8_t selectPos,
                                uint8_t top,
                                uint8_t left,
                                uint8_t doubleFrame );
                                
uint8_t term_draw_menu( const uint8_t* menu,
                              uint8_t selectPos,
                              uint8_t top,
                              uint8_t left,
                              uint8_t doubleFrame );
                              
uint16_t term_get_sequence();

uint8_t term_send_flashStr( const uint8_t* str );

uint8_t term_send_RAMStr( uint8_t* str );

void term_draw_frame( uint8_t top,
                      uint8_t left,
                      uint8_t height,
                      uint8_t width,
                      uint8_t doubleFrame );

void term_erase_line();

void term_erase_screen();

void term_erase_screenBottom();

void term_erase_screenTop();

void term_erase_to_end_of_line();

void term_erase_to_start_of_line();

void term_move_cursor( uint8_t distance, uint8_t direction );

void term_print_screen();

void term_restore_cursor_position();

void term_save_cursor_position();

void term_send( uint8_t _data );

void term_send_value_as_digits( uint8_t value );

void term_set_cursor_position( uint8_t row, uint8_t column );

void term_set_display_attribute_mode( uint8_t mode );

void term_set_display_colour( uint8_t fg_bg, uint8_t colour );

void term_set_scroll_mode_all();

void term_set_scroll_mode_limit( uint8_t start, uint8_t end );

#endif