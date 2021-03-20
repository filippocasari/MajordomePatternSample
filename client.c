//
//  Irondomo Protocol  CLEAR client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"

int main(int argc, char *argv[]) {
    int verbose = (argc > 1 && streq (argv[1], "-v"));
    verbose = 1;
    idcli_t *session = idcli_new("tcp://127.0.0.1:5000", "Client", verbose);

    idcli_connect_to_broker(session);

    int count;
    zmsg_t *reply=NULL;
    zmsg_t *request = zmsg_new();
    int64_t start;
    int64_t end;
    start=zclock_time();
    for (count = 0; count < 200; count++) {
        request = zmsg_new();
        int succ = zmsg_pushstr(request, "I wanna a coffee");
        if (succ == -1) {
            puts("ERROR ");
        }
        reply= idcli_send(session, "coffee", &request);

    }
    end=zclock_time()-start;

    printf("%d requests/replies processed\n", count);
    printf("Time for Synchronous Client is : %ld ms", end);
    idcli_destroy(&session);
    return 0;
}
