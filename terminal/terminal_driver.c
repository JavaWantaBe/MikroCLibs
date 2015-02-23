#include <stddef.h>
#include "terminal_driver.h"

//***************************
// Macros
//***************************

/*** Send escape sequence start ***/
#define SENDESC            \
    term_send( 27 );       \
    term_send( '[' );

static void( *send_char )( uint8_t c );
static uint8_t( *get_char )( void );

//***************************
// Convert byte to 3 ASCII digits and send
//***************************
void term_send_value_as_digits( uint8_t value )
{
    uint8_t digit;
    
    digit = '0';
    while( value >= 100 )                // Still larger than 100 ?
    {
        digit++;                         // Increment first digit
        value -= 100;
    }
    
    term_send( digit );                  // Send first digit
    
    digit = '0';
    while( value >= 10 )                 // Still larger than 10 ?
    {
        digit++;                         // Increment second digit
        value -= 10;
    }
    
    term_send( digit );                  // Send second digit
    
    term_send( '0' + value );            // Send third digit
}


//***************************
// Initialize UART Terminal
//***************************
void term_initialise( void( *send_c )( uint8_t c ), uint8_t( *get_c )( void ) )
{
    if( send_c == NULL || get_c == NULL )
        return;
    
    send_char = send_c;
    get_char  = get_c;
    
    term_set_display_attribute_mode( MODE_NONE ); // Disable all previous modes
    term_erase_screen();                          // Clear screen
    term_set_cursor_position( 1, 1 );             // Move to top-left corner
}


//****************************
// Transmit one byte
//****************************
void term_send( uint8_t _data )
{
    send_char( _data );                              // Send byte
}


//***************************
// Receive one byte
//***************************
uint8_t term_get()
{
    return get_char();
}


//***************************
// Decode incoming ESC sequence
//***************************
uint16_t term_get_sequence()
{
    uint8_t val;
    uint16_t ret;
    
    val = term_get();
    
    if( val != 27 )                      // Received ESC ?
    {
        ret = val;                       // If not, return char
    }
    else
    {
        val = term_get();
        if( val != '[' )                 // Received '[' ?
        {
            ret = val;                   // If not, return char
        }
        else
        {
            val = term_get();
            
            if( val == 'A' ||
                val == 'B' ||
                val == 'C' ||
                val == 'D' )             // Arrow keys ?
            {
                ret = val << 8;          // Put code in upper byte
            }
            else
            {
                ret = val;               // If not, return char
            }
        }
    }
    return ret;
}


//***************************
// Transmit string from Flash
//
// Returns byte count
//***************************
uint8_t term_send_flashStr( const uint8_t* str )
{
    uint8_t count = 0;            // Initialize byte counter

    while( *str != 0 )                  // Reached zero byte ?
    {
        term_send( *str );
        str++;                          // Increment ptr
        count++;                        // Increment byte counter
    }    
    
    return count;                       // Return byte count
}


//***************************
// Transmit string from SRAM
//
// Returns byte count
//***************************
uint8_t term_send_RAMStr( uint8_t* str )
{
    uint8_t count = 0;            // Initialize byte counter

    while( *str != 0 )                  // Reached zero byte ?
    {
        term_send( *str );
        str++;                          // Increment ptr
        count++;                        // Increment byte counter
    }    
    
    return count;                       // Return byte count
}


//***************************
// Send 'clear bottom of screen' sequence
//***************************
void term_erase_screenBottom()
{
    SENDESC                             // Send escape sequence start
    
    term_send( 'J' );
}


//***************************
// Send 'clear top of screen' sequence
//***************************
void term_erase_screenTop()
{
    SENDESC                             // Send escape sequence start
    
    term_send( '1' );
    term_send( 'J' );
}


//***************************
// Send 'clear screen' sequence
//***************************
void term_erase_screen()
{
    SENDESC                             // Send escape sequence start
    
    term_send( '2' );
    term_send( 'J' );
}


//***************************
// Send 'clear end of line' sequence
//***************************
void term_erase_to_end_of_line()
{
    SENDESC                             // Send escape sequence start
    
    term_send( 'K' );
}


//***************************
// Send 'clear start of line' sequence
//***************************
void term_erase_to_start_of_line()
{
    SENDESC                             // Send escape sequence start
    
    term_send( '1' );
    term_send( 'K' );
}


//***************************
// Send 'clear line' sequence
//***************************
void term_erase_line()
{
    SENDESC                             // Send escape sequence start
    
    term_send( '2' );
    term_send( 'K' );
}


//***************************
// Set text mode
//***************************
void term_set_display_attribute_mode( uint8_t mode )
{
    SENDESC                             // Send escape sequence start
    
    term_send( mode );
    term_send( 'm' );
}


//***************************
// Set text colour
//***************************
void term_set_display_colour( uint8_t fg_bg, uint8_t colour )
{
    SENDESC                             // Send escape sequence start
    
    term_send( fg_bg );                 // Select foreground/background
    term_send( colour );
    term_send( 'm' );
}


//***************************
// Set cursor position
//
// Top-left is (1,1)
//***************************
void term_set_cursor_position( uint8_t row, uint8_t column )
{
    SENDESC                                        // Send escape sequence start
    
    term_send_value_as_digits( row );              // Convert row byte
    term_send( ';' );
    term_send_value_as_digits( column );           // Convert column byte
    term_send( 'H' );
}


