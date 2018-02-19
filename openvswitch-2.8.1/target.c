#include "flow.h"
#include "dp-packet.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct dp_packet packet;
    dp_packet_use_const(&packet, data, size);

    struct flow flow;
    pkt_metadata_init(&packet.md, u32_to_odp(ofp_to_u16(OFPP_ANY)));
    flow_extract(&packet, &flow);
    flow_print(stdout, &flow, NULL);

    return 0;
}
