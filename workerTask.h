//
// Created by filippocasari on 22/03/21.
//
#include <stdio.h>
#ifndef MAJORDOMEPATTERNSAMPLE_WORKERTASK_H
#define MAJORDOMEPATTERNSAMPLE_WORKERTASK_H
// raspberry endpoint : "tcp//:192.168.0.113:5001"
//localhost : "tcp//:127.0.0.1:5555"
#define BROKER_ENDPOINT_SECURE  "tcp//:127.0.0.1:5001"
#define BROKER_ENDPOIN_PLAIN "tcp://127.0.0.1:5000"

char *rand_string(char *str, size_t size)
{
    unsigned char data[size];
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&data, 1, size, fp);
    fclose(fp);
    const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            unsigned char  key = data[n] % (unsigned char) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}
// #TODO pass to args if worker task has to connect to secure or plain port
static void
workerTask(zsock_t *pipe, void *args){
        char identity[32];
        char random_part[5];
        strcpy(identity, "coffeeService_");
        rand_string(random_part, 5);
        strcat(identity,random_part);
        zclock_log("Worker %s",identity);
        int verbose=0;
        idwrk_t *session = idwrk_new (
        BROKER_ENDPOIN_PLAIN, "coffee", identity, verbose);
        idwrk_set_heartbeat(session, 7500);
        idwrk_connect_to_broker(session);

        zmsg_t *reply;

        while (1) {
            reply=zmsg_new();
            zmsg_pushstr(reply,"here u are" );
            zmsg_t *request = idwrk_recv (session, &reply);


            if (request == NULL)
                break;

        }
        idwrk_destroy (&session);

}






#endif //MAJORDOMEPATTERNSAMPLE_WORKERTASK_H
