/* 
 * File:   init.h
 * Author: Gani
 *
 * Created on 2 Октябрь 2014 г., 15:49
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "port.h"

#define Fcy 16000000UL
/*******************************Defining peripheria***************************/
/* UART */
//#define initUART1
#define initUART2
/* Timer*/
#define TIMER1
//#define TIMER2
//#define TIMER3
//#define TIMER4
//#define TIMER5
//#define TIMER6
//#define TIMER7
//#define TIMER8
//#define TIMER9

    /* There is not config to set polatrity in uart.h.
     * Then I added. */
#define UART_URXINVONE           0xFFFF  /*2 stop bits*/
#define UART_URXINVZERO          0xFFEF  /*1 stop bit*/

#define mcuAtomicOr(variable, mask)            \
     __asm__ volatile (                        \
         "    ior.w %0, [%1], [%1] \n"        \
         : : "r"(mask), "r"(&(variable))        \
         : "cc"                                \
     )

 #define mcuAtomicAnd(variable, mask)        \
     __asm__ volatile (                        \
         "    and.w %0, [%1], [%1] \n"        \
         : : "r"(mask), "r"(&(variable))        \
         : "cc"                                \
     )

 #define mcuAtomicXor(variable, mask)        \
     __asm__ volatile (                        \
         "    xor.w %0, [%1], [%1] \n"        \
         : : "r"(mask), "r"(&(variable))        \
         : "cc"                                \
     )

///* Macros to  Enable/Disable interrupts and set Interrupt priority of UART2 */
//#define EnableIntU2RX                    _U2RXIE = 1
//#define EnableIntU2TX                    _U2TXIE = 1
//
//#define DisableIntU2RX                   _U2RXIE = 0
//#define DisableIntU2TX                   _U2TXIE = 0


///*********************************************************************
// * UART 2 Interrupt Control Functions
// ********************************************************************/
///*********************************************************************
// * UART 2 Error
// ********************************************************************/
//#define mU2EClearIntFlag()                  (IFS1CLR = _IFS1_U2EIF_MASK)
//#define mU2EGetIntFlag()                    (IFS1bits.U2EIF)
//#define mU2EGetIntEnable()                  (IEC1bits.U2EIE)
//#define mU2EIntEnable(enable)               (IEC1CLR = _IEC1_U2EIE_MASK, IEC1SET = ((enable) << _IEC1_U2EIE_POSITION))
///*********************************************************************
// * UART 2 Receive
// ********************************************************************/
//#define mU2RXClearIntFlag()                 (IFS1CLR = _IFS1_U2RXIF_MASK)
//#define mU2RXGetIntFlag()                   (IFS1bits.U2RXIF)
//#define mU2RXGetIntEnable()                 (IEC1bits.U2RXIE)
//#define mU2RXIntEnable(enable)               (IEC1CLR = _IEC1_U2RXIE_MASK, IEC1SET = ((enable) << _IEC1_U2RXIE_POSITION))
///********************************************************************
// * UART 2 Transfer
// ********************************************************************/
//#define mU2TXClearIntFlag()                 (IFS1CLR = _IFS1_U2TXIF_MASK)
//#define mU2TXGetIntFlag()                   (IFS1bits.U2TXIF)
//#define mU2TXGetIntEnable()                 (IEC1bits.U2TXIE)
//#define mU2TXIntEnable(enable)               (IEC1CLR = _IEC1_U2TXIE_MASK, IEC1SET = ((enable) << _IEC1_U2TXIE_POSITION))
///*********************************************************************
// * UART 2 Vector
// ********************************************************************/
//#define mU2ClearAllIntFlags()              ( IFS1CLR = (_IFS1_U2EIF_MASK | _IFS1_U2RXIF_MASK | _IFS1_U2TXIF_MASK))
//#define mU2IntDisable()                    ( IEC1CLR = (_IEC1_U2EIE_MASK | _IEC1_U2RXIE_MASK | _IEC1_U2TXIE_MASK))
//#define mU2SetIntEnable(flags)             ( IEC1SET = ((flags)<<_IEC1_U2EIE_POSITION) )
//#define mU2SetIntPriority(priority)        ( IPC8CLR = _IPC8_U2IP_MASK, IPC8SET = ((priority) << _IPC8_U2IP_POSITION))
//#define mU2GetIntPriority()                (IPC8bits.U2IP)
//#define mU2SetIntSubPriority(subPriority)  ( IPC8CLR = _IPC8_U2IS_MASK, IPC8SET = ((subPriority) << _IPC8_U2IS_POSITION))
//#define mU2GetIntSubPriority()             (IPC8bits.U2IS)


void InitUART(ULONG baud);
void initTimer1(unsigned int period);
void INTDisableInterrupts();
void INTEnableSystemMultiVectoredInt();
#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

