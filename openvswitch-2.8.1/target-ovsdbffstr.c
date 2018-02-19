#include "openvswitch/dynamic-string.h"
#include "ovsdb-condition.h"
#include "ovsdb-error.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size == 0 || data[size-1] != '\0')
      return 0;

    /* Initialize ds from fuzzer in */
    struct ds in;
    ds_init(&in);
    ds_put_buffer(&in, (const char *)data, size);

    enum ovsdb_function function;
    struct ovsdb_error *error = ovsdb_function_from_string(ds_cstr(&in), &function);
    if (error) ovsdb_error_destroy(error);
    ds_destroy(&in);
    return 0;
}
    
