#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/dynamic-string.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    int error;
    ovs_u128 ufid;

    /* Initialize ds from fuzzer in */
    struct ds in;
    ds_init(&in);
    ds_put_buffer(&in, (const char *)data, size);

    odp_ufid_from_string(ds_cstr(&in), &ufid);

    ds_destroy(&in);
    return 0;
}
