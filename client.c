//
//  Irondomo Protocol  CLEAR client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"
#include <uuid/uuid.h>

#define NUM_CLIENTS 5

static void
client_task(zsock_t *pipe, void *args);

int main(int argc, char *argv[]) {
    //int verbose = (argc > 1 && streq (argv[1], "-v"));
    zactor_t *clients[NUM_CLIENTS];
    for (int i = 0; i < NUM_CLIENTS; i++) {
        clients[i] = zactor_new(client_task, NULL);
        if(zctx_interrupted){
            zclock_log("error signal handled...");
        }

    }
    for (int i = 0; i < NUM_CLIENTS; i++) {
        zactor_destroy(&clients[i]);

    }



    return 0;
}


static void
client_task(zsock_t *pipe, void *args) {
    //zsock_signal(pipe, 0);
    int verbose = 0;
    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuid_str[37];
    uuid_unparse_upper(uuid, uuid_str);
    zclock_log("UUID CLIENT: %s", uuid_str);
    idcli_t *session = idcli_new("tcp://127.0.0.1:5000", uuid_str, verbose);

    idcli_connect_to_broker(session);

    int count;
    zmsg_t *reply = NULL;
    zmsg_t *request = zmsg_new();
    int64_t start;
    int64_t end;
    start = zclock_time();
    for (count = 0; count < 200; count++) {
        request = zmsg_new();
        assert(request);
        int succ = zmsg_pushstr(request, "I wanna a coffee");
        if (succ == -1) {
            zclock_log("ERROR ");
        }
        reply = idcli_send(session, "coffee", &request);
        if (reply == NULL) {
            zsys_error("No reply...destroying the client...");
            idcli_destroy(&session);

        }
    }
    end = zclock_time() - start;

    zclock_log("%d requests/replies processed", count);
    zclock_log("Time for Synchronous Client is : %ld ms\n", end);
    idcli_destroy(&session);
}
