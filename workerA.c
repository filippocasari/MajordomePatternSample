//
//  Irondomo Protocol CLEAR worker example
//  Uses the idwrk API to hide all IDP aspects
//

//  Lets us build this source without creating a library
#include "include/idwrkapi.h"
#include "workerTask.h"

//Random string for identity

#define NUM_WORKERS 5
int main (int argc, char *argv [])
{
    //int verbose = (argc > 1 && streq (argv [1], "-v"));

    zactor_t *workers[NUM_WORKERS];
    for(int i=0; i<NUM_WORKERS; i++){
        workers[i]=zactor_new(workerTask, NULL);
    }
    for(int i=0; i<NUM_WORKERS; i++){
        zactor_destroy(&workers[i]);
    }

    return 0;
}
