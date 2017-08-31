#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "flow.h"
#include "pcap-file.h"

void flow_extract(struct ofpbuf *, const struct pkt_metadata *md, struct flow *);

void write_pcap_header(unsigned int *buf) {
  buf[0] = 0xa1b2c3d4; /* magic */
  buf[1] = 0x00040002; /* version */
  buf[2] = 0; /* thiszone */
  buf[3] = 0; /* sigfigs */
  buf[4] = 65535; /* snaplen */
//  buf[5] = 1;     /* eth */
}

void cleanup(FILE *fh) {
  fclose(fh);
  unlink("test.pcap");
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  int retval;
  FILE *pcap;
  struct flow flow;
  struct ofpbuf *packet;
  uint8_t t[size+20];
  write_pcap_header((unsigned int *)t);
  for (size_t i = 0 ; i < size; i++) {
    t[20+i] = data[i];
  }
  pcap = fopen ("test.pcap", "wb");
  fwrite(t, 1, sizeof(t), pcap);
  fclose(pcap);
  pcap = fopen("test.pcap", "rb");

  retval = ovs_pcap_read_header(pcap);
  if (retval) {
//        ovs_fatal(retval > 0 ? retval : 0, "reading pcap header failed");
	cleanup(pcap);
	return 0;
  }

  retval = ovs_pcap_read(pcap, &packet, NULL);
  if (retval == EOF) {
//    ovs_fatal(0, "unexpected end of file reading pcap file");
	cleanup(pcap);
	return 0;
  } else if (retval) {
//    ovs_fatal(retval, "error reading pcap file");
	cleanup(pcap);
	return 0;
  }

  flow_extract(packet, NULL, &flow);
  cleanup(pcap);
  return 0;
}
