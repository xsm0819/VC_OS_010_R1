//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "dht.h"



// ������� ��� ������ � GPIO
#define DHT_Data_HIGH()       GPIO_SetBits(DHT_Data_Port, DHT_Data_Pin)
#define DHT_Data_LOW()        GPIO_ResetBits(DHT_Data_Port, DHT_Data_Pin)
#define DHT_Data_READ()       GPIO_ReadInputDataBit(DHT_Data_Port, DHT_Data_Pin)

int DHT_BitTimeTreshold = 0;
unsigned char dht_Buff[5];          // ����� ����� �� DHT




//==============================================================================
// ��������� �������������� ��� ���������������� ��� ����/�����
//==============================================================================
void dht_DataPin_init(char IsOutput)
{
  // �������� ������������ ��������� ������
  
  // ����������� ���� GPIO
  GPIO_InitTypeDef InitStruct;
  InitStruct.GPIO_Mode = IsOutput ? GPIO_Mode_Out_OD : GPIO_Mode_IPU;
  InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  InitStruct.GPIO_Pin = DHT_Data_Pin;
  GPIO_Init(DHT_Data_Port, &InitStruct);
}
//==============================================================================

//==============================================================================
// ������� ������� ����� �� ����� Data. ���� IsRising = 0 - ������ ������, �����
// - ��������.
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
// ������� ���������� �� ��� Data ������������ DHT ������� � ���������, ����� ��
// DHT ���� �������������� ������� � �����
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
// ������� ��������� 1 ��� ������ �� DHT
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
// ������� ������� ���� ����������� ����� �� ������ 4 ������ ������ �� DHT
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
// ������� ��������� �� DHT-11 5 ���� ������ � ������ dht_Buff, ��������� 
// ����������� ���������� ������. ���������� ��� ������
//==============================================================================
signed char dht_read_data(void)
{
  for (int i = 0; i < 5; i++)
    dht_Buff[i] = 0;
  
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
    
    if (dht_Buff[4] == dht_chkbyte())
      return DHT_ERR_Ok;
    else
      return DHT_ERR_BadChksum;
  }
  
  return DHT_ERR_NotConnect;
}
//==============================================================================


//==============================================================================
// ������� ���������� �������������� ��������� �������� dht_read_data() �������� 
// ����������� �� DHT-11.
//==============================================================================
float dht_get_temperature(void)
{
#if (DHT_MODEL == DHT22)
  float Val;
  char Val_Buff[2];
  Val_Buff[0] = dht_Buff[3];    // ������� ���� int16 �����
  Val_Buff[1] = dht_Buff[2];    // ������� ���� int16 �����
  Val = (*((uint16_t *)Val_Buff)) & 0x7FFF;
  Val /= 10;
  if (Val_Buff[0] & 0x80)       // �������� ����������� �������������
    Val = -Val;
  return Val;
#else
  return *((signed char *)&dht_Buff[2]);
#endif
}
//==============================================================================


//==============================================================================
// ������� ���������� �������������� ��������� �������� dht_read_data() �������� 
// ��������� �� DHT-11.
//==============================================================================
float dht_get_humidity(void)
{
#if (DHT_MODEL == DHT22)
  float Val;
  char Val_Buff[2];
  Val_Buff[0] = dht_Buff[1];    // ������� ���� int16 �����
  Val_Buff[1] = dht_Buff[0];    // ������� ���� int16 �����
  Val = *((uint16_t *)Val_Buff);
  Val /= 10;
  return Val;
#else
  return *((signed char *)&dht_Buff[0]);
#endif
}
//==============================================================================
DHT_DATA DHT11_RESULT;

void ConvertDHT (void)
{
	if (dht_read_data()==0)
	{
		char temp = dht_get_temperature();

		DHT11_RESULT.TERM[0]=temp/10+0x30;
		DHT11_RESULT.TERM[1]=temp%10+0x30;
		temp = dht_get_humidity();

		DHT11_RESULT.HUMID[0]=temp/10+0x30;
		DHT11_RESULT.HUMID[1]=temp%10+0x30;
	}
}
