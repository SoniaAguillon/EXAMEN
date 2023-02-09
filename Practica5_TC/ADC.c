/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"


#define NVIC_ST_CTRL_R      (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile uint32_t *)0xE000E018))

void delay_ms(uint8_t milis);

extern void Configura_Reg_ADC0(void)
{
       
    // 1. RCGCADC
    SYSCTL->RCGCADC |= (1<<0);
    // 2. RCGCGPIO
    SYSCTL->RCGCGPIO |= (1<<4);
    // 3. GPIOFSEL
    GPIOE->AFSEL |= (1<<3);// habilitar pin correspondiente a señal analógica
    // 4. GPIODEN
    GPIOE->DEN &= ~0x0e;
    // 5. GPIOAMSEL
    GPIOE->AMSEL |= 0x0e;

    /////////////// SECUENCIADOR
    // 1. ADCACTSS
    ADC0->ACTSS &= ~(0x0f);
    // 2. ADCEMUX
    ADC0->EMUX |= 0x0000;
    // 3. ADCSSMUXn
    ADC0->SSMUX0 |= 0x00000000;
    // 4. ADCSSCTLn
    ADC0->SSCTL0 |= 0x00000006;
    // 5. ADCIM
    ADC0->IM |= (0xf<<16) | (0xf<<0);
    // 6. ADCPP y ADCPC
    //ADC0_PP_R |= 0x7
    ADC0->PC |= 0x3;
    // 7. ADCACTSS
    ADC0->ACTSS |= (1<<0);
}
extern void ADC0_Leer(uint16_t *adc_data){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
    ADC0->PSSI |= (1<<0);
    delay_ms(1);

    while (ADC0->RIS & 0x01 == 0);
    delay_ms(1);

    while(ADC0->SSOP0 & (1<<0) == (1<<0));
    adc_data[0] = ADC0->SSFIFO0 & 0xfff;
    delay_ms(1);
  

   

    ADC0->ISC |= (1<<0);
    delay_ms(1);
}



void delay_ms(uint8_t milis)
{
    uint8_t indice;
    for(indice=0;indice<milis;indice++)
    {
        NVIC_ST_RELOAD_R=1999;
        NVIC_ST_CTRL_R=5;
        while ((NVIC_ST_CTRL_R&0X10000)){}
    }
}



