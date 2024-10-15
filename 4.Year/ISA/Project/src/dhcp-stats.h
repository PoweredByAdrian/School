/*
 * File: dhcp-stats.h
 * Author: Adrian Horvath
 * Created on: 20.11.2023
 * Description: Header file for dhcp-stats.cpp containing function prototypes and structure definitions.
 */

#ifndef DHCP_STATS_H
#define DHCP_STATS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <pcap.h>
#include <stdint.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <cstdint>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <syslog.h>
#include <csignal>
#include <netinet/in.h>
#include <arpa/inet.h>

//Enums
enum DhcpMessageType {
    DHCPDISCOVER = 1,
    DHCPOFFER,
    DHCPREQUEST,
    DHCPDECLINE,
    DHCPACK,
    DHCPNAK,
    DHCPRELEASE,
    DHCPINFORM
};

enum DhcpOptions{
    Pad = 0,
    SubnetMask,
    TimeOffset,
    Router,
    TimeServer,
    NameServer,
    DomainServer,
    LogServer,
    QuotesServer,
    LPRServer,
    ImpressServer,
    RLPServer,
    Hostname,
    BootFileSize,
    MeritDumpFile,
    DomainName,
    SwapServer,
    RootPath,
    ExtensionFile,
    Forward,
    SrcRte,
    PolicyFilter,
    MaxDGAssembly,
    DefaultIPTTL,
    MTUTimeout,
    MTUPlateau,
    MTUInterface,	
    MTUSubnet,
    BroadcastAddress,
    MaskDiscovery,
    MaskSupplier,
    RouterDiscovery,
    RouterRequest,
    StaticRoute,
    Trailers,
    ARPTimeout	,
    Ethernet,
    DefaultTCPTTL,
    KeepaliveTime,
    KeepaliveData,
    NISDomain,
    NISServers,
    NTPServers,
    VendorSpecific,
    NETBIOSName,
    NETBIOSDist,
    NETBIOSNode,
    NETBIOSScope,
    XWindowFont,
    XWindowManager,
    AddressRequest,
    AddressTime,
    Overload,
    DHCPMsgType,
    DHCPServerId,
    ParameterList,
    DHCPMessage,
    DHCPMaxMsgSize,
    RenewalTime,
    RebindingTime,
    ClassId,
    ClientId,
    NetWareIPDomain,
    NetWareIPOption,
    NISDomainName,
    NISServerAddr,
    ServerName,
    BootfileName,
    HomeAgentAddr,
    SMTPServer,
    POP3Server,
    NNTPServer,
    WWWServer,
    FingerServer,
    IRCServer,
    StreetTalkServer,
    STDAServer
};


//Data structures

/**
 * @brief Struct to hold command line arguments.
 */
struct CommandLineArgs
{
    char *interface;        ///< Network interface name.
    char *filename;         ///< PCAP file name.
    char **ipPrefixes;      ///< Array of IP prefixes.
    int ipPrefixCount;      ///< Number of IP prefixes.
};

/**
 * @brief Struct to represent a network prefix.
 */
struct NetworkPrefix {
    char prefix[16];            ///< String representing the network prefix (e.g., "192.168.1.0/24").
    uint32_t ip;                ///< IP address of the network prefix.
    int prefix_length;          ///< Length of the prefix (e.g., 24 for the example above).
    uint32_t allocated;         ///< Count of allocated IP addresses in the prefix.
    uint32_t maxAddress;        ///< Maximum address in the prefix.
    uint32_t mask;              ///< Subnet mask of the prefix.
    size_t MAX_PREFIXES;        ///< Maximum number of prefixes.
};

/**
 * @brief Structure representing a DHCP packet.
 */
struct dhcp_packet {
    uint8_t op;              ///< Message opcode: 1 for request, 2 for reply.
    uint8_t htype;           ///< Hardware address type.
    uint8_t hlen;            ///< Hardware address length.
    uint8_t hops;            ///< Number of hops.
    uint32_t xid;            ///< Transaction ID.
    uint16_t secs;           ///< Seconds elapsed.
    uint16_t flags;          ///< Flags.
    uint32_t ciaddr;         ///< Client IP address.
    uint32_t yiaddr;         ///< Your client IP address.
    uint32_t siaddr;         ///< Server IP address.
    uint32_t giaddr;         ///< Relay agent IP address.
    uint8_t chaddr[16];      ///< Client hardware address.
    uint8_t sname[64];       ///< Server host name.
    uint8_t file[128];       ///< Boot filename.
    uint32_t magic_cookie;   ///< DHCP magic cookie.

    // Options follow here
    uint8_t options[576];    ///< Array to store all options.
};

/**
 * @brief Hash function object for hashing uint32_t IP addresses.
 */
struct IpHash {
    /**
     * @brief Calculates the hash value for a uint32_t IP address.
     *
     * @param ip The IP address to hash.
     * @return std::size_t The calculated hash value.
     */
    std::size_t operator()(const uint32_t& ip) const {
        // Use a simple hash function for demonstration
        return std::hash<uint32_t>{}(ip);
    }
};


// Function declarations
NetworkPrefix* getPrefixes(int maxPrefixes);
bool isValidPrefix(const char* prefix);
int argumentCheck(int argc, char *argv[], struct CommandLineArgs *args);
void notifyAdministrator(const char *prefix, int lenght);
void printHeader();
void printPrefixes();
dhcp_packet* getPacket();
int printOptions(struct dhcp_packet *packet);
void printIpAddress(uint32_t ipAddress);
bool isIPInPrefix(uint32_t ipAddress, const NetworkPrefix& prefix);
void modifyDataForMatchingPrefix(uint32_t ipAddress);
void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet);
int openFile(const char *filename);
int openLiveCapture(const char *interface);
void initializePrefixes(int MAX_PREFIXES, char** prefixes);
pcap *getHandle(pcap* handle);
int setPacketFilter(pcap_t* handle);
void ncursesInit();



#endif  // DHCP_STATS_H