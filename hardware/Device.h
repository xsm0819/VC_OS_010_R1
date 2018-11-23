#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "inttypes.h"
//----------RCC-------------//
#define F_HSE	8000000		//MHz
#define F_CPU	72000000UL

#define F_APB1	F_CPU/2
#define F_APB2	F_CPU

//--------GPIO-------------//
#define	Ci869			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define	Ci02			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
#define	en_gsm_eth(x)	GPIO_WriteBit(GPIOA, GPIO_Pin_8, !(x))
#define	RST_eth(x)		GPIO_WriteBit(GPIOC, GPIO_Pin_7, (x))

#define	rel1State		!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)
#define	rel2State		!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12)
#define	rel1(x)			GPIO_WriteBit(GPIOA, GPIO_Pin_11, !(x))
#define	rel2(x)			GPIO_WriteBit(GPIOA, GPIO_Pin_12, !(x))
#define	DIRECT_call(x)	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (x))
#define	DIRECT_ext(x)	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (x))

#define	sensor_dht11	GPIOB, GPIO_Pin_2

#define	L1				GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define	L2				GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define	cs_eth(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_12, (x))
#define	Ci120			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define	Ci130			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define	Ci140			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)
#define	Ci100			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)
#define	DOOR_mp			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define	en_spk(x)		GPIO_WriteBit(GPIOC, GPIO_Pin_9, !(x))
#define	L3				GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

#define	B1				GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)
#define	B2				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)
#define	B3				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14)

#define	RST_SD(x)		GPIO_WriteBit(GPIOC, GPIO_Pin_5, !(x))
#define	CS_SD(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_15, !(x))

#define USERS			1
#define ADMIN			5


#define CH1_WRITE		2
#define CH2_WRITE		3
#define PRINT_LOG		4

#define NormalStateO1	6
#define NormalStateO2	7
#define GSM_APN			8
#define GSM_USER		9
#define GSM_PASS		10
#define TimReflesh		11
#define TimeDate		12
#define PASSWOR 		13


//----struct for state IO pins-----//
typedef struct
{
	//OUTPUT
	unsigned char RELAY1;
	unsigned char RELAY2;
	unsigned char GSM_ETH_EN;
	unsigned char SPK_EN;

	//INPUT
	unsigned char C120;
	unsigned char C130;
	unsigned char C140;
	unsigned char Line1;
	unsigned char Line2;
	unsigned char Line3;
	unsigned char C100;
	unsigned char C02;
	unsigned char C869;
	unsigned char DoorMP;
} PinState;

static uint32_t TimDelay1, TimDelay2, TimDelay3;

#define Signal0	0
#define Signal1	1
#define Signal2	2
#define Signal3	3
#define Signal4	4
#define LAN		5
#define NotLan	6

#define ActiveAudioMP	0
#define ActiveAudioCab	1
#define SystemMessedg	2
#define NotE_NET		3
#define NET_OK			4


typedef struct
{
	unsigned char connectTyp;
	unsigned char ENetStatus;
	unsigned char Time[8];

} StatusBar;

extern unsigned long millis;

//----------RTOS------------//
void SetupHardware( void );


//----------GPIO------------//
void GPIO_Initialise (void);
//----------RCC------------//
void SetSysClock(void);
void Calc_PLL	(void);
void EnableClockPeriph (void);
void RW_IO(void) ;
void Print (void);
