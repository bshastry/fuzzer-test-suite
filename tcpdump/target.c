#include "netdissect-stdinc.h"
#include "netdissect.h"
#include "pcap.h"
#include "print.h"
#include <string.h>
#include <stdio.h>

//#define SWITCH

#ifndef SWITCH
netdissect_options Ndo;
netdissect_options *ndo = &Ndo;
#endif

#ifdef SWITCH
void printert_init(netdissect_options *ndo) {
    char ebuf[PCAP_ERRBUF_SIZE];

    if (nd_init(ebuf, sizeof ebuf) == -1)
        printf("%s", ebuf);
    memset(ndo, 0, sizeof(*ndo));
    ndo_set_function_pointers(ndo);
    init_print(ndo, 0, 0, 0);

    ndo->program_name = "tcpdump-libfuzzer";
    // Don't convert address to names AND verbose output
    ++ndo->ndo_nflag;
    ++ndo->ndo_vflag;
}
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    struct pcap_pkthdr h;
    int dlt = 0;
    if_printer printer;
#ifdef SWITCH
    netdissect_options Ndo;
    char ebuf[PCAP_ERRBUF_SIZE];
    netdissect_options *ndo = &Ndo;
    printert_init(ndo);
#endif

    // We need at least eth frame to continue
    if (size < 18 || size > 65535)
        return 0;

    // Init
    h.caplen = size;
    h.len = size;
    h.ts = (struct timeval){0};
    
    // Get datalink type for printer and print packet
//    dlt = ntohs((const uint16_t *)data[12]);
    // Hard-coded data linktype
    dlt = DLT_EN10MB;
//    printer = lookup_printer(dlt);
//    if (printer == NULL)
//	return 0;

    ndo->ndo_if_printer = get_if_printer(ndo, dlt);
    pretty_print_packet(ndo, &h, data, 1);

    nd_cleanup();
    return 0;
}

#ifndef SWITCH
int
LLVMFuzzerInitialize(int *argc, char ***argv)
{
    char ebuf[PCAP_ERRBUF_SIZE];

    if (nd_init(ebuf, sizeof ebuf) == -1)
        printf("%s", ebuf);
    memset(ndo, 0, sizeof(*ndo));
    ndo_set_function_pointers(ndo);
    init_print(ndo, 0, 0, 0);

    ndo->program_name = "tcpdump-libfuzzer";
    // Don't convert address to names AND verbose output
    ++ndo->ndo_nflag;
    ++ndo->ndo_vflag;
    return 0;
}
#endif
