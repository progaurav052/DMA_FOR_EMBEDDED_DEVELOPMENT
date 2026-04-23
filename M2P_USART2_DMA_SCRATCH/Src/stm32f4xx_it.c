/*
 * stm32f4xx_it.c
 *
 *  Created on: Apr 21, 2026
 *      Author: ggpai
 */
#include "stm32f446xx.h"

extern void HT_Complete_callback(void);
extern void FT_Complete_callback(void);
extern void TE_error_callback(void);
extern void DME_error_callback(void);
extern void FE_error_callback(void);

#define is_it_HT() 	DMA1->HISR & ( 1 << 20)
#define is_it_FT() 	DMA1->HISR & ( 1 << 21)
#define is_it_TE() 	DMA1->HISR & ( 1 << 19)
#define is_it_FE() 	DMA1->HISR & ( 1 << 16)
#define is_it_DME() DMA1->HISR & ( 1 << 18)

void clear_exti_pending_bit(){


	if(EXTI->PR & ( 1 << 13))
	{
		EXTI->PR |= ( 1 << 13);

	}
}

void EXTI15_10_IRQHandler(void){


	USART2->CR3 |= (0x1 << 7);

	// here clear the corresponding EXTI_PR bit by programming 1 to it
	// if we dont do this interrupt will be triggered again and again
	// if this bit is 1 , than NVIC's corresponding ENbale bit correspoding to IRQ number will be set
	clear_exti_pending_bit();

}

//IRQ handler for DMA1_stream6 gloabl interrupt
void DMA1_Stream6_IRQHandler(void)
{
	//test the code , enable the Interrupt control bits for Half transfer and full_transfer

	// in every DMA peripheral there are interrupt status register LISR(stream 0 -> 3) , HISR(stream 4 -> 7 ) eg: DMA1->LISR , DMA1->HISR

	// for this application we need to work with HISR regsiter of DMA1


		//Half-transfer
		if( is_it_HT() )
		{
			DMA1->HIFCR |= ( 1 << 20);
			HT_Complete_callback();
		}else if(is_it_FT() )
		{
			DMA1->HIFCR |= ( 1 << 21);
			FT_Complete_callback();
		}else if ( is_it_TE() )
		{
			DMA1->HIFCR |= ( 1 << 19);
			TE_error_callback();

		}else if (is_it_FE() )
		{
			DMA1->HIFCR |= ( 1 << 16);
			FE_error_callback();
		}else if( is_it_DME() )
		{
			DMA1->HIFCR |= ( 1 << 18);
			DME_error_callback();
		}else{
			  ;
		}



}

