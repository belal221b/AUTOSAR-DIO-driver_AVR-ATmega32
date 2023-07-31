/******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio.c
 *
 * Description: Source file for AVR ATmega32 Microcontroller - Dio Driver
 *
 * Author: Belal Mohamed
 ******************************************************************************/

#include "Dio.h"
#include "Dio_Regs.h"
/* Software Version checking between Dio_Regs.h and Dio.h files */
#if ((DIO_REGS_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION)\
 ||  (DIO_REGS_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION)\
 ||  (DIO_REGS_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif


#if (DIO_DEV_ERROR_DETECT == STD_ON)

#include "../Det_Driver/Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif




STATIC const Dio_ConfigType *Dio_PortPins = NULL_PTR;
STATIC uint8 Dio_Status = DIO_NOT_INITIALIZED;

//static uint8 getPinId(Dio_PortType Port_Id, Dio_PinType PinId) {
//	if (PinId > 7) {
//		return PinId;
//		/* Ex.
//		 * PIN_D_3 = 27
//		 * return: 27
//		 * Done!
//		 * */
//	} else if (PinId <= 7) {
//		return (PinId % NUM_OF_PINS_IN_SINGLE_PORT
//				+ (Port_Id - 1) * NUM_OF_PINS_IN_SINGLE_PORT);
//		/* Ex.
//		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//		 * PORTD_ID = 4
//		 * PIN3 = 3 --->> PIN_D_3 = 27
//		 * return: 3 % 8 + (4-1) * 8 = 3 + 3*8 = 3 + 24 = 27
//		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//		 * PORTA_ID = 1
//		 * PIN3 = 3 --->> PIN_D_3 = 3
//		 * return: 3 % 8 + (1-1) * 8 = 3 + 0*8 = 3
//		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//		 * * * * * * * * * * * * * Done! * * * * * * * * * * * * * * * *
//		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//		 * */
//	} else {
//		/* Do Nothing */
//	}
//	return PinId;
//}
static uint8 getPinNum(Dio_PinType PinId) {
	if (PinId <= 7) {
		return PinId;
		/* Ex.
		 * PIN_A_3 = 3
		 * return: 3
		 * Done!
		 * */
	} else if (PinId > 7) {
		return PinId % NUM_OF_PINS_IN_SINGLE_PORT;
		/* Ex.
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 * PIN_D_3 = 27
		 * return: 27 % 8 = 3
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 * * * * * * * * * * * * * * Done! * * * * * * * * * * * * * * *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 * */
	} else {
		/* Do Nothing */
	}
	return PinId;
}

/************************************************************************************
 * Service Name: Dio_Init
 * Service ID[hex]: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to post-build configuration data.
 * Parameters (input): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the Dio module.
 ************************************************************************************/
void Dio_Init(const Dio_ConfigType *ConfigPtr) {
#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_INIT_SID,
		     DIO_E_PARAM_CONFIG);
	}
	else
#endif
	{
//	Dio_PortPins = (ConfigPtr->pins); /* address of the first pin structure --> pin[0] */

		if (DIO_NOT_INITIALIZED == Dio_Status) {
			Dio_Status = DIO_INITIALIZED;
			Dio_PortPins = ConfigPtr;
			uint8 pinNum;
			volatile uint8 *Port_Ptr = NULL_PTR;
			
			/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
			for (uint8 i = 0; i < DIO_CONFIGURED_CHANNLES; ++i) {

				if (Dio_PortPins->pins[i] == NULL_PTR) {
					/* Do Nothing */
				} else {
					switch (Dio_PortPins->pins[i]->Port_Id) {
					case 1:
						Port_Ptr = &PORTA;
						break;
					case 2:
						Port_Ptr = &PORTB;
						break;
					case 3:
						Port_Ptr = &PORTC;
						break;
					case 4:
						Port_Ptr = &PORTD;
						break;
					}

					pinNum = getPinNum(Dio_PortPins->pins[i]->Pin_Id);

					if (Port_Ptr != NULL_PTR) {

						/*Set Pin Direction*/
						if (Dio_PortPins->pins[i]->dir == INPUT) {
							SET_BIT(*Port_Ptr, pinNum);
						} else if (Dio_PortPins->pins[i]->dir == OUTPUT) {
							CLEAR_BIT(*Port_Ptr, pinNum);
						} else {
							/* Do Nothing */
						}

						/*Set Pin Value*/
						if (Dio_PortPins->pins[i]->level == STD_HIGH) {
							SET_BIT(*Port_Ptr, pinNum);
						} else if (Dio_PortPins->pins[i]->level == STD_LOW) {
							CLEAR_BIT(*Port_Ptr, pinNum);
						} else {
							/* Do Nothing */
						}
					} else {
						/* Do Nothing */
					}
				}
			}
		} else {
			/* Do Nothing */
		}
	}
}

