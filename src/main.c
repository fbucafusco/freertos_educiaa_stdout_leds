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

/*==================[inclusiones]============================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#include "stdout_leds.h"
#include "sapi.h"

/*==================[definiciones y macros]==================================*/
#define LED_RATE_MS 50

#define LED_RATE        pdMS_TO_TICKS(LED_RATE_MS)


#define MODE 3

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void heart_beat( void* taskParmPtr ); // Prendo A - Apago A

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------

    boardConfig();									// Inicializar y configurar la plataforma

    debugPrintConfigUart( UART_USB, 250000 );		// UART for debug messages

    // Crear tarea en freeRTOS
    BaseType_t res =
        xTaskCreate(
            heart_beat,                     	// Funcion de la tarea a ejecutar
            ( const char * )"heart_beat",   	// Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
            0,                          		// Parametros de tarea
            tskIDLE_PRIORITY+1,         		// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            0                          			// Puntero a la tarea creada en el sistema
        );

    // Gestion de errores

    configASSERT( res == pdPASS ); // gestion de errores

    // Iniciar scheduler
    vTaskStartScheduler(); // Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    //while( TRUE )
    //{
    // Si cae en este while 1 significa que no pudo iniciar el scheduler
    //}
    configASSERT( 0 );

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.


    return TRUE;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void heart_beat( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------

    uint32_t led  = 0;

    stdout_leds_init( );

    stdout_leds_set_color( 0, ANSI_RED );
    stdout_leds_set_color( 1, ANSI_GREEN );
    stdout_leds_set_color( 2, ANSI_YELLOW );
    stdout_leds_set_color( 3, ANSI_BLUE );
    stdout_leds_set_color( 4, ANSI_MAGENTA );
    stdout_leds_set_color( 5, ANSI_CYAN );
    stdout_leds_set_color( 6, ANSI_LIGHT_BLUE );
    stdout_leds_set_color( 7, VERDECITO );
    stdout_leds_set_color( 8, RED_196 );
    stdout_leds_set_color( 9, VIOLET_128 );

    /* the res of the leds, are red (default color)*/

    TickType_t base_ini = xTaskGetTickCount();
    TickType_t base  = base_ini;
    TickType_t diff;
    TickType_t xLastWakeTime = base_ini;

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        /* toggle the led */
        stdout_leds_toggle( led );

        vTaskDelayUntil( &xLastWakeTime, LED_RATE );

        /* increment and wrap arround */
        led ++ ;
        if( led == NRO_LEDS )
        {
            led = 0;
        }

        /* 1 second counter */
        diff = xTaskGetTickCount()-base;
        if( diff>1000 )
        {
            stdout_printf( "%s sec: %03u ", RED_196, ( xTaskGetTickCount()-base_ini ) /1000 ) ;
            base = xTaskGetTickCount();
        }
    }
}


/*==================[fin del archivo]========================================*/