//***************************
// Move cursor
//***************************
void term_move_cursor( uint8_t distance, uint8_t direction )
{
    SENDESC                             // Send escape sequence start
    
    term_send_value_as_digits( distance );         // Convert distance byte

    term_send( direction );
}



//***************************
// Save cursor position
//***************************
void term_save_cursor_position()
{
    SENDESC                             // Send escape sequence start
    
    term_send( 's' );
}


//***************************
// Restore cursor position
//***************************
void term_restore_cursor_position()
{
    SENDESC                             // Send escape sequence start
    
    term_send( 'u' );
}


//***************************
// Enable scroll for entire screen
//***************************
void term_set_scroll_mode_all()
{
    SENDESC                             // Send escape sequence start
    
    term_send( 'r' );
}


//***************************
// Limit screen scrolling to some lines only
//***************************
void term_set_scroll_mode_limit( uint8_t start, uint8_t end )
{
    SENDESC                             // Send escape sequence start
    
    term_send_value_as_digits( start );            // Convert start line byte
    term_send( ';' );
    term_send_value_as_digits( end );              // Convert end line byte
    term_send( 'r' );
}


//***************************
// Send 'print screen' command sequence
//***************************
void term_print_screen()
{
    SENDESC                             // Send escape sequence start
   
    term_send( 'i' );
}


//***************************
// Draw frame on terminal client
//***************************
void term_draw_frame( uint8_t top, uint8_t left,
                      uint8_t height, uint8_t width,
                      uint8_t doubleFrame )
{
    /*** Defines egde character codes for both single and double frames ***/
    static const uint8_t edges[] = { 0xda, 0xbf, 0xc0, 0xd9, 0xc4, 0xb3,
                                             0xc9, 0xbb, 0xc8, 0xbc, 0xcd, 0xba };

    uint8_t i = 0;
    height++;
    width++;    
    
    /*** Draw frame ***/
    term_set_cursor_position( top, left );                // Corners first
    term_send( edges[ doubleFrame * 6 + 0 ] );
    
    term_set_cursor_position( top, left + width );
    term_send( edges[ doubleFrame * 6 + 1 ] );
    
    term_set_cursor_position( top + height, left );
    term_send( edges[ doubleFrame * 6 + 2 ] );
    
    term_set_cursor_position( top + height, left + width );
    term_send( edges[ doubleFrame * 6 + 3 ] );
    
    for( i = left + 1; i < left + width; i++ )           // Top and bottom edges
    {
        term_set_cursor_position( top, i );
        term_send( edges[ doubleFrame * 6 + 4 ] );
        
        term_set_cursor_position( top + height, i );
        term_send( edges[ doubleFrame * 6 + 4 ] );
    }
    
    for( i = top + 1; i < top + height; i++ )            // Left and right edges
    {
        term_set_cursor_position( i, left );
        term_send( edges[ doubleFrame * 6 + 5 ] );
        
        term_set_cursor_position( i, left + width );
        term_send( edges[ doubleFrame * 6 + 5 ] );
    }
}


//***************************
// Draw menu on terminal client
//***************************
uint8_t term_draw_menu( const uint8_t* menu, uint8_t selectPos,
                              uint8_t top, uint8_t left, uint8_t doubleFrame )
{
    uint8_t i, width, height;
    const uint8_t* ptr = menu;
    
    width = height = 0;
    
    while( *ptr != 0 )                          // Scan through menu string
    {
        height++;                               // Keep track of item count

        if( selectPos == height )
        {
            term_set_display_attribute_mode( MODE_REVERSED );  // Reverse selected item
        }
        else
        {
            term_set_display_attribute_mode( MODE_NONE );
        }
    
        term_set_cursor_position( top + height, left + 1 );   // Start of item text
        
        i = 0;
        
        while( *ptr != '\n' )                   // Scan through item text
        {
            term_send( *ptr );                  // Print item character
            i++;                                // Item character count
            ptr++;
        }
        
        ptr++;                                  // Skip newline character
        
        if( i > width )
            width = i;                          // Always keep max width
    }
    
    term_set_display_attribute_mode( MODE_NONE );
    if( selectPos == 0 )                        // Draw frame only if selectPos == 0
    {
        term_draw_frame( top, left, height, width, doubleFrame );
    }
    
    term_set_cursor_position( top + selectPos, left + 1 );    // Postition at start of selected item
    return height;
}


//***************************
// Handle menu
//***************************
uint8_t term_handle_menu( const uint8_t* menu, uint8_t selectPos,
                                uint8_t top, uint8_t left, uint8_t doubleFrame )
{
    uint8_t height;
    uint16_t ret;
        
    /* Print menu frame */
    height = term_draw_menu( menu, 0, top, left, doubleFrame );
    
    while(1)
    {
        /* Print menu text with selected item reversed */
        term_draw_menu( menu, selectPos, top, left, doubleFrame );
        
        ret = term_get_sequence();             // Decode ESC sequence
        
        if( ret == 13 )                        // Exit on ENTER
            return selectPos;
            
        if( (ret & 0xff) == 0 )                // Arrow key ?
        {
            ret >>= 8;
            
            if( ret == MOVE_UP )
            {
                selectPos--;                   // Move selection up, with wrap
                if( selectPos < 1 )
                    selectPos = height;
            } else if( ret == MOVE_DOWN )
            {
                selectPos++;                   // Move selection down, with wrap
                if( selectPos > height )
                    selectPos = 1;
            }
        }
    }
}