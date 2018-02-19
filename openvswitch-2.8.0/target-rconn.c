#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0)
      return 0;

    struct ofpbuf buf;
    ofpbuf_use_const(&buf, data, size);
    const struct ofp_header *oh = buf.data;
    enum ofptype type;

    ofptype_decode(&type, oh);
    return 0;
}
