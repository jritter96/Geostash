#ifndef _PARSING_H_
#define _PARSING_H_

#include "../gps/gps.h"
#include "../../main/players/players.h"

#define HTTP_REQUEST_MAX_LENGTH 512
#define TERMINATION_CHARACTER '%'


enum WebServerCommsCommand { PING, REGISTER_USER, };
char const*const WEBSERVER_COMMS_CMD_STRINGS[] = { "PING", "RU" };

struct http_message {
    enum WebServerCommsCommand requestType;
    char request[HTTP_REQUEST_MAX_LENGTH];
    struct location loc;
    char username[USERNAME_MAX_LENGTH];
};

struct http_message parseMessage(char* message, int buff_length);


#endif