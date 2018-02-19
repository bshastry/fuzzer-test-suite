#include "flow.h"
#include "dp-packet.h"
#include "ofp-print.h"
#include "ofp-util.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct dp_packet packet;
    dp_packet_use_const(&packet, data, size);

    struct flow flow;
    struct match match;
    struct ofp10_match expected_match, extracted_match;
    memset(&expected_match, 0, sizeof(struct ofp10_match));

    pkt_metadata_init(&packet.md, u32_to_odp(ofp_to_u16(OFPP_ANY)));
    flow_extract(&packet, &flow);
    flow_print(stdout, &flow);

    flow.in_port.ofp_port = u16_to_ofp(1);
    match_wc_init(&match, &flow);

    return 0;
}
