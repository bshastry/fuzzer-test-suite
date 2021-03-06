#include "flow.h"
#include "dp-packet.h"
#include "pcap-file.h"
#include "odp-util.h"
#include "openvswitch/ofpbuf.h"
#include "openvswitch/ofp-print.h"

#ifdef TRACKPF
#include "trackpf.h"
size_t pfcount = 0;
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct ofpbuf b;
    if (size < sizeof(struct ofp_header)) return 0;

    ofpbuf_use_const(&b, data, size);
    for (;;) {
	struct ofp_header *oh;
	size_t length, tail_len;
	void *tail;

	// Check if ofpbuf contains ofp header
	oh = (struct ofp_header *)ofpbuf_at(&b, 0, sizeof *oh);
	if (!oh) break;

	// Check if length is geq than lower bound
	length = ntohs(oh->length);
	if (length < sizeof *oh) break;

	// Check if ofpbuf contains payload
	tail_len = length - sizeof *oh;
	tail = ofpbuf_at(&b, sizeof *oh, tail_len);
	if (!tail) break;

	ofp_print(stdout, ofpbuf_pull(&b, length), length, NULL, NULL, 2);
    }
    ofpbuf_uninit(&b);
    return 0;
}
