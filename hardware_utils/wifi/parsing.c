#include "parsing.h"

#include "../../main/players/players.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PARSING_DEBUG 0


struct http_message parseMessage(char* message, int buff_length) {
    int i = 0;
    struct http_message parsed_data;
    parsed_data.requestType = -1;

    if (PARSING_DEBUG) printf("message inside parser: %s\n", message);

    int req_index = 0;

    while (message[i] != TERMINATION_CHARACTER && i < buff_length) {
        int alpha = isalpha((int) message[i]);

        if (PARSING_DEBUG) printf("char: %c, alpha: %d\n", message[i], alpha);

        if (alpha && message[i] != 'n' && message[i] != 'i' && message[i] != 'l') {
            parsed_data.request[req_index] = message[i];
            req_index ++;
        }
        if (PARSING_DEBUG) printf("i: %d\n", i);

        i ++;
    }
    parsed_data.request[req_index] = '\0';
    i ++;

    if (strcmp(parsed_data.request, WEBSERVER_COMMS_CMD_STRINGS[PING]) == 0) {
        parsed_data.requestType = PING;
        int num_buf_index = 0;
        char num_buf[HTTP_REQUEST_MAX_LENGTH];
        char* ptr;
        if (PARSING_DEBUG)printf("Message confirmed PING\n");

        while (message[i] != TERMINATION_CHARACTER && i < buff_length) {
            num_buf[num_buf_index] = message[i];
            i ++;
            num_buf_index ++;
        }
        num_buf[num_buf_index] = '\0';
        i ++;
        num_buf_index = 0;
        if (PARSING_DEBUG) printf("longitude string: %s", num_buf);
        parsed_data.loc.lon = strtod(num_buf, &ptr);
        if (PARSING_DEBUG) printf("longitude number form: %lf\n", parsed_data.loc.lon);

        while (message[i] != TERMINATION_CHARACTER && i < buff_length) {
            num_buf[num_buf_index] = message[i];
            i ++;
            num_buf_index ++;
        }
        num_buf[num_buf_index] = '\0';
        if (PARSING_DEBUG) printf("latitude string: %s", num_buf);
        parsed_data.loc.lat = strtod(num_buf, &ptr);
        if (PARSING_DEBUG) printf("latitude number form: %lf\n", parsed_data.loc.lat);

    } else if (strcmp(parsed_data.request, WEBSERVER_COMMS_CMD_STRINGS[REGISTER_USER]) == 0) {
        parsed_data.requestType = REGISTER_USER;
        int num_buf_index = 0;
        char num_buf[HTTP_REQUEST_MAX_LENGTH];
        while (message[i] != TERMINATION_CHARACTER && i < buff_length) {
            num_buf[num_buf_index] = message[i];
            i ++;
            num_buf_index ++;
        }
        num_buf[num_buf_index] = '\0';
        i ++;
        num_buf_index = 0;

        strcpy(parsed_data.username, num_buf);

    }

    return parsed_data;
}
