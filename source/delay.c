#include <delay.h>
#include <mm32g0001_it.h>

volatile uint32_t DelayTick;

void SysTick_Handler(void)
{
    if (DelayTick)
        DelayTick--;
}

void initDelay(void)
{
    RCC_ClocksTypeDef rccClocks;
    RCC_GetClocksFreq(&rccClocks);

    if (SysTick_Config(rccClocks.HCLK_Frequency / 1000))
        while (1);

    NVIC_SetPriority(SysTick_IRQn, 0x0);
}

void delayMs(uint32_t Millisecond)
{
    DelayTick = Millisecond;
    while (DelayTick);
}