#include <util/delay.h> /* For the delay functions */

#include "Dio/Dio.h"

#define LED_PORT  PORTA_ID
#define LED_PIN   PIN4_ID

int main(void)
{
	Dio_Init(&Dio_ConfigurationSet);
//	GPIO_setupPinDirection(LED_PORT, LED_PIN, PIN_OUTPUT);

	while(1) {
		Dio_WritePin(LED_PORT, LED_PIN, STD_HIGH);
		_delay_ms(1000);
		Dio_WritePin(LED_PORT, LED_PIN, STD_LOW);
		_delay_ms(1000);
	}

}
