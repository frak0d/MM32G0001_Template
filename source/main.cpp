#include <mm32g0001.h>
#include <delay.h>

void configGPIO()
{
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

	GPIO_InitTypeDef PA11 {
		.GPIO_Pin = GPIO_Pin_11,
		.GPIO_Speed = GPIO_Speed_High,
		.GPIO_Mode = GPIO_Mode_Out_PP
	};

	GPIO_Init(GPIOA, &PA11);
}

int main()
{
	initDelay();
	configGPIO();
	while (1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) == Bit_RESET ? Bit_SET : Bit_RESET);
		delayMs(1000);
	}
}
