#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "flow.h"
#include "pcap-file.h"

extern void flow_extract(struct ofpbuf *, const struct pkt_metadata *md, struct flow *);


void write_pcap_header(unsigned int *buf) {
  buf[0] = 0xa1b2c3d4; /* magic */
  buf[1] = 0x00040002; /* version */
  buf[2] = 0; /* thiszone */
  buf[3] = 0; /* sigfigs */
  buf[4] = 65535; /* snaplen */
  buf[5] = 1;     /* eth */
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  int retval;
  FILE *pcap;
  struct flow flow;
  struct ofpbuf *packet;
  uint8_t t[size+24];
  write_pcap_header((unsigned int *)t);
  for (size_t i = 0 ; i < size; i++) {
    t[24+i] = data[i];
  }
  pcap = fopen ("test.pcap", "wb");
  fwrite(t, 1, sizeof(t), pcap);
  fclose(pcap);
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
