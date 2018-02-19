#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct dp_packet packet;
    uint8_t *copy = malloc(size);
    memcpy(copy, data, size);
    dp_packet_use_const(&packet, copy, size);
    while (dp_packet_size(&packet) >= sizeof(struct ofp_header)) {
      const struct ofp_header *oh;
      enum ofptype type;
      enum ofperr err;
      void *pdata = dp_packet_data(&packet);
      int length;
      dp_packet_shift(&packet, -((intptr_t) pdata & 7));
      oh = dp_packet_data(&packet);
      length = ntohs(oh->length);
      if (!length || dp_packet_size(&packet) < length)
        break;
      err = ofptype_decode(&type, oh);
      if (err)
        break;

      if (type == OFPTYPE_NXT_TLV_TABLE_REPLY) {
        struct ofputil_tlv_table_reply reply;
        enum ofperr error = ofputil_decode_tlv_table_reply(oh, &reply);
      }
      dp_packet_pull(&packet,length);
    }
    free(copy);
    return 0;
}
