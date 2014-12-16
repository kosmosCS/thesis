/* 
 * File:   main.c
 * Author: kosmosCS
 *
 * Created on 2 Октябрь 2014 г., 11:54
 */

//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include "init.h"
//#include "uart.h"
//#include <stdint.h>

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure Segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Mode (Primary Oscillator (XT, HS, EC))
#pragma config IESO = OFF                // Two-speed Oscillator Start-Up Enable (Start up with FRC, then switch)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config LPOL = ON                // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD3               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

/*
 * 
 */

#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

//
///*************************************************************************
// * Processor-specific peripheral libraries are optional for XC32 porting C32
// *************************************************************************/
//OPTIONAL("libmchp_peripheral.a")
//OPTIONAL("libmchp_peripheral_32MX460F512L.a")
//
#define REG_HOLDING_START 0
#define REG_HOLDING_NREGS 15

static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

#define	EXIT_SUCCESS	0
#define	EXIT_FAILURE	1


int main() {
//    const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
    eMBErrorCode eStatus;
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 0;
    usRegHoldingBuf[0] = 44;
    usRegHoldingBuf[1] = 55;
    usRegHoldingBuf[2] = 66;
    usRegHoldingBuf[3] = 77;

    eStatus = eMBInit(MB_RTU, 0x01, 2, 256000, MB_PAR_NONE);
   // eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 );
    
    eStatus = eMBEnable(  );
    U2STAbits.UTXEN = 0;
    U2STAbits.UTXEN = 1;
    LATBbits.LATB8 = 0;
    while(1)
    {
//        char ReceivedChar;
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;

     }
    return (EXIT_SUCCESS);
}

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void)
{
    /* Interrupt Service Routine code goes here */
    pxMBFrameCBByteReceived(  );
    IFS1bits.U2RXIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0;
    pxMBFrameCBTransmitterEmpty(  );
}


eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
 eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                           eMBRegisterMode eMode )
  {
  eMBErrorCode    eStatus = MB_ENOERR;
     int             iRegIndex;

     if( ( usAddress >= REG_HOLDING_START ) &&
         ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
     {
         iRegIndex = ( int )( usAddress - usRegHoldingStart );
         switch ( eMode )
         {
             /* Pass current register values to the protocol stack. */
         case MB_REG_READ:
             while( usNRegs > 0 )
             {
                 *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                 *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                 iRegIndex++;
                 usNRegs--;
             }
             break;

             /* Update current register values with new values from the
              * protocol stack. */
         case MB_REG_WRITE:
             while( usNRegs > 0 )
             {
                 usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                 usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                 iRegIndex++;
                 usNRegs--;
             }
         }
     }
     else
     {
         eStatus = MB_ENOREG;
     }
     return eStatus;
  }

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}


