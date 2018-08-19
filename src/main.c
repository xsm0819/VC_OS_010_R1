/* Standard includes. */
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "Device.h"
#include "Terminal.h"
#include "main.h"


//--------------------------------------------------------------
int main(void)
{
	while (1)
	{
		WorkProc();
	}
}


void WorkProc (void)
{
	switch(Windows)
	{
		case WND_Null:
		{
			Windows = WND_Starting;
		}break;

		case WND_Starting:
		{
			SetupHardware();
			Windows = WND_ReadIO;
		}break;

		case WND_ReadIO:
		{
			RW_IO();
			Windows = WND_SearchEvent;
		}break;


		case WND_GSM_Event:
		{

		}break;

		case WND_ETH_Event:
		{

		}break;

		case WND_ScanButton:
		{

		}break;

		case WND_Calling:
		{

		}break;

		case WND_ScanCall:
		{

		}break;

		case WND_ScanSensor:
		{

		}break;

		case WND_NevSettings:
		{

		}break;

		case WND_SearchEvent:
		{
			Windows = WND_ReadIO;
		}break;
	}
}
