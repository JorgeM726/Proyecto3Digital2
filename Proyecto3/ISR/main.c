#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

//********************PROTOTIPOS DE FUNCIONES*********************
void TimerConfig(void);
void Timer0IntHandler(void);
void UARTconfig(void);
void SendString(char* frase);
void SendString1(char* frase);
void PortFIntHandler (void);
void delay(uint32_t msec);
void delay1ms(void);


//*******************DECLARACION DE VARIABLES**********************

bool state = false;
bool state2 = false;
char outBin[] = "0000";
char valor = ' ';
char valorP = ' ';
int32_t portValue;
uint8_t n=0;
uint32_t i = 0;
//**************************PROGRAMA********************************
void main(void)
//*****************************SETUP***********************************
{   //Se configura reloj a 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //Se activa el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Se configuran como salida los LEDS
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);



    IntMasterEnable();
    GPIOIntRegister(GPIO_PORTF_BASE, PortFIntHandler);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_BOTH_EDGES);
    TimerConfig();
    UARTconfig();
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,0);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_PIN_5);
   // SendString("R: Rojo, V: Verde, B: Azul");
    //SendString1("R: Rojo, V: Verde, B: Azul");
//****************************MAIN LOOP******************************
    while(1){


    }

}

//********************************FUNCIONES*******************************************
void TimerConfig(void){
    //Configuracion del Timer 0 segun parte 2 del lab
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) //Espera a que el modulo del T0 este listo
    { }
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //Se configura T0 como periodico de 32 bits
    TimerLoadSet(TIMER0_BASE, TIMER_A, 20000000);//Se asigna un valor de 20M de modo que se tarde 0.5s en hacer un timeout
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    IntEnable(INT_TIMER0A); //Se activa interrupcion para el timer 0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //Se habilita interrupcion para que se haga en el timeout de T0A
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A);
    // Simplemente se hace un toggle de la bandera state, para que el led se apague y encienda cada 500ms
    state= !state;
}

void PortFIntHandler (void){
    delay(1000);

    if (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)){
        outBin[0] = '1';
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    }else{
        outBin[0] = '0';
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,0);
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
    }

    if (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)){
            outBin[1] = '1';
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0);
        }else{
            outBin[1] = '0';
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0);
            GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_PIN_5);
        }

    if (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_3)){
            outBin[2] = '1';
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2);
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0);
        }else{
            outBin[2] = '0';
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_PIN_3);
        }

    if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)){
            outBin[3] = '1';
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0);
        }else{
            outBin[3] = '0';
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_PIN_5);
        }
    SendString(outBin);
    SendString1(outBin);


    GPIOIntClear(GPIO_PORTF_BASE,  GPIO_INT_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 );


}



void UARTconfig(void){
    //Aqui se configura el UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Activar clock para UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//Activar clock para puerto A de la tiva
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1); //Se activan los pines 0 y 1 del puerto A
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);


    //UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                                                    //Habilitar el puerto B
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));                                             //Esperar a que termine
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);                                                    //Habilitar UART1
    GPIOPinConfigure(GPIO_PB0_U1RX);                                                                //B0 como RX amarillo
    GPIOPinConfigure(GPIO_PB1_U1TX);                                                                //B1 como TX
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);                                      //Habilitar los pines para el UART
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); //Setear el UART1 con 115200 de baudrate, de 8 bits, de un dato y sin paridad
    UARTEnable(UART1_BASE);                                                                         //Iniciar UART1
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

    //UARTIntEnable(UART0_BASE, UART_INT_RT | UART_INT_RX); //Se activa interrupcion cada vez que se reciba un dato
    //UARTIntRegister(UART0_BASE, UARTIntHandler); //Se le coloca el nombre a la funcion del handler
}

void SendString(char* frase){
    //Funcion para poder enviar string mediante UART
    while (*frase){
        UARTCharPut(UART0_BASE, *frase++);
    }
}
void SendString1(char* frase){
    //Funcion para poder enviar string mediante UART
    while (*frase){
        UARTCharPut(UART1_BASE, *frase++);
    }
}
void delay(uint32_t msec)
{
    for (i = 0; i < msec; i++)
    {
        delay1ms();
    }

}
void delay1ms(void)
{
    SysTickDisable();
    SysTickPeriodSet(40000);
    SysTickEnable();

    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0); //Pg. 138

}
