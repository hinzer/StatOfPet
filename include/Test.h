#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef __cplusplus
extern "C" {
#endif

extern uint16 CurrentVoltage,Current,RealTimePower,BatteryPercentage,Runtime;
extern uint16 Rand_01,Rand_02,Rand_03,Rand_04;

extern uint16 Body_stat;
extern uint16 Walk_count,BodyTemp;
extern uint16 Rand_11,Rand_12,Rand_13,Rand_14;


void ATaskPower( void *pvParameters );
void Power_init(void);

void ATaskBody( void *pvParameters );
void Body_init(void);

#ifdef __cplusplus
}
#endif

#endif

