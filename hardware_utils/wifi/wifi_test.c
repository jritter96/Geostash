#include "wifi.c"
#include "parsing.c"

#include <stdio.h>
#include <stdlib.h>

#define DOFILE_LEN 36
#define CHECKWIFI_LEN 19
#define HTTP_LEN 31
#define DISTANCE_LEN 9

/**
 * For testing purposes only.
 */
int wifiTest(void) {
  //create array for various commands
  char http_response[HTTP_REQUEST_MAX_LENGTH];
  char *distance = "1943.5000000009i82828283";
  char *ip = "";
  int i, j;

  Init_WiFi(ip);
  delay(1.0);

  if (PARSING_DEBUG)
    printf("ip from init wifi: %s\n", ip);

  if (PARSING_DEBUG)
    printf("Got response\nWaiting for http_request\n");

	http_response[0] = getcharWiFi();
	i = 1;
  while (http_response[i-1] != '$') {
    http_response[i] = getcharWiFi();
		i++;
  }
  http_response[i] = '\0';

  if (PARSING_DEBUG) {
    printf("Got http message: %s of length: %d\nParsing\n", http_response, i);
    for (j = 0; j < i; j++) {
      printf("%c", http_response[j]);
    }
    printf("\n");
  }

  struct http_message message = parseMessage(http_response+2, i);

  // send data to distance calculator

  if (PARSING_DEBUG) {
    printf("request: %s\nlongitude: %lf, latitude: %lf\n", message.request, message.loc.lon, message.loc.lat);
    printf("parsed and distance found\nSending distance\n");
  }

  wifiSendResponse(distance);

  if (PARSING_DEBUG)
    printf("Waiting for > response\n");

  waitForDone();

  if (PARSING_DEBUG)
    printf("Got response\nDone\n");

  return 0;
}
