#include <stdint.h>
#include "osipparser2/internal.h"
#include "osipparser2/osip_port.h"
#include "osipparser2/osip_parser.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  osip_message_t *sip;
#if 0
  int err=0;
  int expected_error=OSIP_SUCCESS;
  int pos=0;

  // Filter out expected_error message
  if (osip_strncasecmp(data, "expected_error:", strlen("expected_error:"))==0) {
    pos=strlen("expected_error:");
    data+=pos;
    expected_error=atoi(data);
    while ((pos <= (size-1)) && *data!='\r' && *data!='\n') {
      data++;
      pos++;
    }
    while ((pos <= (size-1)) && (*data=='\r' || *data=='\n')) {
      data++;
      pos++;
    }
  }
  
  if (pos > size)
    return 0;
#endif
  // The actual parsing happens here
  osip_message_init(&sip);
  osip_message_parse (sip, data, size);
  osip_message_free(sip);

  return 0;
}
