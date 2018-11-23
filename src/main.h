static unsigned char Windows = 0;
extern char LCD_REFRESH;

//////////	windows
#define WND_Null		0
#define WND_Starting	1
#define WND_ReadIO		2
#define WND_ReadDHT		3
#define WND_GSM_Event	4
#define WND_ETH_Event	5
#define WND_ScanButton	6
#define WND_Calling		7
#define WND_ScanCall	8
#define WND_ScanSensor	9
#define WND_NevSettings	10
#define WND_SearchEvent	11

void WorkProc (void);
