#include "include/idbrokerapi.h"
/*
Example of broker with no cert store configured
*/

int main(int argc, char *argv[])
{
    int _verbose = (argc > 1 && streq(argv[1], "-v"));
    _verbose=1;
    zcert_t * c ;

    zsys_file_mode_private ();
    if(zsys_file_exists(".curveServer/server_cert.txt")){
        c=zcert_load(".curveServer/server_cert.txt");
    }else{
        c=zcert_new();
        zcert_set_meta(c, "name", "Server Certificate");
        zsys_dir_create(".curveServer");
        zcert_save(c, ".curveServer/server_cert.txt");
    }
    const char *secret_key=zcert_secret_txt(c);
    zclock_log("I: secret_key: %s", zcert_secret_txt(c));
    const char *public_key =zcert_public_txt(c);
    zclock_log("I: public_key: %s", public_key);

    // char public_key[] = ".8Q^k*3E/4-Wg4()r^(4yTk2>qvZFDW?mXUyRPvr";
    //const char secret_key[] = "3vup%:I!lF>^QWT@[[g]dwa>1:(B-^3RWw^7tIMf";


    broker_t *self = s_broker_new("tcp://127.0.0.1:5000", "tcp://127.0.0.1:5001", public_key, secret_key, NULL, _verbose);

    s_broker_loop(self);

    if (zctx_interrupted)
        printf("W: interrupt received, shutting down...\n");

    s_broker_destroy(&self);
    zcert_destroy(&c);
    zsys_file_delete("./curveServer/server_cert.txt");
    zsys_file_delete("./curveServer/server_cert.txt_secret");
    zsys_dir_delete("./curveServer");

    return 0;
}
