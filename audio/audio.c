

unsigned char AudioCH = 0;

void initAudio (void)
{
	TIM6->PSC = 0;
	TIM6->ARR = 500;
	TIM6->DIER |= TIM_DIER_UIE; //разрешаем прерывание от таймера
	TIM6->CR1 |= TIM_CR1_CEN; // Начать отсчёт!
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания

}

void TIM6_IRQHandler(void)
{
	TIM6->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
	if (AudioCH)
		DAC->DHR12L1 = AudioUP;
	else DAC->DHR12L2 = AudioUP;
	DAC->CR |= DAC_CR_EN1;

	AudioUP = sin[i++]; //Запихиваем в ЦАП очередной элемент массива
	if (i==32) i=0; //Если вывели в ЦАП все 32 значения то начинаем заново
}
