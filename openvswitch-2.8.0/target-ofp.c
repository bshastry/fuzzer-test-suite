#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"

static bool
is_openflow_port(ovs_be16 port_)
{
    uint16_t port = ntohs(port_);
    return port == OFP_PORT || port == OFP_OLD_PORT;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct dp_packet packet;
    struct flow flow;
    struct tcp_reader *reader;
    dp_packet_use_const(&packet, data, size);

    pkt_metadata_init(&packet.md, ODPP_NONE);
    flow_extract(&packet, &flow);
    if (flow.dl_type == htons(ETH_TYPE_IP)
        && flow.nw_proto == IPPROTO_TCP
        && (is_openflow_port(flow.tp_src) ||
            is_openflow_port(flow.tp_dst))) {
            struct dp_packet *payload = tcp_reader_run(reader, &flow, &packet);
            if (payload) {
                while (dp_packet_size(payload) >= sizeof(struct ofp_header)) {
                    const struct ofp_header *oh;
                    void *pdata = dp_packet_data(payload);
                    int length;

                    /* Align OpenFlow on 8-byte boundary for safe access. */
                    dp_packet_shift(payload, -((intptr_t) pdata & 7));

                    oh = dp_packet_data(payload);
                    length = ntohs(oh->length);
                    if (dp_packet_size(payload) < length) {
                        break;
                    }

                   ofp_print(stdout, dp_packet_data(payload), length, 4);
                   dp_packet_pull(payload, length);
                }
            }
    	    tcp_reader_close(reader);
	}
    return 0;
}