/************************************************************************************
* Service Name: Dio_SetupPortDirection
* Service ID[hex]: None
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
* 				   Pin_Id - ID of DIO pin.
*                  Direction - Value to be written.
* Parameters (input): None
* Parameters (out): None
* Return value: None
* Description: Function to set the direction of a channel.
************************************************************************************/
void Dio_SetupPinDirection(Dio_PortType Port_Id, Dio_PinType Pin_Id,
		Dio_Direction dir) {
	volatile uint8 *Port_Ptr = NULL_PTR;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_SETUP_PIN_DIRECTION_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_CHANNLES <= Pin_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_SETUP_PIN_DIRECTION_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
	//	uint8 pinId = getPinId(Port_Id, Pin_Id);
		uint8 pinNum = getPinNum(Pin_Id);

		/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
		switch (Port_Id) {
		case 1:
			Port_Ptr = &DDRA;
			break;
		case 2:
			Port_Ptr = &DDRB;
			break;
		case 3:
			Port_Ptr = &DDRC;
			break;
		case 4:
			Port_Ptr = &DDRD;
			break;
		}

		if (Port_Ptr != NULL_PTR) {
			if (GET_BIT(*Port_Ptr, pinNum) == INPUT) {
				SET_BIT(*Port_Ptr, pinNum);
			} else if (GET_BIT(*Port_Ptr, pinNum) == OUTPUT) {
				CLEAR_BIT(*Port_Ptr, pinNum);
			} else {
				/* Do Nothing */
			}
		} else {
		/* Do Nothing */
		}
	}
}

/************************************************************************************
* Service Name: Dio_SetupPortDirection
* Service ID[hex]: None
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
*                  Direction - Value to be written.
* Parameters (input): None
* Parameters (out): None
* Return value: None
* Description: Function to set the direction of a port.
************************************************************************************/
void Dio_SetupPortDirection(Dio_PortType Port_Id, Dio_Direction dir) {
	volatile uint8 *Port_Ptr = NULL_PTR;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_PORTS <= Port_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_SETUP_PORD_DIRECTION_SID, DIO_E_PARAM_INVALID_PORT_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
	switch (Port_Id) {
	case 1:
		Port_Ptr = &DDRA;
		break;
	case 2:
		Port_Ptr = &DDRB;
		break;
	case 3:
		Port_Ptr = &DDRC;
		break;
	case 4:
		Port_Ptr = &DDRD;
		break;
	}
	if (Port_Ptr != NULL_PTR) {
		if (dir == INPUT) {
			*Port_Ptr = dir;
		} else if (dir == OUTPUT) {
			*Port_Ptr = dir;
		} else {
			/* Do Nothing */
		}
	} else {
		/* Do Nothing */
	}
}

/************************************************************************************
* Service Name: Dio_WritePin
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
* 				   Pin_Id - ID of DIO pin.
*                  Level - Value to be written.
* Parameters (input): None
* Parameters (out): None
* Return value: None
* Description: Function to set a level of a channel.
************************************************************************************/
void Dio_WritePin(Dio_PortType Port_Id, Dio_PinType Pin_Id,
		Dio_PinLevelType level) {
	volatile uint8 *Port_Ptr = NULL_PTR;

	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_WRITE_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_CHANNLES <= Pin_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_WRITE_CHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
	//	uint8 pinId = getPinId(Port_Id, Pin_Id);
		uint8 pinNum = getPinNum(Pin_Id);

		switch (Port_Id) {
		case 1:
			Port_Ptr = &PORTA;
			break;
		case 2:
			Port_Ptr = &PORTB;
			break;
		case 3:
			Port_Ptr = &PORTC;
			break;
		case 4:
			Port_Ptr = &PORTD;
			break;
		}
		if (Port_Ptr != NULL_PTR) {
			if (level == STD_HIGH) {
				SET_BIT(*Port_Ptr, pinNum);
			} else if (level == STD_LOW) {
				CLEAR_BIT(*Port_Ptr, pinNum);
			} else {
				/* Do Nothing */
			}
		} else {
			/* Do Nothing */
		}
	}
}


