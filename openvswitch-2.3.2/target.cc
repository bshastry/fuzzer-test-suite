#include <stdio.h>
#include <string.h>
#include "flow.h"
#include "ofpbuf.h"
#include "pcap-file.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  FILE *pcap;
  struct flow flow;
  struct ofpbuf *packet;
  uint8_t t[size+24];
  (unsigned int *)t[0] = 0xa1b2c3d4; /* magic */
  (unsigned int *)t[1] = 0x00040002; /* version */
  (unsigned int *)t[2] = 0; /* thiszone */
  (unsigned int *)t[3] = 0; /* sigfigs */
  (unsigned int *)t[4] = 65535; /* snaplen */
  (unsigned int *)t[5] = 1;     /* eth */
  for (size_t i = 0 ; i < size; i++) {
    (uint8_t *)t[24+i] = data[i];
  }
  fwrite(t, 1, sizeof(t), "test.pcap");
  pcap = fopen("test.pcap", "rb");
  if (!pcap) {
        ovs_fatal(errno, "failed to open %s for reading", "test.pcap");
  }

  retval = ovs_pcap_read_header(pcap);
  if (retval) {
        ovs_fatal(retval > 0 ? retval : 0, "reading pcap header failed");
  }

  retval = ovs_pcap_read(pcap, &packet, NULL);
  if (retval == EOF) {
    ovs_fatal(0, "unexpected end of file reading pcap file");
  } else if (retval) {
    ovs_fatal(retval, "error reading pcap file");
  }

  flow_extract(packet, NULL, &flow);
  unlink("test.pcap"); 
  return 0;
}
