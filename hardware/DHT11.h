///////////////////////////////////////////////////
//	File:	dht11.h
//	Autor:	XSM
//	Date:	09.08.2018
///////////////////////////////////////////////////
#ifndef _DHT_H
#define _DHT_H

typedef struct
{
	char TERM[4];
	char HUMID[4];
}DHT_DATA;

// ������ �������
#define DHT11                   11
#define DHT22                   22      // AM2302
#define DHT_MODEL               DHT11   // ������ ������� (DHT11 ��� DHT22)

// ���������, ������������ ������ ��� ������ � DHT
#define DHT_ERR_Ok              0
#define DHT_ERR_NotConnect      -1
#define DHT_ERR_BadChksum       -2

#define DHT_WaitMaxTicks        1000    // ���������� ������ GPIO, ����� �������� �������, ��� ����� �����������

// ����, ����������� ����������� dht-11/21/22
#define DHT_Data_Port         GPIOB
#define DHT_Data_Pin          GPIO_Pin_2



// ������� ��������� �� DHT-11 5 ���� ������ � ������ dht_Buff, ��������� ����������� ���������� ������. ���������� ��� ������
signed char dht_read_data(void);
// ������� ���������� �������������� ��������� �������� dht_read_data() �������� ����������� �� DHT-11.
float dht_get_temperature(void);
// ������� ���������� �������������� ��������� �������� dht_read_data() �������� ��������� �� DHT-11.
float dht_get_humidity(void);

#endif
