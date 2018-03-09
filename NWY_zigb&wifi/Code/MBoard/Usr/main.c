/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

#include "modDetect.h"
#include "UART_dataTransfer.h"
#include "LCD_1.44.h"

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	
	osMemoryInit();
	msgQueueInit();
	
	communicationActive(comObj_DbugP1,ftOBJ_DEBUG);
//	communicationActive(comObj_DataTransP2,ftOBJ_ZIGB);
//	communicationActive(comObj_DataTransP3,ftOBJ_WIFI);
	
	threadModDetect_Active();
	
	LCD144Disp_Active();

  osKernelStart ();                         // start thread execution 
}
