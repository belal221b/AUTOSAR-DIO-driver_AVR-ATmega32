# **AUTOSAR DIO Driver for AVR ATmega32**
This repository contains a DIO (Digital Input/Output) driver based on the AUTOSAR (Automotive Open System Architecture) layered architecture v4.0.3 for the AVR ATmega32 microcontroller.

## **File Description**
The repository contains the following files:

- **Det_Driver**: Development Error Tracer driver implementation.
- **Dio**: DIO driver implementation.
- **LIB**: Library files for the AVR ATmega32 microcontroller.
- **main.c**: Sample main file to demonstrate the usage of the DIO driver.
  
## **Function Description**
### **Dio_Init**
```
void Dio_Init(const Dio_ConfigType* ConfigPtr)
```
This function initializes the DIO driver with the provided configuration structure. It sets the direction of the configured pins as input or output based on the configuration.

The ***const Dio_ConfigType\* ConfigPtr** is a pointer to a constant **Dio_ConfigType** structure, which contains the configuration data for the DIO driver. This pointer is used as an input parameter for the **Dio_Init** function, which initializes the DIO driver with the provided configuration.

The **Dio_ConfigType** structure consists of arrays of channel and port configurations, which specify the direction and initial level of each channel and port. The ConfigPtr pointer points to the first element of this structure, which contains the array of channel configurations.

### **Dio_SetupPinDirection**
```
void Dio_SetupPinDirection(Dio_ChannelType ChannelId, Dio_PortLevelType Direction)
```
This function sets the direction of the specified pin to input or output based on the provided direction parameter. The direction parameter should be one of the following values: **PIN_INPUT, PIN_OUTPUT**.

### **Dio_SetupPortDirection**
```
void Dio_SetupPortDirection(Dio_PortType PortId, Dio_PortLevelType Direction)
```
This function sets the direction of all pins in the specified port to input or output based on the provided direction parameter. The direction parameter should be a binary number where each bit corresponds to the direction of a pin. The least significant bit corresponds to the pin with the lowest index.

### **Dio_WritePin**
```
void Dio_WritePin(Dio_PinType ChannelId, Dio_LevelType Level)
```
This function writes the provided logic level to the specified channel. If the channel is configured as an input, this function has no effect.

### **Dio_ReadPin**
```
Dio_LevelType Dio_ReadPin(Dio_PinType ChannelId)
```
This function reads the logic level from the specified channel and returns the result. If the channel is configured as an output, the returned value may not reflect the actual value on the pin.


### **Dio_WritePort**
```
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
```
This function writes the provided binary number to all pins of the specified port. The least significant bit corresponds to the pin with the lowest index. If a pin is configured as an input, this function has no effect.

### **Dio_ReadPort**
```
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
```
This function reads the logic level from all pins of the specified port and returns the result as a binary number. The least significant bit corresponds to the pin with the lowest index.

### **Dio_TogglePin**
```
void Dio_TogglePin(Dio_PinType ChannelId)
```
This function toggles the logic level of the specified channel. If the channel is configured as an input, this function has no effect.

## **Macro Description**
### **DIO_CFG_CHANNEL_\<channel>**
These macros are used to configure the direction and initial level of a specific channel. The available channels are defined in the Dio_PinType enumeration.

### **DIO_CFG_PORT_\<port>**
These macros are used to configure the direction and initial level of a specific port. The available ports are defined in the Dio_PortType enumeration.

### **DIO_CFG_PIN_\<port>_\<pin>**
These macros are used to configure the direction and initial level of a specific pin. The available ports and pins are defined in the Dio_PinType enumeration.

## **Data Types**

### **Dio_ConfigType**
This structure contains the configuration data for the DIO driver. It consists of arrays of channel and port configurations.

### **Dio_PinType**
This enumeration defines the available channels that can be used with the DIO driver. Each channel corresponds to a single pin on the AVR ATmega32 microcontroller.

### **Dio_PortType**
This enumeration defines the available ports that can be used with the DIO driver. Each port corresponds to a group of pins on the AVR ATmega32 microcontroller.

### **Dio_PinType**
This enumeration defines the available pins that can be used with the DIO driver. Each pin corresponds to a specific bit in a port's register.

### **Dio_LevelType**
This enumeration defines the possible logic levels that can be used with the DIO driver. It consists of two values: **STD_HIGH** and **STD_LOW**.

### **Dio_PortLevelType**
This typedef represents a binary number that contains the logic levels of all pins in a port. The least significant bit corresponds to the pin with the lowest index.


# **AUTOSAR Det Driver**

### **Det_ReportError**
```
void Det_ReportError(uint16_t ModuleId, uint8_t InstanceId, uint8_t ApiId, uint8_t ErrorId)
```
This API is used to report an error to the Development Error Tracer (DET) driver. It takes four parameters: ModuleId, InstanceId, ApiId, and ErrorId. These parameters are used to identify the module, instance, API, and error that caused the error.

# **Version Information**
- Module Version: 1.0.0
- AUTOSAR Version: 4.0.3
- Software Version:
    - Major Version: 1
    - Minor Version: 0
    - Patch Version: 0

## **Implementation Details**
The DIO driver implementation in this repository uses the Development Error Tracer (DET) driver to detect and report errors during runtime. The AUTOSAR-defined error parameter APIs are used to report errors to the DET driver.

When an error is detected in the DIO driver, it calls one of the error parameter APIs to report the error to the DET driver. The DET driver then handles the error according to the configured error policies, which may include logging the error, notifying the system integrator, or triggering a system reset.

For more information about the DET driver and the error parameter APIs used in the DIO driver implementation, you can refer to the AUTOSAR SWS (Software Specification) for the DIO driver. This document provides a detailed description of the driver architecture, functions, and error handling mechanisms.
