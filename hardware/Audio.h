

#ifndef _AUDIO_H
#define _AUDIO_H

extern unsigned int f_Discret;

#define SPK_Data		DAC->DHR12R2
#define GSM_SPK_Data	DAC->DHR12R1

void InitAudioMP 	(void);
void InitAudioGSM 	(void);
void InitAudioCall 	(void);


#endif /*	_AUDIO_H	*/
