#include "communication.h"

#include "debugprint.h"

#include "../hardware_utils/gps/gps.h"
#include "../hardware_utils/wifi/wifi.h"
#include "../hardware_utils/wifi/parsing.h"
#include "players/players.h"


#define END_OF_DATA '$'

/*
 * Handle data from the WiFi UART buffer, if any exists. Otherwise,
 * do nothing and simply return.
 */
void processWiFi() {
    char bufferData[HTTP_REQUEST_MAX_LENGTH];
    char distStr[GPD_DISTANCE_STRING_BUFFER_SIZE] = "";
    int i;
    double distance;


    for (int i = 0; i<1000; i++){
        if ((WIFI_UART_ADDR_LSR & 0x01)) {
            // There is something to process:
            break;
        }
        if (i == 999) {
            return;
        }
    }

    // Read all values from the UART buffer into an array:
    for (i = 0; i < HTTP_REQUEST_MAX_LENGTH; i++) {
        char c = getcharWiFi();
        bufferData[i] = c;
        if (c == END_OF_DATA) break;
    }
    bufferData[i++] = '\0';
    if (DEBUG_COMMUNICATION) printf("get data!!!!!!!\n");

    // Parse message retrieved by UART buffer
    struct http_message message = parseMessage(bufferData, i);
    if (DEBUG_COMMUNICATION) printf("message requestType: %s\n, request wanted: %d\n",
        WEBSERVER_COMMS_CMD_STRINGS[message.requestType], REGISTER_USER);
    if (message.requestType == PING) {
        if (DEBUG_COMMUNICATION) printf("message: %s, lat: %lf, long: %lf\n", message.request, message.loc.lat, message.loc.lon);
        if (DEBUG_COMMUNICATION) printf("getting location\n");
        distance = getDistance(getLocation(), message.loc);

        gpsDistanceToString(distance, distStr);
        if (DEBUG_COMMUNICATION) printf("Return from to string: %s\n", distStr);
        wifiSendResponse(distStr);

    } else if (message.requestType == REGISTER_USER) {
        if (DEBUG_COMMUNICATION) printf("message: %s, username:%s\n", message.request, message.username);
        int pinNumber = registerPlayer(message.username);
        char pin[5];
        int charsWritten = sprintf(pin, "%d\n", pinNumber);
        pin[charsWritten] = '\0';
        wifiSendResponse(pin);
    }

    // Clear 'nil' at the end of every forward from the Wifi 
    delay(0.1);
    if (DEBUG_COMMUNICATION) printf("clear buffer\n");
    while (WIFI_UART_ADDR_LSR & 0x01) {
        char c = getcharWiFi();
        if (DEBUG_COMMUNICATION) printf("%c\n", c);
    }
    if (DEBUG_COMMUNICATION) printf("buffer cleared\n");
}
