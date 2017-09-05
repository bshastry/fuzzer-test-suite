#include <osipparser2/internal.h>
#include <osipparser2/osip_port.h>
#include <osipparser2/osip_parser.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  osip_message_t *sip;
  int err=0;
  char *result;

  // Filter out expected_error message
  if (osip_strncasecmp(data, "expected_error:", strlen("expected_error:"))==0) {
    data+=strlen("expected_error:");
    expected_error=atoi(data);
    while (*data!='\r' && *data!='\n')
      data++;
    while (*data=='\r' || *data=='\n')
      data++;
  }

  // The actual parsing happens here
  osip_message_init(&sip);
  err = osip_message_parse (sip, data, size);
  osip_message_free(sip);

  return 0;
}
