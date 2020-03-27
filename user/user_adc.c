#include "user_adc.h"
#include "esp_common.h"

#include "gpio.h"



void ATaskAdc( void *pvParameters )
{

	for(;;)	
	{
		u16 adc = system_adc_read();	//读Pin6管脚的ADC 0-1v

        vTaskDelay(200);			//任务延时2s
	}
	vTaskDelete( NULL );
}

void Adc_init(void)
{

    xTaskCreate(ATaskAdc, "ATaskAdc", 256, NULL, 2, NULL);
}


