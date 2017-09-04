#include "openvswitch/json.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size > 0 &&
        data[size-1] == '\0') {
        struct json *j = json_from_string(data);
        json_destroy(j);
    }
    return 0;
}
