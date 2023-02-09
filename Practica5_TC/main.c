#include "lib/include.h"
 //uint16_t Result[2];
    //uint16_t duty=1999;
    uint8_t c;
    uint8_t num;
    uint16_t duty_c;
    uint8_t duc=25;
    char datos[32]="";
    uint16_t adc_data[3]={0};
    uint16_t *ptr;
    uint8_t i=0;
    uint16_t dcycle_S1=0;
    uint16_t dcycle_S2=0;
    uint16_t dcycle_S3=0;
    int boton;
    char valor[4]; 
    


extern void INTPORTA (void){
TIMER0->CTL|=(1<<0); // Se habilita el timer para que empiece a contar 
GPIOA->ICR|=(1<<6); //Se indica el pin donde esta el boton de nuestra interrupcion 
}


int main(void)
{
  char string [6];
  
    Configurar_PLL(_20MHZ);  //Confiuracion de velocidad de reloj EXP2
    Configura_Reg_ADC0();
    Configurar_UART2 ();
    Configura_Reg_PWM1(50,duty_c);//Configuro a 50hz el pwm
    Configurar_Timer0(2);
    Configurar_GPIO();
    //duty_c=1000;
    while(1)


    {
      ///LECTURA DE TIMER
      if  ((TIMER0 -> RIS & 0x00000001) == 1)
      {
        TIMER0 ->ICR|=(1<<0); //Limpia la interrupción
        GPIOF->DATA^= (1<<2); //Prende el led para verificar 
        ADC0_Leer(adc_data);  //Lectura del ADC   

      }

     //Se lleva a cabo la lectura del ADC ---------------------------------------
     //ADC0 - SECUENCIADOR 0
    ADC0->PSSI |= (1<<0); 
    while ((ADC0->  RIS & (1<<0)) == 0){};
    adc_data[0] = ADC0->SSFIFO0 & 0xfff;
    delay_ms(1);
  
    utoa(adc_data[0],valor,10); //funcion que convierte str-caracter
    transmisor(valor);

    ADC0->ISC |= (1<<0); //Conversion señal analógica a dígital -- finalizada
    i=readString('S',string);
    
    
     
      
     
    }
}