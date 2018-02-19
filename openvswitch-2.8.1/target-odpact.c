#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/dynamic-string.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    struct ofpbuf odp_actions;
    int error;

    /* Initialize ds from fuzzer in */
    struct ds in, out;
    ds_init(&in);
    ds_put_buffer(&in, (const char *)data, size);

    /* Convert string to OVS DP key. */
    ofpbuf_init(&odp_actions, 0);
    error = odp_actions_from_string(ds_cstr(&in), NULL, &odp_actions);
    if (error) goto err;

//    ds_init(&out);
//    format_odp_actions(&out, odp_actions.data, odp_actions.size, NULL);
//    puts(ds_cstr(&out));
//    ds_destroy(&out);

err:
    ofpbuf_uninit(&odp_actions);
    ds_destroy(&in);
    return 0;
}
