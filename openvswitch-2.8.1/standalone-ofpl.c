#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/ofp-print.h"
#include <unistd.h>

int main()
{
    struct dp_packet packet;
    FILE *fp;
    fp = fopen(argv[1], "rb");
    if (!fp) return -1;
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    uint8_t *copy = malloc(size);
    result = fread(copy, 1, size, fp);
    if (result != size) return -1;
    fclose(fp);
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
