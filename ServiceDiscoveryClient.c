//
//  Irondomo Protocol  CLEAR client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"
// raspberry endpoint : "tcp//:192.168.0.113:5001"
//localhost : "tcp//:127.0.0.1:5555"
#define BROKER_ENDPOINT_PLAIN  "tcp//:127.0.0.1:5000"

int main(int argc, char *argv[]) {

    int verbose = (argc > 1 && streq (argv[1], "-v"));
    verbose = 1;
    idcli_t *session = idcli_new(BROKER_ENDPOINT_PLAIN, "ServiceDiscoveryClient", verbose);

    idcli_connect_to_broker(session);


    zmsg_t *reply = NULL;
    zmsg_t *request = zmsg_new();


    request = zmsg_new();

    zmsg_pushstr(request, "coffee");
    int64_t start;
    int64_t end;
    start = zclock_time();
    int64_t start2 = zclock_mono();
    reply = idcli_send(session, "mmi.service", &request);

    char *code_response = zmsg_popstr(reply);

    if (strcmp("200", code_response) == 0) {
        zclock_log("contacting the service coffee...");
        request = zmsg_new();
        zmsg_pushstr(request, "I've discovered you!");
        reply = idcli_send(session, "coffee", &request);



    }

    end = zclock_time() - start;
    long int end2 = zclock_mono() - start2;
    printf("Time for Service Discovery is : \t%ld ms\n", end);
    printf("Time for Service Discovery is (with monoclock) :\t %ld ms", end2);
    idcli_destroy(&session);
    return 0;
}


