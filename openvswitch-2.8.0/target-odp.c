#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/dynamic-string.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    struct ofpbuf odp_key;
    struct ofpbuf odp_mask;
    int error;

    /* Initialize ds from fuzzer in */
    struct ds in, out;
    ds_init(&in);
    ds_put_buffer(&in, (const char *)data, size);

    /* Convert string to OVS DP key. */
    ofpbuf_init(&odp_key, 0);
    ofpbuf_init(&odp_mask, 0);
    error = odp_flow_from_string(ds_cstr(&in), NULL,
                                 &odp_key, &odp_mask);
    if (error) goto err;
    ds_init(&out);
    odp_flow_format(odp_key.data, odp_key.size,
                        odp_mask.data, odp_mask.size, NULL, &out, false);
    puts(ds_cstr(&out));
    ds_destroy(&out);

err:
    ofpbuf_uninit(&odp_key);
    ofpbuf_uninit(&odp_mask);
    ds_destroy(&in);
    return 0;
}
