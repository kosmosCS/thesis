#include <xc.h>
#include"uart.h"
#include "init.h"
#include "timer.h"
//#define LPC_TIM0              ((U1MODEBITS       *) LPC_TIM0_BASE     )

void InitUART(ULONG baud) {
    unsigned int UMODEvalue, USTAvalue;
//    float BRGValue;
    int i;

     // Load a value into Baud Rate Generator.  Example is for 9600.
    // See section 19.3.1 of datasheet.

//    BRGValue = (Fcy /(16 * (float)baud)) - 1;
    ULONG BRGValue;
    BRGValue = (100*Fcy  /(16 * baud) - 100);
    if ((BRGValue % 100) >= 50) BRGValue += 100;
    BRGValue = BRGValue/100;

    UMODEvalue = UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW &
		 UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK & UART_DIS_ABAUD &
                 UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & UART_1STOPBIT & UART_URXINVZERO;
    USTAvalue = UART_INT_TX & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED &
                UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR & UART_TX_ENABLE &
                UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR;

#ifdef initUART1
    OpenUART1(UMODEvalue, USTAvalue, (int)BRGValue);

    IPC3bits.U1TXIP = 4;    // Mid Range Interrupt Priority level, no urgent reason
    IPC2bits.U1RXIP = 4;
    ConfigIntUART1(UART_RX_INT_DIS & UART_RX_INT_PR7 & UART_TX_INT_DIS & UART_TX_INT_PR7);

    U1MODEbits.UARTEN = 1;	// And turn the peripheral on
    U1STAbits.UTXEN = 1;        // I think I have the thing working now.
#endif

#ifdef initUART2
    OpenUART2(UMODEvalue, USTAvalue, BRGValue);

    IPC7bits.U2TXIP = 4;    // Mid Range Interrupt Priority level, no urgent reason
    IPC7bits.U2RXIP = 4;
    IPC7 = 0x4400;	// Mid Range Interrupt Priority level, no urgent reason
    ConfigIntUART2(UART_RX_INT_EN & UART_RX_INT_PR7 & UART_TX_INT_EN & UART_TX_INT_PR7);
    
    U2MODEbits.UARTEN = 1;	// And turn the peripheral on
    U2STAbits.UTXEN = 0;        // I think I have the thing working now.
#endif
    /* Delay after turning on. */
        for(i = 0; i < 4160; i++)
        {
            Nop();
        }

}



void initTimer1(unsigned int period)
{
    unsigned int config;

    config = T1_ON & T1_IDLE_CON & T1_GATE_OFF & T1_PS_1_256 & T1_SYNC_EXT_OFF
            & T1_SOURCE_INT;

    OpenTimer1(config, period);

    ConfigIntTimer1(T1_INT_PRIOR_3 & T1_INT_ON);

//    T1CONbits.TON = 0;
//    T1CONbits.TCKPS = 0b11;
//    T1CONbits.TCS = 0;
//    T1CONbits.TGATE = 0;
//    TMR1 = 0;
//    PR1 = 1000;

//    IPC0bits.T1IP = 0x01;// Set Timer1 Interrupt Priority Level
//    IFS0bits.T1IF = 0;// Clear Timer1 Interrupt Flag
//    IEC0bits.T1IE = 1;// Enable Timer1 interrupt

//    T1CONbits.TON = 1;
}


void INTDisableInterrupts(){
    INTCON1bits.NSTDIS = 1;   // disable nesting
//    mcuAtomicOr(SR, 0x00E0);  // disable interrupts
}
void INTEnableSystemMultiVectoredInt(){
//    mcuAtomicAnd(SR, 0xFF1F); // enable interrupts
 	INTCON1bits.NSTDIS = 0;   // enable nesting
}
