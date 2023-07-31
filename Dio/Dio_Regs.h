 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio_Regs.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Dio Driver Registers
 *
 * Author: Belal Mohamed
 ******************************************************************************/

#ifndef DIO_REGS_H
#define DIO_REGS_H


/*
 * Module Version 1.0.0
 */
#define DIO_REGS_SW_MAJOR_VERSION           (1U)
#define DIO_REGS_SW_MINOR_VERSION           (0U)
#define DIO_REGS_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_REGS_AR_RELEASE_MAJOR_VERSION   (4U)
#define DIO_REGS_AR_RELEASE_MINOR_VERSION   (0U)
#define DIO_REGS_AR_RELEASE_PATCH_VERSION   (3U)

#include "../LIB/Std_Types.h"
/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DIO_REGS_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != DIO_REGS_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != DIO_REGS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif


#define DDRA          *((volatile uint8 *)(0X3A))
#define DDRB          *((volatile uint8 *)(0X37))
#define DDRC          *((volatile uint8 *)(0X34))
#define DDRD          *((volatile uint8 *)(0X31))

typedef union PORT_Union
{
	volatile uint8 Register;
	struct BitField
	{
		volatile uint8 PIN_0: 1; // 0
		volatile uint8 PIN_1: 1; // 1
		volatile uint8 PIN_2: 1; // 2
		volatile uint8 PIN_3: 1; // 3
		volatile uint8 PIN_4: 1; // 4
		volatile uint8 PIN_5: 1; // 5
		volatile uint8 PIN_6: 1; // 6
		volatile uint8 PIN_7: 1; // 7
	} BitField;
}PORT_Union;
#define PORTA          *((volatile uint8 *)(0X3B))
#define PORTB          *((volatile uint8 *)(0X38))
#define PORTC          *((volatile uint8 *)(0X35))
#define PORTD          *((volatile uint8 *)(0X32))

#define PINA           *((volatile uint8 *)(0X39))
#define PINB           *((volatile uint8 *)(0X36))
#define PINC           *((volatile uint8 *)(0X33))
#define PIND           *((volatile uint8 *)(0X30))

#endif /* DIO_REGS_H */
