#include "openvswitch/dynamic-string.h"
#include "nx-match.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    struct ofpbuf nx_match;
    int error;

    /* Initialize ds from fuzzer in */
    struct ds in;
    ds_init(&in);
    ds_put_buffer(&in, (const char *)data, size);

    nx_match_from_string(ds_cstr(&in), &nx_match);

    ds_destroy(&in);
    return 0;
}
