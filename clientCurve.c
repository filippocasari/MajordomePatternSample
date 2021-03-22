//
//  Irondomo Protocol CURVE client example
//  Uses the idcli API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idcliapi.h"


int main(int argc, char *argv[]) {
    int verbose = (argc > 1 && streq (argv[1], "-v"));
    verbose = 1;

    idcli_t *session = idcli_new("tcp://localhost:5001", "ClientCurve", verbose);
    //@create new certificate for the client and store it
    zcert_t *client_cert = zcert_new();
    zsys_file_mode_private();
    zcert_set_meta(client_cert, "name", "Client Certificate");
    zsys_dir_create(".curve");
    zcert_save(client_cert, ".curve/client_cert.txt");

    assert (zsys_file_exists(".curve/client_cert.txt"));

    //@extract secret and public key form client cert
    const char *client_public = zcert_public_txt(client_cert);
    zclock_log("Public client key: %s", client_public);
    const char *client_secret = zcert_secret_txt(client_cert);
    zclock_log("Secret client key: %s", client_secret);
    zcert_destroy(&client_cert);

    //@well-know server pub key
    const char *server_public;
    if (zsys_file_exists(".curveServer/server_cert.txt")) {
        zcert_t *server_cert = zcert_load(".curveServer/server_cert.txt");
        server_public = zcert_public_txt(server_cert);
    } else {
        zsys_error("No server public key...exit");
        return 2;
    }


    //const char *server_public = ".8Q^k*3E/4-Wg4()r^(4yTk2>qvZFDW?mXUyRPvr";

    idcli_setup_curve(session, client_public, client_secret, server_public);

    idcli_connect_to_broker(session);

    int count;
    zmsg_t *reply = NULL;
    zmsg_t *request;

    int64_t start;
    int64_t end;
    start = zclock_time();


    for (count = 0; count < 200; count++) {

        request = zmsg_new();
        zmsg_pushstr(request, "I wanna a secure coffee");

        reply = idcli_send(session, "coffeeCurve", &request);
        if (reply == NULL) {
            puts("NO REPLY FOR THIS SERVICE...\n");
            break;
        }

    }
    end = zclock_time() - start;
    printf("%d requests/replies processed\n", count);
    printf("Time for Synchronous Curve Client is : %ld ms", end);
    idcli_destroy(&session);

    return 0;
}

