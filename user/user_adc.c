#include "user_adc.h"
#include "esp_common.h"

#include "gpio.h"



// static uint16 NTCTAB[21] = {0x200,0x1F6,0x1ED,0x1E3,0x1DA,0x1D1,0x1C8,0x1BE,0x1B5,0x1AC,0x1A4,
// 							0x19B,0x192,0x18A,0x181,0x179,0x171,0x169,0x161,0x159,0x151};
//每变化1度 对应的量化值
static uint16 NTCTAB[21] = {903,890,877,864,851,838,825,812,799,786,773,760,
							747,734,721,708,695,682,669,656,643};
static uint8 Table_data=0;
float Temp_data=0;

void ATaskAdc( void *pvParameters )
{
    int iVariableExample = 0;

	// u16 adc = system_adc_read();	//读Pin6管脚的ADC 0-1v
	// u16 adc = system_get_vdd33();
	for(;;)	//实现一个死循环
	{
		u16 adc = system_adc_read();	//读Pin6管脚的ADC 0-1v
		Table_data = look_up_table(NTCTAB,21,adc);
		Temp_data = (float) (num_to_temperature(Table_data)*10 + (adc%10))/10;	//整数部分

		// Temp_data += (float)((adc%10) / 10);			//添加小数点后的部分

		os_printf("ADC: %d\r\n", (int)Temp_data);

        vTaskDelay(200);			//任务延时2s
	}
	vTaskDelete( NULL );
}

void adc_timer_init(void)
{
    //os_timer_disarm(&adc_timer);
    //os_timer_setfn(&adc_timer, (os_timer_func_t *) adc_timer_cb, NULL);
    //os_timer_arm(&adc_timer, ADC_READ_INTERVAL, 1);
    xTaskCreate(ATaskAdc, "adc_test", 256, NULL, 2, NULL);
}


//ADC量化后的处理算法
uint8 look_up_table(uint16 *a,uint8 ArrayLong,uint16 sample_value)
{    
	uint8 begin,end,middle;  
	uint8 cnt=0; 
	
	begin = 0;  
	end = ArrayLong-1;  
	 	
	if(sample_value >= a[begin])
		return begin;  
	else if(sample_value <= a[end])
		return end;
  
	while(begin < end)  
	{  
		middle = (begin+end)/2;  
		
		if(sample_value == a[middle]) 
			break;  
		
		if(sample_value < a[middle] && sample_value > a[middle+1])
			break;  
		
		if(sample_value > a[middle]) 
			end = middle;                      
		else
			begin = middle; 
		
		if(cnt++ > ArrayLong) 
			break;  
	} 
	
	if(begin > end )
		return 0;
	
	return middle;  
}

//转换成数值
uint8 num_to_temperature(uint8 num)
{
	uint8 temp=0;
	
	temp = num + 25;
	return temp;
}