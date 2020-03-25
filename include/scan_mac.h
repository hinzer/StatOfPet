#ifndef __SCAN_MAC_H__
#define __SCAN_MAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAC_COUNTS 2


extern char AP_MAC_macs[MAC_COUNTS][32];
extern char STA_MAC_mac01[32];


void ATaskSCANMAC( void *pvParameters );

void SCAN_MAC_init(void);


#ifdef __cplusplus
}
#endif

#endif

