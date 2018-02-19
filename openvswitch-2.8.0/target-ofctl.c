#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    struct ofputil_meter_mod mm;
    enum ofputil_protocol usable_protocols;

    parse_ofp_meter_mod_str(&mm, data, OFPMC13_ADD, &usable_protocols);
 
    return 0;
}
