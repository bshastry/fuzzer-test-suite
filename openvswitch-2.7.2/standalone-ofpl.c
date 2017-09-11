#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/ofp-print.h"
#include <unistd.h>

int main()
{
    struct dp_packet packet;
    uint8_t copy[2048];
    memset(copy, 0, 2048);
    size_t size = read(0, copy, 2048);
    uint8_t *temp = malloc(size);
    memcpy(temp, copy, size);
    dp_packet_use_const(&packet, temp, size);
    while (dp_packet_size(&packet) >= sizeof(struct ofp_header)) {
      const struct ofp_header *oh;
      void *pdata = dp_packet_data(&packet);
      int length;
      dp_packet_shift(&packet, -((intptr_t) pdata & 7));
      oh = dp_packet_data(&packet);
      length = ntohs(oh->length);
      if (!length || dp_packet_size(&packet) < length)
        break;
      ofp_print(stdout, oh, length, 4);
      dp_packet_pull(&packet, length);
    }
    free(temp);
    return 0;
}
