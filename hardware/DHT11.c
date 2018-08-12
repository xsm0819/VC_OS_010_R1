#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "delay.h"
#include "DHT11.h"
#include "misc.h"


// Макросы для работы с GPIO
#define DHT_Data_HIGH()       GPIO_SetBits(DHT_Data_Port, DHT_Data_Pin)
#define DHT_Data_LOW()        GPIO_ResetBits(DHT_Data_Port, DHT_Data_Pin)
#define DHT_Data_READ()       GPIO_ReadInputDataBit(DHT_Data_Port, DHT_Data_Pin)

int DHT_BitTimeTreshold = 0;
unsigned char dht_Buff[5];          // Буфер приёма из DHT

DHT_DATA DHT11_RESULT;
void vTaskReadDHT11(void *pvParameters)
{
	while(1)
	{
		if (dht_read_data()==0)
		{
			sprintf(DHT11_RESULT.TERM, "%d*\n", dht_get_temperature());
			sprintf(DHT11_RESULT.HUMID, "%d*\n", dht_get_humidity());
			vTaskDelay(30000);
		}
	}
}

//==============================================================================
// Процедура инициализирует пин микроконтроллера как вход/выход
//==============================================================================
void dht_DataPin_init(char IsOutput)
{
  // Стартуем тактирование периферии портов

  // Настраиваем ногу GPIO
  GPIO_InitTypeDef InitStruct;
  InitStruct.GPIO_Mode = IsOutput ? GPIO_Mode_Out_OD : GPIO_Mode_IPU;
  InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  InitStruct.GPIO_Pin = DHT_Data_Pin;
  GPIO_Init(DHT_Data_Port, &InitStruct);
}
//==============================================================================

//==============================================================================
// Функция ожидает фронт на линии Data. Если IsRising = 0 - фпронт задний, иначе
// - передний.
//==============================================================================
int dht_waitfront(char IsRising)
{
  int tick = 0;

  if (IsRising)
  {
    while ((!DHT_Data_READ()) && (tick < DHT_WaitMaxTicks))
      tick++;
  }
  else
  {
    while ((DHT_Data_READ()) && (tick < DHT_WaitMaxTicks))
      tick++;
  }

  return tick;
}
//==============================================================================


//==============================================================================
// Функция выставляет на пин Data пробуждающий DHT импульс и проверяет, выдал ли
// DHT свой подтверждающиц импульс в ответ
//==============================================================================
signed char dht_request(void)
{
  dht_DataPin_init(1);

  DHT_Data_LOW();
  delay_ms(22);
  DHT_Data_HIGH();

  dht_DataPin_init(0);

  dht_waitfront(0);
  int ticks1 = dht_waitfront(1);
  int ticks2 = dht_waitfront(0);

  if (ticks1 && ticks2 && (ticks1 < DHT_WaitMaxTicks) && (ticks2 < DHT_WaitMaxTicks))
  {
    DHT_BitTimeTreshold = ticks1 >> 1;
    return DHT_ERR_NotConnect;
  }

  return DHT_ERR_Ok;
}
//==============================================================================


//==============================================================================
// Функция считывает 1 бит данных из DHT
//==============================================================================
char dht_readbit(void)
{
  dht_waitfront(1);
  int ticks = dht_waitfront(0);

  if (ticks && (ticks < DHT_WaitMaxTicks))
    return (ticks > DHT_BitTimeTreshold) ? 1 : 0;

  return 0;
}
//==============================================================================


//==============================================================================
// Функция считает байт контрольной суммы по первым 4 байтам пакета из DHT
//==============================================================================
unsigned char dht_chkbyte(void)
{
  unsigned char ChkByte = 0;

  for (int i = 0; i < 4; i++)
    ChkByte += dht_Buff[i];

  return ChkByte;
}
//==============================================================================


//==============================================================================
// Функция считывает из DHT-11 5 байт данных в массив dht_Buff, проверяет
// целостность считанного пакета. Возвращает код ошибки
//==============================================================================
signed char dht_read_data(void)
{
  for (int i = 0; i < 5; i++)
    dht_Buff[i] = 0;
  __disable_irq();
  if (dht_request())
  {
    for (int i = 0; i < 40; i++)
    {
      if (dht_readbit())
      {
        char bitnum = 7 - (i % 8);
        dht_Buff[i >> 3] |= (1 << bitnum);
      }
    }
    __enable_irq();
    if (dht_Buff[4] == dht_chkbyte())
      return DHT_ERR_Ok;
    else
      return DHT_ERR_BadChksum;
  }

  return DHT_ERR_NotConnect;
}
//==============================================================================


//==============================================================================
// Функция возвращает предварительно считанное функцией dht_read_data() значение
// температуры из DHT-11.
//==============================================================================
float dht_get_temperature(void)
{
#if (DHT_MODEL == DHT22)
  float Val;
  char Val_Buff[2];
  Val_Buff[0] = dht_Buff[3];    // Младший байт int16 слова
  Val_Buff[1] = dht_Buff[2];    // Старший байт int16 слова
  Val = (*((uint16_t *)Val_Buff)) & 0x7FFF;
  Val /= 10;
  if (Val_Buff[0] & 0x80)       // Значение температуры отрицательное
    Val = -Val;
  return Val;
#else
  return *((signed char *)&dht_Buff[2]);
#endif
}
//==============================================================================


//==============================================================================
// Функция возвращает предварительно считанное функцией dht_read_data() значение
// влажности из DHT-11.
//==============================================================================
float dht_get_humidity(void)
{
#if (DHT_MODEL == DHT22)
  float Val;
  char Val_Buff[2];
  Val_Buff[0] = dht_Buff[1];    // Младший байт int16 слова
  Val_Buff[1] = dht_Buff[0];    // Старший байт int16 слова
  Val = *((uint16_t *)Val_Buff);
  Val /= 10;
  return Val;
#else
  return *((signed char *)&dht_Buff[0]);
#endif
}
//==============================================================================
