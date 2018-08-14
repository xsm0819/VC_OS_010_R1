///////////////////////////////////////////////////
//	File:	Terminal.h
//	Autor:	XSM
//	Date:	07.08.2018
///////////////////////////////////////////////////
#ifndef _TERMINAL_H
#define _TERMINAL_H

extern char PASSWORD[8];

#define CMD_StartSession	"START"
#define CMD_EndSession		"END"
#define CMD_ADMIN			"ADMIN"
#define CMD_USER			"USER"

#define CMD_SENSOR_MP		"SENS_MP"

extern unsigned char Terminal_CMD;

void InitUART_Terminal	(unsigned long BRR);
void Terminal_Send(char chr);
void Terminal_Send_String(char* str);
void Search_Command (void);
void Clear_Buffer (void);

#endif /*	_TERMINAL_H	*/
