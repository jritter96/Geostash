#include "wifi.h"

#include "../../main/debugprint.h"
#include "../system_defs.h"
#include "wifi_defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*
 * Initializes the wifi module, including connecting to wifi and turning on the
 * http-server
 * Parameters: char array for holding the IP address of the module
 * Returns nothing.
 */
void Init_WiFi(char * ip) {
    char c = '1';
    char *const dofile = "dofile(\"send_text_message.lua\")";
    char *const checkwifi = "connect_wifi()";
    char *const dohttp = "dofile(\"http_example.lua\")";
    int success = 0;
    int i;
    // Set the UART chip's baud rate:
    const unsigned int divisorLatch = SYSTEM_CLOCK_FREQ / 115200 / 16;
    WIFI_UART_ADDR_LCR = 0x80;
    WIFI_UART_ADDR_DLL = divisorLatch & 0xFF;
    WIFI_UART_ADDR_DLM = divisorLatch >> 8;
    WIFI_UART_ADDR_LCR = 0x00;

    //Set bit 7 of Line control register back to 0 and
    //program other bits in that reg for 8 bit data, 1 stop bit, and no parity
    WIFI_UART_ADDR_LCR = 0x03;

    // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    WIFI_UART_ADDR_FCR = 0x06;

    // Now Clear all bits in the FiFo control registers
    WIFI_UART_ADDR_FCR = 0x00;
    sendNewLine();
    delay(1.0);
    while (c != '>'){
        c = getcharWiFi();
    }

    delay(1.0);
    if (DEBUG_WIFI) printf("Initialized\nDo file\n");
    sendCommandWiFi(dofile);

    if (DEBUG_WIFI) printf("Waiting for > response\n");
    waitForDone();

    while (success == 0) {
        delay(1.0);

        if (DEBUG_WIFI) printf("Got response\nCheck Wifi\n");
        sendCommandWiFi(checkwifi);

        if (DEBUG_WIFI) printf("Waiting for > response\n");
        char c = '0';
        i = 0;
        while (c != '>') {
            c = getcharWiFi();
            if ((c >= '0' && c <= '9') || c == '.') {
                success = 1;
                ip[i] = c;
                i++;
            }
        }
        if (DEBUG_WIFI && success) {
            printf("\n===============\nIP: %s\n===============\n\n", ip);
        }
    }
    if (DEBUG_WIFI) printf("Got response\nOpening http file\n");
    sendCommandWiFi(dohttp);

    if (DEBUG_WIFI) printf("Waiting for > response\n");
    waitForDone();

    // Clear the buffer:
    while (WIFI_UART_ADDR_LSR & 0x01) {
        char c = getcharWiFi();
    }
}

/*
 * Sends a character to the wifi module via the UART bridge.
 * Parameters: the character (in int form) to send to the module
 * Returns: the character (in int form) that was sent to the module
 */
char putcharWiFi(char c) {
    // wait for Transmitter Holding Register bit (5) of line status register to be '1'
    // indicating we can write to the device
    // write character to Transmitter fifo register
    // return the character we printed
    while(!(WIFI_UART_ADDR_LSR & 0x20)) {
        //do nothing
    }

    WIFI_UART_ADDR_THR = c;
    return c;
}

/*
 * Reads a character (in int form) from the wifi module
 * BLOCKS UNTIL THERE IS SOMETHING TO READ. MAKE SURE SOMETHING
 * IS THERE BEFORE YOU CALL THIS OR ELSE YOU WILL BE STUCK
 * You have been warned ;)
 * Returns the character (in int form) that it read from the wifi
 * module
 */
char getcharWiFi(void) {
    // wait for Data Ready bit (0) of line status register to be '1'
    // read new character from ReceiverFiFo register
    // return new character
    while(!(WIFI_UART_ADDR_LSR & 0x01)) {
        // do nothing
    }

    return WIFI_UART_ADDR_RBR;
}

/*
 * Delays for a number of seconds. Returns after that amount of time
 * has passed
 * Parameters: a double with the number of seconds to delay for (decimals
 * are fractions of seconds)
 * Returns the current time (doesn't need to be used. Mostly to make
 * sure the compiler doesn't optimize anything away)
 */
double delay(double seconds) {
    // Storing start time
    double start_time = (double) clock() / CLOCKS_PER_SEC;
    double curr_time = (double) clock() / CLOCKS_PER_SEC;

    // looping till required time has passed
    while (curr_time < start_time + seconds){
        curr_time = (double) clock() / CLOCKS_PER_SEC;
    }
    return curr_time;
}

/*
 * Sends a command to the wifi module. Sends all necessary newline
 * characters to the wifi module, so strings passed to this function
 * do not need to include those.
 * Parameters: char array (string) with the command to be sent to
 * the wifi module. Does not include any newline characters and is
 * terminated by a null character
 * Returns nothing.
 */
void sendCommandWiFi(char* command) {
    int i;
    sendNewLine();
    for (i = 0; command[i] != '\0'; i++ ) {
        putcharWiFi(command[i]);
        char c = getcharWiFi();
        while(c!= command[i]) {
        c = getcharWiFi();
        }
    }
    sendNewLine();
}

/*
 * Sends the distance to the wifi module so it can be returned
 * to the user.
 * Parameters: string of the distance in meters.
 * Returns nothing.
 */
void wifiSendResponse(char* response) {
    int i;
    for (i = 0; response[i] != '\0'; i++) {
        char d = putcharWiFi(response[i]);
    }
    sendNewLine();
}

/*
 * Waits for the wifi module to be ready for a new command. Returns once
 * the module is ready.
 * Takes no parameters and returns nothing
 */
void waitForDone() {
    char c = getcharWiFi();
    while (c != '>') {
        c = getcharWiFi();
    }
}

/*
 * Sends a new line to the wifi module.
 * Takes no parameters and returns nothing.
 */
void sendNewLine() {
    putcharWiFi('\r');
    getcharWiFi();
    putcharWiFi('\n');
    getcharWiFi();
    delay(0.1);
}
