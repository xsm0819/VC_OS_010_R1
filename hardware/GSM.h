

void InitUART_GSM (unsigned long BRR);
void GSM_Send(char chr);
void GSM_Send_String(char* str);

#define CMD_Null						"AT\n"
#define Autoanswer						"ATS0=0\n"
#define InputCall							"+CLIP"
#define DTMF_Code						"DTMF"
#define BUSY								"BUSY"
#define NO_ANSWER					"NO ANSWER"
#define NO_CARRIER					"NO CARRIER"
#define OutCall								"ATD"
#define Anslaver							"ATA\n"
#define EndCall								"ATH0\n"
#define OK										"OK"
#define ERROR_code					"ERROR"
#define VeryfyInternetData		"+HTTPACTION: 0,200"
#define ReadInternetData			"+HTTPREAD"

#define	CMD_SMS_TextMode				"AT+CMGF=1\n"
#define	CMD_SMS_TextModeGSM		"AT+CSCS=\"GSM\"\n"
#define	CMD_SpecialSMS_Disable		"AT+CSCB=1\n"
#define	CMD_AutoDetectionNumb		"AT+CLIP=1\n"
#define	CMD_DTMF_Enable					"AT+DDET=1\n"

#define	CMD_OperatorInfo					"AT+COPS?\n"
#define	CMD_StatusInfo						"AT+CPAS\n"
#define	CMD_LevelSignal						"AT+CSQ\n"
#define	CMD_DellAllSMS						"AT+CMGD=1,4\n"
#define	CMD_SendSMS							"AT+CMGS=\""
#define	CMD_ReadSMS							"AT+CMGR"
#define	CMD_InputSMS						"+CMTI:"
#define  CMD_SendDTMF						"AT+VTS="
#define	CMD_EhoDisable						"ATE0\n"
#define  CMD_ReadCharge 					"AT+CBC"
#define  CMD_TimDTMF							"AT+VTD=2\n"

#define CMD_MaxVolume						"AT+CLVL = 20\n"
#define CMD_Volume								"AT+CLVL = "

#define CMD_MaxVolumeMic				"AT+CMIC = 0, 15\n"
#define CMD_VolumeMic(x)					"AT+CMIC = 0, "

#define TypInternetConnecting			"AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
#define SettAPN										"AT+SAPBR=3,1,\"APN\","
#define InitGPRS_Session						"AT+SAPBR=1,1\n"
#define InitHTTP_Session						"AT+HTTPINIT\n"

#define SettURL											"AT+HTTPPARA=\"URL\","
#define GetURL												"AT+HTTPACTION=0\n"
#define ReadInternetData							"AT+HTTPREAD = 0,65\n"
#define HTTP_Page								"+HTTPACTION:"
#define ValidHTTP_Page								"+HTTPACTION: 0,200,"

#define StatusGPRS_Session						"AT+SAPBR=2,1\n"
#define DeInitGPRS_Session						"AT+SAPBR=0,1\n"
#define DeInitHTTP_Session						"AT+HTTPTERM\n"
#define StatusGPRS_OK								"+SAPBR: 1,1"
#define StatusGPRS_ERR								"+SAPBR: 1,3"
#define ReloadGSM								"AT+CFUN = 1,1\n"

//	Sett Output
// all incoming command codes start with "CH"
#define CH1													"CH1"
#define CH2													"CH2"
#define CH3													"CH3"

#define ON													"ON"
#define OFF													"OFF"
#define RESET_CH												"RST"

//	Sett new parameter
// all incoming command codes start with "SETT"

#define NevSettigs										"SETT"
#define Location_ODS											"LOCAT"
#define ID_ODS													    "ID_"
#define SaveUser1												"U1"
#define SaveUser2												"U2"
#define SaveUser3												"U3"

#define SaveAdmin										"ADM"
#define SettNevAPN									"APN"
#define SettNevURL										"URL"

#define SendToSIM_Module						"SIM_CMD"
#define ClearAllSettigs								"Reset_all_settings!"

extern unsigned char CallStatus, CallEN, GsmTermSession;
typedef struct
{
	unsigned int PA	 	:1;
	unsigned int PB  		:1;
	unsigned int PC  		:1;
	unsigned int C120  	:1;
	unsigned int C130  	:1;
	unsigned int C140  	:1;
	unsigned int CH1S  	:1;
	unsigned int CH2S  	:1;
	unsigned int CH3S  	:1;
	unsigned int IMS  	:1;
}  OutBuffer;


unsigned char GSM_Anslaver (void);
void InputCalling (void);
void InitGSM (void);
