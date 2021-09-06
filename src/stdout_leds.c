/**
BSD 3-Clause License

Copyright (c) 2021, Franco BUcafusco
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdarg.h>
#include "stdout_leds.h"


#define ANSI_GRAY   "\033[38;5;255m"

typedef struct stdout_leds_c_
{
    uint8_t state;
    char*  color;
} stdout_leds_c_t;


static stdout_leds_c_t led_cfg[NRO_LEDS];


#define clear() printf("\033[H\033[J")

/* ubica en el screen de la terminal de manera absoluta el cursor */
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


void hide_cursor()
{
    printf( "\e[?25l" );
}

void draw_frame()
{
    printf( ANSI_GRAY );
    int x = 1;
    int y = 1;

    for( int i = 0; i<NRO_LEDS; i++ )
    {
        for( int w = 0; w<3; w++ )
        {
            for( int h = 0; h<3; h++ )
            {
                gotoxy( x,y );

                if ( i == 0 )
                {
                    if( w==0 && h==0 )
                    {
                        printf( "%c", 218 ) ;
                    }
                    else if( w==0 && h==2 )
                    {
                        printf( "%c", 192 ) ;
                    }
                    else if( w==2 && h==0 )
                    {
                        printf( "%c", 194 ) ;
                    }
                    else if( w==2 && h==2 )
                    {
                        printf( "%c", 193 ) ;
                    }
                    else if( w==0||w==2 )
                    {
                        printf( "%c", 179 ) ;
                    }
                    else if( h==0||h==2 )
                    {
                        printf( "%c", 196 ) ;
                    }
                }
                else if ( i == NRO_LEDS-1 )
                {
                    if( w==0 && h==0 )
                    {
                        printf( "%c", 194 ) ;
                    }
                    else if( w==0 && h==2 )
                    {
                        printf( "%c", 193 ) ;
                    }
                    else if( w==2 && h==0 )
                    {
                        printf( "%c", 191 ) ;
                    }
                    else if( w==2 && h==2 )
                    {
                        printf( "%c", 217 ) ;
                    }
                    else if( w==0||w==2 )
                    {
                        printf( "%c", 179 ) ;
                    }
                    else if( h==0||h==2 )
                    {
                        printf( "%c", 196 ) ;
                    }
                }
                else
                {

                    if( w==0 && h==0 )
                    {
                        printf( "%c", 194 ) ;
                    }
                    else if( w==0 && h==2 )
                    {
                        printf( "%c", 193 ) ;
                    }
                    else if( w==2 && h==0 )
                    {
                        printf( "%c", 194 ) ;
                    }
                    else if( w==2 && h==2 )
                    {
                        printf( "%c", 193 ) ;
                    }
                    else if( w==0||w==2 )
                    {
                        printf( "%c", 179 ) ;
                    }
                    else if( h==0||h==2 )
                    {
                        printf( "%c", 196 ) ;
                    }
                }

                y++;
            }
            x++;
            y = 1;
        }
        x--;
    }
}

void stdout_led_update( uint32_t hnd )
{
    gotoxy( hnd*2 + 2, 2 );
    if( led_cfg[hnd].state )
    {
        printf( led_cfg[hnd].color );
        printf( "%c",178 ) ;
    }
    else
    {
        printf( "%c",' ' ) ;
    }

    draw_frame();
    fflush( stdout );
}
//deprecated
void stdout_leds_update()
{
    gotoxy( 1,1 );

    for( int i = 0; i<NRO_LEDS; i++ )
    {
        if( led_cfg[i].state )
        {
            printf( led_cfg[i].color );
            printf( "%c",178 ) ;
        }
        else
        {
            printf( "%c",' ' ) ;
        }
    }
    //draw_frame();
    fflush( stdout );
}

void stdout_leds_init( )
{
    clear() ;
    hide_cursor();
    for( int i = 0; i<NRO_LEDS; i++ )
    {
        led_cfg[i].state = 0;
        led_cfg[i].color = RED_196;

        stdout_led_update( i );
    }

    //stdout_leds_update();
}

void stdout_leds_set_color( uint32_t hnd, char* color )
{
    led_cfg[hnd].color =color;
}

void stdout_leds_set( uint32_t hnd, uint32_t state )
{
    if( state )
    {
        led_cfg[hnd].state = 1;
    }
    else
    {
        led_cfg[hnd].state = 0;
    }
    stdout_led_update( hnd );
    // stdout_leds_update();
}

void stdout_leds_toggle( uint32_t hnd   )
{
    led_cfg[hnd].state = 1- led_cfg[hnd].state;
    stdout_led_update( hnd );
    //stdout_leds_update();
}

void stdout_printf( char* format, ... )
{
    gotoxy( 1, 4 );
    va_list valist;

    /* initialize valist for num number of arguments */
    va_start( valist, format );

    vprintf( format, valist );

    /* clean memory reserved for valist */
    va_end( valist );
    fflush( stdout );
}