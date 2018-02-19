#include "flow.h"
#include "dp-packet.h"
#include "conntrack.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct conntrack ct; 
    conntrack_init(&ct);
    struct dp_packet_batch pkt_batch;
    struct flow flow;
    struct dp_packet pkt;
    dp_packet_use_const(&pkt, data, size);

    packet_batch_init_packet(&pkt_batch, &pkt);
    /* This also initializes the l3 and l4 pointers. */
    flow_extract(pkt_batch.packets[0], &flow);

    conntrack_execute(&ct, &pkt_batch, flow.dl_type, true, 0, NULL, NULL, NULL);

    conntrack_destroy(&ct);
    return 0;
}
