// Titanic service
//
// Implements server side of http://rfc.zeromq.org/spec:9

#include "../include/idwrkapi.h"
#include "../include/idcliapi.h"
#include <zfile.h>
#include <uuid/uuid.h>

static char *
s_generate_uuid(void)
{
    char hex_char [] ="0123456789ABCDEF";
    char *uuidstr=zmalloc(sizeof(uuid_t)*2 +1);
    uuid_t uuid;
    uuid_generate(uuid);
    int byte_nbr;
    for(byte_nbr=0; )






}