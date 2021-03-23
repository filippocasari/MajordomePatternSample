//
//  Irondomo Protocol  CLEAR client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"
// raspberry endpoint : "tcp//:192.168.0.113:5000"
//localhost : "tcp//:127.0.0.1:5000"
#define BROKER_ENDPOINT  "tcp//:127.0.0.1:5000"
int main(int argc, char *argv[]) {
    int verbose = (argc > 1 && streq (argv[1], "-v"));
    verbose = 1;
    idcli_t *session2 = idcli_new2(BROKER_ENDPOINT, "ClientAsynchronous", verbose);

    idcli_connect_to_broker(session2);

    int count;
    zmsg_t *reply = NULL;
    zmsg_t *request = zmsg_new();
    int64_t start;
    int64_t end;
    start = zclock_time();
    for (count = 0; count < 200; count++) {
        request = zmsg_new();
        int succ = zmsg_pushstr(request, "I wanna a secure coffee");
        if (succ == -1) {
            puts("ERROR ");
        }
        idcli_send2(session2, "coffee", &request);

    }

    free(reply);

    for (count = 0; count < 200; count++) {


        zmsg_t *reply2 = idcli_recv2(session2);
        if(reply2){
            zmsg_destroy(&reply2);
        }
        if (reply2 == NULL) {
            puts("NO REPLY...");
        }
    }

    end = zclock_time() - start;

    printf("%d requests/replies processed\n", count);
    printf("Time for Synchronous Client is : %ld ms", end);
    idcli_destroy(&session2);
    return 0;
}


