#include "flow.h"
#include "ofpbuf.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct ofpbuf packet;
    ofpbuf_use_const(&packet, data, size);

    struct flow flow;
    flow_extract(&packet, NULL, &flow);

    return 0;
}
