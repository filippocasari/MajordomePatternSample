//
//  Irondomo Protocol CURVE client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"
#define CERTDIR "cert_store"

int main (int argc, char *argv [])
{
    int verbose = (argc > 1 && streq (argv [1], "-v"));
    verbose=1;
    idcli_t *session = idcli_new ("tcp://localhost:5001", "ClientCurve", verbose);

    zcert_t *c = zcert_new();
    zcert_save_public(c, CERTDIR"/client_cert.txt");

    const char *client_public = zcert_public_txt(c);
    const char *client_secret = zcert_secret_txt(c);
    const char *server_public = ".8Q^k*3E/4-Wg4()r^(4yTk2>qvZFDW?mXUyRPvr";

    idcli_setup_curve(session, client_public, client_secret, server_public);

    idcli_connect_to_broker(session);

    int count;
    zmsg_t *reply=NULL;
    zmsg_t *request;

    int64_t start;
    int64_t end;
    start = zclock_time();


    for (count = 0; count < 200; count++) {

        request = zmsg_new ();
        zmsg_pushstr (request, "I wanna a secure coffee");

        reply = idcli_send (session, "coffeeCurve", &request);
        if(reply==NULL){
            puts("NO REPLY FOR THIS SERVICE...\n");
            break;
        }

    }
    end = zclock_time() - start;
    printf ("%d requests/replies processed\n", count);
    printf("Time for Synchronous Curve Client is : %ld ms", end);
    idcli_destroy (&session);
    zcert_destroy(&c);
    return 0;
}

