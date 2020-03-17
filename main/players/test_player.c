#include "../../main/debugprint.h"
#include "../../hardware_utils/wifi/wifi.c"
#include "../../hardware_utils/gps/gps.c"
#include "../../hardware_utils/wifi/parsing.c"

#include "../communication.c"
#include "players.c"


int main (void) {
    //create array for various commands
  char http_response[HTTP_REQUEST_MAX_LENGTH];
  char *ip = "";

  Init_WiFi(ip);
  delay(1.0);

  if (PARSING_DEBUG) printf("ip from init wifi: %s\n", ip);

  if (PARSING_DEBUG) printf("Got response\nWaiting for http_request\n");

	while(1) {
    processWiFi();
  }

  return 0;
}
