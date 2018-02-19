#include "libsoup/soup.h"
#include "libsoup/soup-requester.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

  char *method, *path;
  SoupHTTPVersion version;
  SoupMessageHeaders *headers;
  guint status;

  headers = soup_message_headers_new (SOUP_MESSAGE_HEADERS_REQUEST);
  method = path = NULL;

  status = soup_headers_parse_request (Data, Size, headers, 
					&method, &path,
					&version);
  g_free (method);
  g_free (path);
  soup_message_headers_free (headers);
  return 0;
}
