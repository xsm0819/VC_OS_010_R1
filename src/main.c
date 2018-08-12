/* Standard includes. */
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Device.h"


#define StartDHT11Conversion xTaskCreate( vTaskReadDHT11, ( signed char * ) "Sensor DHT11 result", configMINIMAL_STACK_SIZE, NULL, 2,( xTaskHandle * ) NULL);

//--------------------------------------------------------------
PinState IOState;

void vTaskRW_IO(void *pvParameters);

void vTaskReadDHT11(void *pvParameters);



//--------------------------------------------------------------
int main(void) {

	prvSetupHardware();
	xTaskCreate( vTaskRW_IO, ( signed char * ) "Get and Set IO pins", configMINIMAL_STACK_SIZE, NULL, 2,
			( xTaskHandle * ) NULL);
	xTaskCreate( vTaskReadDHT11, ( signed char * ) "Sensor DHT11 result", configMINIMAL_STACK_SIZE, NULL, 2,
			( xTaskHandle * ) NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task.  The idle task is created within vTaskStartScheduler(). */
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {

//	TimingDelay_Decrement();
}
/*-----------------------------------------------------------*/
