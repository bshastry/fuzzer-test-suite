#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/ofp-print.h"

#ifdef TRACKPF
#include "trackpf.h"
size_t pfcount = 0;
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct dp_packet packet;
    uint8_t *copy = malloc(size);
    memcpy(copy, data, size);
    dp_packet_use_const(&packet, copy, size);
    while (dp_packet_size(&packet) >= sizeof(struct ofp_header)) {
      const struct ofp_header *oh;
      void *pdata = dp_packet_data(&packet);
      int length;
      dp_packet_shift(&packet, -((intptr_t) pdata & 7));
      oh = dp_packet_data(&packet);
      length = ntohs(oh->length);
      if (!length || dp_packet_size(&packet) < length)
        break;
      ofp_print(stdout, oh, length, NULL, 4);
      dp_packet_pull(&packet, length);
    }
    free(copy);
    return 0;
}