/************************************************************************************
* Service Name: Dio_ReadPin - Dio_ReadChannel
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
* 				   Pin_Id - ID of DIO pin.
* Parameters (input): None
* Parameters (out): None
* Return value: LevelType
* 				STD_HIGH The physical level of the corresponding Pin is STD_HIGH
* 				STD_LOW The physical level of the corresponding Pin is STD_LOW
* Description: Function to Return the value of the specified DIO channel.
************************************************************************************/
Dio_LevelType Dio_ReadPin(Dio_PortType Port_Id, Dio_PinType Pin_Id) {
	volatile uint8 *Port_Ptr = NULL_PTR;
	Dio_LevelType output = STD_LOW;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_CHANNLES <= Pin_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_CHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{

		/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */

	//	uint8 pinId = getPinId(Port_Id, Pin_Id);
		uint8 pinNum = getPinNum(Pin_Id);

		switch (Port_Id) {
		case 1:
			Port_Ptr = &PORTA;
			break;
		case 2:
			Port_Ptr = &PORTB;
			break;
		case 3:
			Port_Ptr = &PORTC;
			break;
		case 4:
			Port_Ptr = &PORTD;
			break;
		}
		if (Port_Ptr != NULL_PTR) {
			if (GET_BIT(*Port_Ptr, pinNum) == STD_HIGH) {
				output = STD_HIGH;
			} else if (GET_BIT(*Port_Ptr, pinNum) == STD_LOW) {
				output = STD_LOW;
			} else {
				/* Do Nothing */
			}
		} else {
			/* Do Nothing */
		}
	}
    return output;
}


/************************************************************************************
* Service Name: Dio_WritePort
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
*                  Level - Value to be written.
* Parameters (input): None
* Parameters (out): None
* Return value: None
* Description: Function Service to set a value of the port.
************************************************************************************/
void Dio_WritePort(Dio_PortType Port_Id, Dio_PinLevelType level) {
	volatile uint8 *Port_Ptr = NULL_PTR;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_WRITE_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_PORTS <= Port_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_WRITE_PORT_SID, DIO_E_PARAM_INVALID_PORT_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
		/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
		switch (Port_Id) {
		case 1:
			Port_Ptr = &PORTA;
			break;
		case 2:
			Port_Ptr = &PORTB;
			break;
		case 3:
			Port_Ptr = &PORTC;
			break;
		case 4:
			Port_Ptr = &PORTD;
			break;
		}
		if (Port_Ptr != NULL_PTR) {
			if (level == STD_HIGH) {
				*Port_Ptr |= level;
			} else if (level == STD_LOW) {
				*Port_Ptr |= level;
			} else {
				/* Do Nothing */
			}
		} else {
			/* Do Nothing */
		}
	}
}

/************************************************************************************
* Service Name: Dio_ReadPort
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
* Parameters (input): None
* Parameters (out): None
* Return value: Dio_PortLevelType
* 				Level of all channels of that port
* Description: Function to Returns the level of all channels of that port.
************************************************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType Port_Id) {
	Dio_LevelType output = STD_LOW;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_CHANNLES <= Port_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_PORT_SID, DIO_E_PARAM_INVALID_PORT_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
		/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
		switch (Port_Id) {
		case 1:
			output = PINA;
			break;
		case 2:
			output = PINB;
			break;
		case 3:
			output = PINC;
			break;
		case 4:
			output = PIND;
			break;
		}
	}
		return output;
}


/************************************************************************************
* Service Name: Dio_TogglePine
* Service ID[hex]: None
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Port_Id - ID of DIO port.
* 				   Pin_Id - ID of DIO pin.
* Parameters (input): None
* Parameters (out): None
* Return value: None
* Description: Function to Toggle the level of the specified DIO channel.
************************************************************************************/
void Dio_TogglePine(Dio_PortType Port_Id, Dio_PinType Pin_Id) {
	volatile uint8 *Port_Ptr = NULL_PTR;
	boolean error = FALSE;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (DIO_NOT_INITIALIZED == Dio_Status)
	{
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_READ_CHANNEL_SID, DIO_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (DIO_CONFIGURED_CHANNLES <= Pin_Id)
	{

		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_TOGGLE_PIN_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
	//	uint8 pinId = getPinId(Port_Id, Pin_Id);
		uint8 pinNum = getPinNum(Pin_Id);

		/* Point to the correct PORT register according to the Port Id stored in the Port_Id member */
		switch (Port_Id) {
		case 1:
			Port_Ptr = &PORTA;
			break;
		case 2:
			Port_Ptr = &PORTB;
			break;
		case 3:
			Port_Ptr = &PORTC;
			break;
		case 4:
			Port_Ptr = &PORTD;
			break;
		}
		if (Port_Ptr != NULL_PTR) {
			if (GET_BIT(*Port_Ptr, pinNum) == STD_HIGH) {
				CLEAR_BIT(*Port_Ptr, pinNum);
			} else if (GET_BIT(*Port_Ptr, pinNum) == STD_LOW) {
				SET_BIT(*Port_Ptr, pinNum);
			} else {
				/* Do Nothing */
			}
		} else {
			/* Do Nothing */
		}
	}
}

/************************************************************************************
* Service Name: Dio_GetVersionInfo
* Service ID[hex]: 0x12
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (input): None
* Parameters (out): VersionInfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Function to get the version information of this module.
************************************************************************************/
#if (DIO_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
				DIO_GET_VERSION_INFO_SID, DIO_E_PARAM_POINTER);
	}
	else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)DIO_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)DIO_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)DIO_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)DIO_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)DIO_SW_PATCH_VERSION;
	}
}
#endif
