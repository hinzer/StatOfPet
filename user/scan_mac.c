#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "scan_mac.h"
#include <stdlib.h>
#include <time.h>





//





char AP_MAC_macs[MAC_COUNTS][32] = {"bc:5f:f6:71:bb:2e,-75", "dc:16:b2:b9:f4:c0,-90"}; 
void scan_done(void *arg, STATUS status)
{ 
    uint8 ssid[33];
    char temp[128];
    uint8 count = 0;        //count
    uint8 counts = MAC_COUNTS;       //mac   numbers       
    if (status == OK) 
    { 
        struct bss_info *bss_link = (struct bss_info *)arg;
        while (bss_link != NULL) 
        { 
            memset(ssid, 0, 33);
            if (strlen(bss_link->ssid) <= 32) 
                memcpy(ssid, bss_link->ssid, strlen(bss_link->ssid));
            else memcpy(ssid, bss_link->ssid, 32);
            //printf("(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n", bss_link->authmode, ssid, bss_link->rssi, MAC2STR(bss_link->bssid),bss_link->channel);
            if (count != counts) {
                sprintf(&AP_MAC_macs[count][0],"%02x:%02x:%02x:%02x:%02x:%02x,%d",MAC2STR(bss_link->bssid), bss_link->rssi);
                count++;
            }
            bss_link = bss_link->next.stqe_next;
        } 
        // printf("%s\r\n", &AP_MAC_macs[0][0]);    //test    
        // printf("%s\r\n", &AP_MAC_macs[1][0]);    //test  
    } 
    else 
    { 
        printf("scan fail !!!\r\n");
    }
}



/******************************************************************************
 * FunctionName : ATaskBody
 * Description  : ssssss
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ATaskSCANMAC( void *pvParameters )
{
    
    for(;;)
    {

        vTaskDelay(500);
        //system_init_done_cb(scan_done);     //
        wifi_station_scan(NULL,scan_done);
    }
    vTaskDelete( NULL );


}


char STA_MAC_mac01[32] = {0};
void SCAN_MAC_init(void)
{
    char sta_mac[6] = {0};
    wifi_get_macaddr(STATION_IF, sta_mac);
    sprintf(&STA_MAC_mac01[0],"%02x:%02x:%02x:%02x:%02x:%02x\n",sta_mac[0],sta_mac[1],sta_mac[2],sta_mac[3],sta_mac[4],sta_mac[5]);

    xTaskCreate(ATaskSCANMAC, "ATaskSCANMAC", 256, NULL, 4, NULL);

}





