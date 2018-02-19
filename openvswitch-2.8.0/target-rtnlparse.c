#include "openvswitch/ofpbuf.h"
#include "rtnetlink.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

    if (size < 16) return 0;

    struct ofpbuf buf;
    struct rtnetlink_change change;

    ofpbuf_use_const(&buf, data, size);

    rtnetlink_parse(&buf, &change);

    return 0;
}
