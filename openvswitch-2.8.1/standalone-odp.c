#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/ofp-print.h"
#include <unistd.h>

int main()
{
    struct ds in;
    struct ofpbuf odp_key, odp_mask;
    int error;
    
    ds_init(&in);
    ofpbuf_init(&odp_key, 0);
    ofpbuf_init(&odp_mask, 0);

    uint8_t copy[2048];
    memset(copy, 0, 2048);
    size_t size = read(0, copy, 2048);
    uint8_t *temp = malloc(size);
    memcpy(temp, copy, size);
    ds_put_buffer(&in, (const char *)temp, size);
    error = odp_flow_from_string(ds_cstr(&in), NULL, &odp_key, &odp_mask);
    ofpbuf_uninit(&odp_key);
    ofpbuf_uninit(&odp_mask);
    ds_destroy(&in);
    free(temp);
    return 0;
}
