#include "Device.h"
#include "ssd1306_i2c.h"
#include "ssd1306.h"
#include "fonts.h"
#include "ico.h"
#include "dht.h"
#include "main.h"

PinState IOState;
StatusBar State;

void StartIcons (void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	for (unsigned char i = 0; i < 70; i+=4)
	{
	SSD1306_GotoXY(0,3);
	SSD1306_Puts("LOADING", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine ((58+i), 2, (58+i), 12, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine ((59+i), 2, (59+i), 12, SSD1306_COLOR_WHITE);
	PrintIcon (wel1_ICO, wel1WP, wel1HP, 0, 17, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
	millis = 0;
	while (millis < 150);
	}
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	for (unsigned char i = 10; i < 70; i+=4)
	{
	SSD1306_GotoXY(0,3);
	SSD1306_Puts("STARTING", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine ((58+i), 2, (58+i), 12, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine ((59+i), 2, (59+i), 12, SSD1306_COLOR_WHITE);
	PrintIcon (wel2_ICO, wel2WP, wel2HP, 0, 17, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
	millis = 0;
	while (millis < 150);
	}
}

void SetupHardware( void )
{
	SetSysClock();
	EnableClockPeriph();
	GPIO_Initialise();
	SysTick_Config(SystemCoreClock/1000);
	ssd1306_I2C_Init();
	State.ENetStatus = 2;
	IOState.GSM_ETH_EN = DISABLE;
	IOState.SPK_EN = DISABLE;
	RW_IO();
	SSD1306_Init();
	ConvertDHT();
	InitUART_Terminal(57600);
	StartIcons();
}

unsigned long millis = 0, sec = 0, timLCD = 0;
void SysTick_Handler(void)
{
	millis ++;
	timLCD++;
	if (millis > 999)
	{
		millis = 0;
		sec++;
	}
	if (timLCD > 100)
	{
		timLCD = 0;
		if (Windows != WND_Starting) Print();
	}
	if (sec > 10)
	{
		ConvertDHT();
	}
}

void GPIO_Initialise (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_SetBits(GPIOC, GPIO_Pin_7);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_3|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_10|GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

}


unsigned long Div, Mul;

void Calc_PLL	(void)
{
	Div = 2;
	Mul = (F_CPU/F_HSE)*Div;
	if (Mul > 16)
	{
		Div = 1;
		Mul = (F_CPU/F_HSE)*Div;
	}

	if ((Mul > 16)||(Mul < 2))
	{

	}
	else
	{
		if (Div == 1)
		{
			Div = RCC_PLLSource_HSE_Div1;
		}
		else if (Div == 2)
		{
			Div = RCC_PLLSource_HSE_Div2;
		}

		switch (Mul)
		{
			case 2:
				Mul = RCC_PLLMul_2;
			break;

			case 3:
				Mul = RCC_PLLMul_3;
			break;

			case 4:
				Mul = RCC_PLLMul_4;
			break;

			case 5:
				Mul = RCC_PLLMul_5;
			break;

			case 6:
				Mul = RCC_PLLMul_6;
			break;

			case 7:
				Mul = RCC_PLLMul_7;
			break;

			case 8:
				Mul = RCC_PLLMul_8;
			break;

			case 9:
				Mul = RCC_PLLMul_9;
			break;

			case 10:
				Mul = RCC_PLLMul_10;
			break;

			case 11:
				Mul = RCC_PLLMul_11;
			break;

			case 12:
				Mul = RCC_PLLMul_12;
			break;

			case 13:
				Mul = RCC_PLLMul_13;
			break;

			case 14:
				Mul = RCC_PLLMul_14;
			break;

			case 15:
				Mul = RCC_PLLMul_15;
			break;

			case 16:
				Mul = RCC_PLLMul_16;
			break;
			default:	while (1);
		}
	}
}


void SetSysClock(void)
{
	Calc_PLL();
    ErrorStatus HSEStartUpStatus;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        //FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        //FLASH_SetLatency( FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);


        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(Div, Mul);



        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}

void EnableClockPeriph (void)
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
		    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

}

void RW_IO(void)
{
		rel1(IOState.RELAY1);
		IOState.RELAY1 = rel1State;
		rel2(IOState.RELAY2);
		IOState.RELAY2 = rel2State;
		en_gsm_eth(IOState.GSM_ETH_EN);
		en_spk(IOState.SPK_EN);

		IOState.C869 = Ci869;
		IOState.C02 = Ci02;
		IOState.Line1 = L1;
		IOState.Line2 = L2;
		IOState.C120 = Ci120;
		IOState.C130 = Ci130;
		IOState.C140 = Ci140;
		IOState.C100 = Ci100;
		IOState.DoorMP = DOOR_mp;
		IOState.Line3 = L3;
}

DHT_DATA DHT11_RESULT;

void Print (void)
{
	//SSD1306_Init();
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	PrintIcon (term_ICO, t_icoWP, t_icoHP, -3, 19, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY (0,25);
	SSD1306_Puts(DHT11_RESULT.TERM, &Font_7x10, SSD1306_COLOR_WHITE);
	PrintIcon (h_ICO, h_icoWP, h_icoHP, 0, 42, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY (0,51);
	SSD1306_Puts(DHT11_RESULT.HUMID, &Font_7x10, SSD1306_COLOR_WHITE);

	PrintIcon (l1_ICO, l1WP, l1HP, 27, 17, !IOState.Line1);
	PrintIcon (l2_ICO, l1WP, l1HP, 27, 33, !IOState.Line2);
	PrintIcon (l3_ICO, l1WP, l1HP, 27, 49, !IOState.Line3);

	PrintIcon (bs_ICO, bsWP, bsHP, 48, 17, !IOState.C120);
	PrintIcon (ds_ICO, bsWP, bsHP, 48, 33, !IOState.C130);
	PrintIcon (dk_ICO, bsWP, bsHP, 48, 49, !IOState.C140);


	PrintIcon (vCC_ICO, vCCWP, vCCHP, 74, 17, !IOState.C100);
	PrintIcon (serv_ICO, servWP, servHP, 74, 33, !IOState.C02);
	PrintIcon (iM_ICO, iMWP, iMHP, 74, 49, !IOState.C869);

	PrintIcon (k1_ICO, k1WP, k1HP, 102, 17, !IOState.RELAY1);
	PrintIcon (k2_ICO, k2WP, k2HP, 102, 33, !IOState.RELAY2);
	PrintIcon (dmp_ICO, dmpWP, dmpHP, 102, 49, !IOState.DoorMP);


	if (State.connectTyp == Signal0)
	{
		PrintIcon (sign0_ICO, sign0WP, sign0HP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == Signal1)
	{
		PrintIcon (sign1_ICO, sign1WP, sign1HP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == Signal2)
	{
		PrintIcon (sign2_ICO, sign2WP, sign2HP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == Signal3)
	{
		PrintIcon (sign3_ICO, sign3WP, sign3HP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == Signal4)
	{
		PrintIcon (sign4_ICO, sign4WP, sign4HP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == LAN)
	{
		PrintIcon (lan_ICO, lanHP, lanWP, 1, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.connectTyp == NotLan)
	{
		PrintIcon (lan2_ICO, lan2WP, lan2HP, 1, 0, SSD1306_COLOR_WHITE);
	}

	if (IOState.SPK_EN)
	{
		PrintIcon (spk_ICO, spkWP, spkHP, 35, 0, SSD1306_COLOR_WHITE);
	}
	/*else if (State.ENetStatus == NotE_NET)
	{
		PrintIcon (eth_ERR_ICO, eth_ERRWP, eth_ERRHP, 35, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.ENetStatus == NET_OK)
	{
		PrintIcon (eth_OK_ICO, eth_OKWP, eth_OKHP, 35, 0, SSD1306_COLOR_WHITE);
	}*/
	else if (State.ENetStatus == ActiveAudioCab)
	{
		PrintIcon (call_ICO, callWP, callHP, 35, 0, SSD1306_COLOR_WHITE);
	}
	else if (State.ENetStatus == SystemMessedg)
	{
		PrintIcon (sms_ICO, smsWP, smsHP, 35, 0, SSD1306_COLOR_WHITE);
	}
	SSD1306_UpdateScreen();
}
