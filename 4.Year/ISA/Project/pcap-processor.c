#include <stdio.h>
#include <pcap.h>

void packetHandler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Process and analyze the packet here.
    // pkthdr contains packet metadata (timestamp, length).
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pcap-file>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_offline(filename, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Error opening PCAP file: %s\n", errbuf);
        return 1;
    }

    if (pcap_loop(handle, 0, packetHandler, NULL) < 0) {
        fprintf(stderr, "Error in pcap_loop()\n");
        return 1;
    }

    pcap_close(handle);

    return 0;
}