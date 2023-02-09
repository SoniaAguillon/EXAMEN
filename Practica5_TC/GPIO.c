
#include "lib/include.h"
extern void Configurar_GPIO(void)
{

 SYSCTL->RCGCGPIO |=(1<<5);
  GPIOF -> LOCK = 0x4C4F434B; //Acceder al registro look del puerto F
  GPIOF->CR = (1<<4)| (1<<3) |(1<<2)| (1<<1) | (1<<0); // Al bit 0, assignale un 1, adicionalmente suma al bit 1, ponerle un 1 y asi sucesivamente 
  GPIOF->AMSEL = 0x00; //disable analog on PF
  GPIOF->PCTL = 0x00000000; // 4) PCTL GPIO on PF4-0
  GPIOF->DIR = 0x0E; //Registro para entradas PF4,PF0 y salidas PF3-1
  GPIOF->AFSEL = 0X00; // 4) PCTL GPIO on PF4-0
  GPIOF->PUR = 0x11; // enable pull-up on PF0 and PF4
  GPIOF->DEN = (1<<4)| (1<<3) |(1<<2)| (1<<1) | (1<<0); 


  
  /////////////INTERRUPCIONES--PUERTO A
  
    SYSCTL->RCGCGPIO |=(1<<0); //Se habilita el  puerto A --- Lo usaremos para las interrupciones
    SYSCTL->PRGPIO |=(1<<0); //PRENDER EL PUERTO -Se indica para decir que el puerto esta listo para el acceso *Se pone tu pin pag406  
    GPIOA->AMSEL = 0x00;     //NO SE NECELa funcion analogica del pin que se indica esta habilitado **Como es un boton no se necesitan funciones analogicas       
    GPIOA->PCTL = 0x00000000;//Debes verificar que el pin que estas agarrando no tenga nada establecido - notas *Registro controlado con tabla 1351*
    GPIOA->DIR = (0<<6); //Se indica pin a utilizar - PIN A6     ----  ENTRADAS/SALIDAS    0->input, 1->output pag 663      
    GPIOA->AFSEL = 0x00; //Se pone la funcion alternativa ----  Función GPIO       
    GPIOA->PUR = (1<<6); //Registro para pull up ---- Se habilita la resistencia de pull up para el pin indicado *Yo usare el pin 6             
    GPIOA->DEN = (1<<6);  //Habilitar funciones digitales al pin correspondiente **PIN A6


  //Interrupción 0
  //PAGINA 104 - CHECAS TU NUMERO DE INTERRUPCION (COLUMNA 2) 
  //* Para las primeras 4 interrupciones es 0
  // Calcular n: #de interrupción/4 (tomar solo la parte entera)
  // A partir de la parte entera, contamos cuál es la que corresponde
  // Vectores [4n] [4n + 1] [4n + 2] [4n + 3]
  // n = 0     0      1         2         3
  // En el registro IP se coloca dentro de los corchetes la n: IP[n]  
  // Cada par de números representa la jerarquia de una interrupción     (NVIC->IP[0]&0x| FF | FF | FF | 00 |)
  //                                                                                     4n+3 4n+2 4n+1  4n
    NVIC->IP[0] = (NVIC->IP[0]&0xFFFFFF00) | 0x00000080;  //PRIMERO SE PONEN LOS DOS 0 EN LA POSICION EN LAS QUE SE VA A USAR 
  //Buscamos el rango de interrupcion que se va utilizar 
  //    0       1        2        3         4
  //   0-31   32-63   64-95     96-127      128-138  
  //En el registro se escribe el rango ISER[rango]=(1<<numero de interupcion)
    NVIC->ISER[0] = (1<<0); //En el corchete va el rango y ya luego en el numero pones la interrupcion que te toca (Checa la tabla para ver las cuentas)
  
 //REGISTRO DE LAS INTERRUPCIONES DEL PUERTO CORRESPONDIENTE --- TODOS CORRESPONDEN AL PIN 
    GPIOA->IM |= (0<<6); // Se apaga la interrupcion para comenzar a configurarla  para PIN B2 
    GPIOA->IS |= (0<<6); // Se pone un 0 porque queremos que se sensible a los pulsos de flanco de subida o de bajada en el PIN B2  *PAG 664
    GPIOA->IBE |= (0<<6);// Como queremos que solo al picar el boton venere la interrupcion se le pone 0  *PAG 665
    GPIOA->IEV |= (1<<6); //Se le pone 1 porque estamos en pull down y queremos que cuando se apriete el boton detecte el flanco de subida *PAG 666
    GPIOA->RIS |= (0<<6); //Forzamos la interrupcion a que no se interrumpa 
    GPIOA->IM|=(1<<6);// Ahora si prendemos la interrupcion 
  }

  extern void Delay(void){unsigned long volatile time;
  time = 1600000;
  while(time){
		time--;
  }
}
  
  
  
    