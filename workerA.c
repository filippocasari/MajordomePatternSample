//
//  Irondomo Protocol CLEAR worker example
//  Uses the idwrk API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idwrkapi.h"

//Random string for identity
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

int main (int argc, char *argv [])
{
    int verbose = (argc > 1 && streq (argv [1], "-v"));

    char identity[32];
    char random_part[5];
    strcpy(identity, "coffeeService_");
    rand_string(random_part, 5);
    strcat(identity,random_part);
    verbose=1;
    idwrk_t *session = idwrk_new (
            "tcp://127.0.0.1:5000", "coffee", identity, verbose);
    idwrk_set_heartbeat(session, 7500);
    idwrk_connect_to_broker(session);

    zmsg_t *reply;

    while (1) {
        reply=zmsg_new();
        zmsg_pushstr(reply,"here u are" );
        zmsg_t *request = idwrk_recv (session, &reply);

            //char *s;
            //s=zmsg_popstr(request);
            //printf("%s", s);
        if (request == NULL)
            break;

    }
    idwrk_destroy (&session);
    return 0;
}
