#include "lib/include.h"

extern void Configurar_UART2 (void){
    // Initialization of UART
    // UART2 PI N D6 YD7 This pins are connected to serial module
    // UART4 PIN C4 Y C5
    SYSCTL -> RCGCUART |= 0x4;  // 0b100 // Enabling UART MODULE 2, in general (1 << X) where X is number of module
    SYSCTL -> RCGCGPIO |= 0x8; // 0b1000// Port D --> Bit field 3; Port C ---> 2
    GPIOD -> LOCK = 0x4C4F434B;
    GPIOD -> CR = 0xC0; // Bit 6 y 7  
    GPIOD -> AFSEL |= (1 << 6) | (1 << 7); // pin D6 and D7
    GPIOD -> PCTL |= (GPIOD -> PCTL & 0x00FFFFFF) | 0x11000000; //(1 << 24) | (1 << 28); // 6 and 7
    GPIOD -> DEN |= (1 << 6) | (1 << 7);
    // BAUD_RATE = 20,000,000  / (16 * 57600) = 21.701388  
    // UARTFBRD[DIVFRAC] = integer(0.701388 * 64 + 0.5) = 45.38 rounded --> 45

    UART2 -> CTL = (0 << 0) | (0 << 8) | (0 << 9); 
    // Integer portion of brd
    UART2 -> IBRD = 21; 
    // Fractional  portion of brd 
    UART2 -> FBRD = 45; 
    // Desired parameters 
    UART2 -> LCRH = (0x3 << 5) | (1<<4); // 8 bits, by default we get no parity and 1-stop bit and FIFO ENABLE
    UART2 -> CC = 0; //(0 << 0); // SYSTEM CLOCK 
    UART2 -> CTL = (1 << 0) | (1 << 8) | (1 << 9); // bits 8th and 9th should be enabled together

}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART2->FR & (1<<4)) != 0 );
    v = UART2->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART2->FR & (1<<5)) != 0 );
    UART2->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}


extern int readString(char delimitador, char *string)
{
   int i = 0;
   char c = readChar();

   while(c != delimitador)
   { 
            string[i] = c;
            i++;
            c = readChar(); 
   }

   return i;
}

extern void trans_char(char c)
{ 
     while((UART2->FR & (1<<5))!=0);
    {
        UART2->DR=c;
    }
}

extern void transmisor(char *string)
{
while(*string)
{
trans_char(*(string++)); 
}
trans_char('\n');
}
