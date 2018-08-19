

unsigned char AudioCH = 0;

void initAudio (void)
{
	TIM6->PSC = 0;
	TIM6->ARR = 500;
	TIM6->DIER |= TIM_DIER_UIE; //��������� ���������� �� �������
	TIM6->CR1 |= TIM_CR1_CEN; // ������ ������!
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //���������� TIM6_DAC_IRQn ����������

}

void TIM6_IRQHandler(void)
{
	TIM6->SR &= ~TIM_SR_UIF; //���������� ���� UIF
	if (AudioCH)
		DAC->DHR12L1 = AudioUP;
	else DAC->DHR12L2 = AudioUP;
	DAC->CR |= DAC_CR_EN1;

	AudioUP = sin[i++]; //���������� � ��� ��������� ������� �������
	if (i==32) i=0; //���� ������ � ��� ��� 32 �������� �� �������� ������
}
