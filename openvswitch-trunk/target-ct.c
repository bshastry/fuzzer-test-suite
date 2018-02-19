#include "flow.h"
#include "dp-packet.h"
#include "conntrack.h"
#include "timeval.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct conntrack ct; 
    conntrack_init(&ct);
    struct dp_packet_batch pkt_batch;
    struct flow flow;
    struct dp_packet pkt;
    dp_packet_use_const(&pkt, data, size);
    long long now = time_msec();

    dp_packet_batch_init(&pkt_batch);
    dp_packet_batch_add(&pkt_batch, &pkt);
    /* This also initializes the l3 and l4 pointers. */
    flow_extract(&pkt, &flow);

    conntrack_execute(&ct, &pkt_batch, flow.dl_type, false, true, 0, NULL, NULL, 0, 0, NULL, NULL, now);

    conntrack_destroy(&ct);
    return 0;
}
