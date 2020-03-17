#ifndef _WIFI_H_
#define _WIFI_H_

void Init_WiFi(char * ip);
char putcharWiFi(char c);
char getcharWiFi(void);
double delay(double seconds);
void waitForDone(void);
void sendCommandWiFi(char* command);
void sendNewLine(void);
void wifiSendResponse(char* distance);

#endif
