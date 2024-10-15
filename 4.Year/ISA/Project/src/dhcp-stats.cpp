/*
 * File: dhcp-stats.cpp
 * Author: Adrian Horvath (xhorva14)
 * Created on: 20.11.2023
 * Description: BAnalyzes DHCP traffic, capturing and reporting IP address allocation.
 */

#include "dhcp-stats.h"

//TODO DEBUG
//int count = 0;

/**
 * @brief Handles program exit, closing resources and terminating the program.
 * 
 * @param flag Flag indicating the exit status and whether a pcap handle is open.
 *             - 0: Pcap handle open, successful exit.
 *             - 1: No pcap handle open, failure exit.
 *             - 2: Pcap handle open, failure exit.
 */
void myExitFunction(int flag){

    // Terminate the ncurses library
    endwin();

    switch (flag) {
    case 0:
        pcap_close(getHandle(NULL));
        exit(EXIT_SUCCESS);
    case 1:
        exit(EXIT_FAILURE);
    case 2:
        pcap_close(getHandle(NULL));
        exit(EXIT_FAILURE);
    default:
        exit(EXIT_FAILURE);
    }

}

/**
 * @brief Parses command line arguments and populates a structure with the specified options.
 *
 * This function uses getopt to parse command line options, checks for valid combinations, and sets
 * the options in the CommandLineArgs structure.
 *
 * @param argc The number of command line arguments.
 * @param argv An array containing the command line arguments.
 * @param args A pointer to the CommandLineArgs structure to store parsed options.
 * @return Returns 0 on success, and 1 on failure.
 */
int argumentCheck(int argc, char *argv[], struct CommandLineArgs *args)
{
    int opt;
    bool r_flag = false;
    bool i_flag = false;

    opterr = 0;

    // Initialize filename and interface to NULL
    args->filename = NULL;
    args->interface = NULL;

    while ((opt = getopt(argc, argv, "i:r:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            // Check for conflicting options
            if (r_flag || i_flag)
            {
                std::cerr << "Error: Cannot use both -i and -r options." << std::endl;
                return 1;
            }
            args->interface = optarg;
            i_flag = true;
            break;
        case 'r':
            // Check for conflicting options
            if (r_flag || i_flag)
            {
                std::cerr << "Error: Cannot use both -i and -r options." << std::endl;
                return 1;
            }
            args->filename = optarg;
            r_flag = true;
            break;
        case '?':
            // Check for missing arguments or unknown options
            if (optopt == 'i' || optopt == 'r')
            {
                std::cerr << "Option -" << static_cast<char>(optopt) << " requires an argument." << std::endl;
            }
            else
            {
                std::cerr << "Unknown option -" << static_cast<char>(optopt) << "." << std::endl;
            }
            return 1;
        default:
            abort();
        }
    }

    // Check if either -i or -r is specified
    if (!(r_flag || i_flag)) {
        std::cerr << "Error: Either -i or -r must be specified." << std::endl;
        return 1;
    }

    // Check for at least one IP prefix
    if (optind >= argc)
    {
        std::cerr << "Usage: " << argv[0] << "[-r <filename>] [-i <interface-name>] <ip-prefix> [ <ip-prefix> [ ... ] ]" << std::endl;
        return 1;
    }

    // Set the IP prefixes
    args->ipPrefixCount = argc - optind;
    args->ipPrefixes = &argv[optind];

    // Validate the format of each IP prefix
    for (int i = 0; i < args->ipPrefixCount; i++)
    {
        if (isValidPrefix(args->ipPrefixes[i]) == false)
        {
            std::cerr << "Error: Invalid IP prefix format\n";
            return 1;
        }

    }

    return 0;
}

/**
 * @brief Validates the format of an IP prefix.
 *
 * This function checks whether the provided IP prefix follows the required format:
 * A valid IP prefix should consist of four octets separated by dots, followed by a subnet length
 * separated by a forward slash (e.g., "192.168.1.1/24"). Each octet and the subnet length should
 * be within the valid range.
 *
 * @param prefix The IP prefix to be validated.
 * @return true if the prefix is valid, false otherwise.
 */
bool isValidPrefix(const char* prefix) {

    // Make a copy of the prefix since strtok modifies the original string
    char* prefixCopy = new char[strlen(prefix) + 1];
    strcpy(prefixCopy, prefix);

    // Tokenize the copied string by '.' and '/'
    char* token = strtok(prefixCopy, "./");

    int count = 0;

    // Array to store the octets and subnet length
    int parts[5];

    while (token != nullptr) {
        if (count >= 5) {
            // Too many parts in the prefix
            return false;
        }

        // Check if the token consists of valid digits
        char* end;
        long value = strtol(token, &end, 10);
        if (*end != '\0') {
            // Not a valid integer
            return false;
        }

        // Check if the value is within the valid range
        if (value < 0 || value > 255) {
            return false;
        }

        // Store the octet in the array
        parts[count++] = value;

        // Move to the next token
        token = strtok(nullptr, "./");
    }

    // Check if the number of parts is valid (should be 5 for a prefix with subnet length)
    if (count != 5) {
        return false;
    }

    // Check if the subnet length is valid
    if (count == 5 && (parts[4] < 0 || parts[4] > 32)) {
        return false;
    }

    return true;
}

/**
 * @brief Notifies the administrator about a prefix exceeding 50% of allocations.
 * 
 * This function logs a warning message to syslog, indicating that the specified IP prefix
 * has exceeded 50% of allocations.
 * 
 * @param prefix The IP prefix that exceeded 50% of allocations.
 * @param length The subnet length associated with the IP prefix.
 */
void notifyAdministrator(const char *prefix, int lenght) {
    
    // Log to syslog
    openlog("dhcp-stats", LOG_CONS | LOG_PID, LOG_USER);

    // Generate a syslog warning message
    syslog(LOG_WARNING, "prefix %s/%d  exceeded 50%% of allocations .", prefix, lenght);

    // Close syslog connection
    closelog();
}

/**
 * @brief Prints the header for displaying IP prefix information.
 * 
 * This function prints the header format for displaying information about IP prefixes,
 * including columns for IP-Prefix, Max-hosts, Allocated addresses, and Utilization.
 */
void printHeader() {
    printw("IP-Prefix\tMax-hosts\tAllocated addresses\tUtilization\n"); //TODO
}

/**
 * @brief Prints information about IP prefixes, including utilization status.
 * 
 * This function clears the screen, prints the header, and then iterates through
 * the list of IP prefixes, displaying details such as IP-Prefix, Max-hosts,
 * Allocated addresses, and Utilization. If the utilization exceeds a predefined
 * threshold (50%), it notifies the administrator via syslog and adds a message
 * to the printed output.
 */
void printPrefixes() {
    clear();
    printHeader();

    // Get the array of NetworkPrefix structures
    NetworkPrefix* prefixes = getPrefixes(0);

    // Define the utilization threshold for notifying the administrator
    double utilizationThreshold = 50.0;

    for (size_t i = 0; i < prefixes->MAX_PREFIXES; ++i) {
        const NetworkPrefix &prefix = prefixes[i];
        double utilization = (prefix.maxAddress == 0) ? utilization = 0.0 : (static_cast<double>(prefix.allocated) / prefix.maxAddress) * 100.0;

        if (utilization > utilizationThreshold) {
            // Notify the administrator if utilization exceeds 50%
            notifyAdministrator(prefix.prefix, prefix.prefix_length);

            // Print to standard output with the additional message
            printw("%s/%d\t%u\t\t%u\t\t\t%.2f%%\tPrefix exceeded 50%% of allocations.\n", prefix.prefix, prefix.prefix_length, prefix.maxAddress, prefix.allocated, utilization);
        }
        else
        {
            // Print information without the additional message
            printw("%s/%d\t%u\t\t%u\t\t\t%.2f%%\n", prefix.prefix,prefix.prefix_length, prefix.maxAddress, prefix.allocated, utilization);   
        }
    }

    //getch();//TODO DEBUG

    refresh();
}

/**
 * @brief Gets a pointer to a static array of NetworkPrefix structures.
 * 
 * This function returns a pointer to a static array of NetworkPrefix structures.
 * The array is created only once, and subsequent calls to this function will provide
 * a pointer to the same array. If the array has not been created yet, it allocates
 * memory for the array based on the specified maximum number of prefixes.
 * 
 * @param maxPrefixes The maximum number of prefixes for which memory should be allocated.
 * @return NetworkPrefix* Pointer to the static array of NetworkPrefix structures.
 */
NetworkPrefix* getPrefixes(int maxPrefixes){
    // Static pointer to the array of NetworkPrefix structures
    static NetworkPrefix* prefixArray = NULL;

    // Check if the array has not been created yet
    if (prefixArray == NULL) {
        // Allocate memory for the array based on the specified maximum number of prefixes
        prefixArray = (NetworkPrefix*)malloc(maxPrefixes * sizeof(NetworkPrefix));

        // Return the pointer to the array
        return prefixArray;
    }

    // Return the pointer to the existing array
    return prefixArray;
}

/**
 * @brief Gets a pointer to a static unordered set of uint32_t with custom hash function IpHash.
 * 
 * This function returns a pointer to a static unordered set of uint32_t with a custom
 * hash function IpHash. The set is created only once, and subsequent calls to this function
 * will provide a pointer to the same set. If the set has not been created yet, it is created
 * and returned.
 * 
 * @return std::unordered_set<uint32_t, IpHash>* Pointer to the static unordered set.
 */
std::unordered_set<uint32_t, IpHash>* getIpTable() {
    // Static unordered set of uint32_t with a custom hash function IpHash
    static std::unordered_set<uint32_t, IpHash> ipTable;

    // Return the pointer to the set
    return &ipTable;
}

/**
 * @brief Check DHCP options from a DHCP packet and determines the DHCP message type.
 *
 * @param packet The DHCP packet containing options.
 * @return int The DHCP message type (DHCPACK, DHCPRELEASE) if valid, 0 otherwise.
 */
int printOptions(struct dhcp_packet *packet) {

    // Flags to track presence of specific options
    bool LeaseTime = false;
    bool MsgTypeBool = false;
    bool ServerId = false;
    int MsgType = 0;

    // Iterate through DHCP options in the packet
    for (size_t i = 0; i < sizeof(packet->options); i += 2 + packet->options[i + 1]) {

        // Switch based on DHCP option code
        switch (packet->options[i])
        {
        case DHCPMsgType:
            // Check if the DHCP message type is either DHCPACK or DHCPRELEASE
            if ((packet->options[i + 2] == DHCPACK) || (packet->options[i + 2] == DHCPRELEASE))
            {
                MsgTypeBool = true;
                // Set MsgType based on the DHCP message type
                MsgType = (packet->options[i + 2] == DHCPACK) ? DHCPACK : DHCPRELEASE;
            }
            else{
                
                return 0; // Invalid DHCP message type
            }
            break;
        case AddressTime:
            // Check if LeaseTime is present and MsgType is not DHCPRELEASE
            if (MsgType == DHCPRELEASE)
            {
                return 0; // LeaseTime not expected in DHCPRELEASE
            }
            else
            {
                LeaseTime = true;
                break;
            }
            
        case AddressRequest:
        case ParameterList:
        case DHCPMaxMsgSize:
            return 0; // Unsupported DHCP options
        case DHCPServerId:
            // Check if ServerId is present and MsgType is DHCPRELEASE
            if (MsgType == DHCPRELEASE)
            {
                ServerId = true;
            }
            break;
        default:
            break;
        }
    }

    // Check conditions to determine DHCP message type
    if ((LeaseTime) && (MsgTypeBool) && (MsgType == DHCPACK))
    {
        return DHCPACK;
    }
    else if ((MsgType == DHCPRELEASE) && (ServerId = true))
    {
        return DHCPRELEASE;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Checks if an IP address is within the specified network prefix.
 * 
 * This function performs a bitwise AND operation on the given IP address and the prefix mask.
 * It then compares the result with the bitwise AND operation on the prefix's IP address and mask.
 * If the results are equal, the IP address is considered to be within the specified network prefix.
 * Additionally, this function excludes the network and broadcast addresses from the check.
 * 
 * @param ipAddress The IP address to check.
 * @param prefix The network prefix to compare against.
 * @return true if the IP address is within the specified network prefix and not the network or broadcast address, false otherwise.
 */
bool isIPInPrefix(uint32_t ipAddress, const NetworkPrefix& prefix) {

    // Perform bitwise AND on the given IP address and prefix mask
    uint32_t result1 = ipAddress & prefix.mask;
    // Perform bitwise AND on the prefix's IP address and mask
    uint32_t result2 = prefix.ip & prefix.mask;

    // Check if the IP address is within the network prefix
    bool isInPrefix = (result2 == result1);

    // Check if the IP address is the network address or broadcast address
    bool isNetworkAddress = (ipAddress == result1);
    bool isBroadcastAddress = (ipAddress == (result2 | ~prefix.mask));

    // Return true if the IP address is within the prefix but not the network or broadcast address
    return (isInPrefix && !isNetworkAddress && !isBroadcastAddress);
}

/**
 * @brief Modifies data based on DHCP information and checks if IP addresses fall within specified network prefixes.
 * 
 * This function takes an IP address and DHCP message type as input. It then checks if the IP address falls
 * within any of the specified network prefixes and performs actions accordingly:
 *   - If the IP address is in the set (IP table), and the message type is DHCPRELEASE, it removes the IP from the set
 *     and decrements the allocated count for the corresponding network prefix.
 *   - If the IP address is not in the set, and the message type is DHCPACK, it adds the IP to the set and increments
 *     the allocated count for the corresponding network prefix.
 * The function updates the prefix information and the IP table accordingly.
 * 
 * @param ipAddress The IP address to be checked and modified.
 * @param msgType The DHCP message type (DHCPRELEASE or DHCPACK).
 */
void modifyDataForMatchingPrefix(uint32_t ipAddress, int msgType) {
    bool erase = false;

    // Get the array of network prefixes and the maximum number of prefixes
    NetworkPrefix* prefixes = getPrefixes(0);
    size_t numPrefixes = prefixes->MAX_PREFIXES;

    // Get the IP table
    std::unordered_set<uint32_t, IpHash> *ipTable = getIpTable();

    // Find the IP in the table
    auto ip = ipTable->find(ipAddress);

    // Iterate over network prefixes
    for (size_t i = 0; i < numPrefixes; ++i) {
        if (isIPInPrefix(ipAddress, prefixes[i])) {

            // Check if an IP address is in the set
            if (ip != ipTable->end()) {
                if (msgType == DHCPRELEASE)
                {
                    // If the IP is in the table and it's a RELEASE packet, remove the IP from the table
                    prefixes[i].allocated--;
                    printPrefixes();
                    erase = true;
                }
                else{
                    continue;
                }
                
            } else {
                if (msgType == DHCPACK)
                {
                    // If the IP is not in the table and it's an ACK packet, add the IP to the table
                    ipTable->insert(ipAddress);
                    prefixes[i].allocated++;
                    printPrefixes();
                }
                else{
                    continue;
                }  
            }
        }
    }
    // If erase flag is set, remove the IP from the table
    if (erase)
    {
        ipTable->erase(ip);
    }
}


/**
 * @brief Packet handler function to process captured DHCP packets.
 * 
 * This function is called for each captured packet. It extracts the Ethernet, IP, and UDP headers
 * to identify DHCP packets on ports 67 and 68. If the packet is a DHCPACK or DHCPRELEASE, it
 * calls the corresponding function to modify data based on network prefixes.
 * 
 * @param user User-defined pointer (not used in this implementation).
 * @param pkthdr Pointer to the packet header structure containing timestamp and packet length.
 * @param packet Pointer to the captured packet data.
 */
void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

    //count++; //TODO


    // Extract Ethernet header
    struct ether_header *ethHeader = (struct ether_header *)packet;
    size_t ethHeaderLength = sizeof(struct ether_header);

    // Extract IP header
    struct ip *ipHeader = (struct ip *)(packet + ethHeaderLength);
    size_t ipHeaderLength = (ipHeader->ip_hl) << 2;  // Length in 32-bit words, so left shift by 2

    // Extract UDP header
    struct udphdr *udpHeader = (struct udphdr *)(packet + ethHeaderLength + ipHeaderLength);
    size_t udpHeaderLength = sizeof(struct udphdr);


    // Check if the packet is on DHCP ports (67 and 68)
    if ((ntohs(udpHeader->source) == 67 && ntohs(udpHeader->dest) == 68) ||
        (ntohs(udpHeader->source) == 68 && ntohs(udpHeader->dest) == 67))
    {
        
        // Calculate the length of the DHCP packet
        size_t dhcpPacketLength = pkthdr->len - ethHeaderLength - ipHeaderLength - udpHeaderLength;

        // Get the DHCP packet structure
        struct dhcp_packet *dhcpPacket = (struct dhcp_packet *)(packet + ethHeaderLength + ipHeaderLength + udpHeaderLength);

        // Check the DHCP message type and modify data accordingly
        if (printOptions(dhcpPacket) == DHCPACK)
        {
            modifyDataForMatchingPrefix(dhcpPacket->yiaddr, DHCPACK);
        }else if (printOptions(dhcpPacket) == DHCPRELEASE)
        {
            modifyDataForMatchingPrefix(dhcpPacket->ciaddr, DHCPRELEASE);
        }
    }
    else{
        // Not a DHCP packet on ports 67 or 68, ignore
        return;
    }

}

/**
 * @brief Get the pcap handle object.
 * 
 * This function returns a pointer to the pcap handle. If a handle is provided as an argument, it 
 * updates the internal static handle with the provided value.
 * 
 * @param handle A pointer to the pcap handle to be set (optional).
 * @return pcap* A pointer to the pcap handle.
 */
pcap *getHandle(pcap* handle){

    // Internal static handle variable
    static pcap* handler;

    // Set the internal handle if a handle is provided
    if (handle != NULL)
    {
        handler = handle;
    }

    // Return the pcap handle
    return handler;

}

/**
 * @brief Initializes the ncurses library, enabling keypad input and disabling echo.
 * 
 * This function should be called to set up the ncurses environment.
 * It initializes the screen, enables keypad input for special keys, and disables echoing of characters.
 * Additionally, it calls the printPrefixes function to display initial information on the screen.
 */
void ncursesInit(){
    // Initialize the ncurses library
    initscr();

    // Enable keypad input for special keys
    keypad(stdscr, TRUE);

    // Disable echoing of characters
    noecho();

    // Print initial information on the screen
    printPrefixes();
}

/**
 * @brief Opens a PCAP file and processes packets.
 *
 * This function opens a PCAP file for offline packet capture, sets up a packet filter, 
 * and then processes the packets in a loop. If there are any errors during the process, 
 * it returns 1; otherwise, it returns 0.
 * 
 * @param filename Path to the PCAP file.
 * @return int Returns 0 on success, 1 on error.
 */
int openFile(const char *filename) {
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open the PCAP file for offline packet capture
    pcap_t *handler = pcap_open_offline(filename, errbuf);
    if (handler == NULL) {
        std::cerr << "Error opening PCAP file: " << errbuf << std::endl;
        myExitFunction(1);
    }

    pcap_t *handle = getHandle(handler);

    // Set up the packet filter
    if (setPacketFilter(handle) != 0) {
        return 1;
    }
    
    //Initialization ncurses
    ncursesInit();
    
    // Start capturing packets in a loop
    pcap_loop(handle, 0, packet_handler, NULL);

    return 0;

}

/**
 * @brief Opens a network interface for live packet capture.
 *
 * This function opens a network interface for live packet capture using pcap_open_live, sets up a packet filter, 
 * and then processes the captured packets in a loop. If there are any errors during the process, 
 * it returns 1; otherwise, it returns 0.
 * 
 * @param interface Name of the network interface.
 * @return int Returns 0 on success, 1 on error.
 */
int openLiveCapture(const char *interface) {

    char errbuf[PCAP_ERRBUF_SIZE];

    // Open the network interface for live packet capture
    pcap_t *handler = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    if (handler == NULL) {
        std::cerr << "Error opening network interface: " << errbuf << std::endl;
        myExitFunction(1);
    }

    pcap_t *handle = getHandle(handler);

    // Set up the packet filter
    if (setPacketFilter(handle) != 0) {
        return 1;
    }

    //Initialization ncurses
    ncursesInit();

    // Start capturing packets in a loop
    pcap_loop(handle, 0, packet_handler, NULL);

    return 0;
}

/**
 * @brief Sets a packet filter to capture only DHCP packets.
 *
 * This function sets a packet filter on the given pcap handle to capture only DHCP packets using
 * the filter expression "udp port 67 or udp port 68." If there are any errors during 
 * the compilation or setting of the filter, it returns 1; otherwise, it returns 0.
 * 
 * @param handle The pcap handle for the capture session.
 * @return int Returns 0 on success, 1 on error.
 */
int setPacketFilter(pcap_t* handle) {
    // Set a packet filter to capture only DHCP packets
    struct bpf_program fp;
    char filter_exp[] = "udp port 67 or udp port 68"; // Filter for DHCP packets
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Error compiling filter" << std::endl;
        return 1;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Error setting filter" << std::endl;
        return 1;
    }
    return 0;
}

/**
 * @brief Initialize an array of network prefixes.
 * 
 * This function takes the maximum number of prefixes and an array of prefix strings as input,
 * initializes the internal structure for each prefix, and stores the result in an array.
 * 
 * @param MAX_PREFIXES The maximum number of network prefixes.
 * @param prefixes An array of strings representing network prefixes in CIDR format.
 */
void initializePrefixes(int MAX_PREFIXES, char** prefixes) {

    // Get the array of network prefixes
    struct NetworkPrefix* prefixList;
    prefixList = getPrefixes(MAX_PREFIXES);

    //Initialize structure for each prefix
    for (int i = 0; i < MAX_PREFIXES; i++) {

        // Separate IP address and prefix length
        if (sscanf(prefixes[i], "%15[^/]/%d", prefixList[i].prefix, &prefixList[i].prefix_length) != 2) {
        std::cerr << "Invalid CIDR format: " << prefixes[i] << "\n";
        myExitFunction(1);
        }

        // Convert IP address string to binary
        uint32_t ip = inet_addr(prefixList[i].prefix);

        // Set the IP, allocated, and maxAddress fields
        prefixList[i].ip = inet_addr(prefixList[i].prefix);
        prefixList[i].allocated = 0;

        prefixList[i].maxAddress = (prefixList[i].prefix_length == 32) ? prefixList[i].maxAddress = 0 : (static_cast<uint32_t>(std::pow(2, 32 - prefixList[i].prefix_length)) - 2);
        // Set the subnet mask based on the prefix length
        prefixList[i].mask = htonl(0xFFFFFFFFu << 2 << (30 - prefixList[i].prefix_length)); 

        prefixList[i].mask = (prefixList[i].prefix_length == 0) ? 0xFFFFFFFFu : htonl(0xFFFFFFFFu << (32 - prefixList[i].prefix_length));

        // Set the maximum number of prefixes
        prefixList[i].MAX_PREFIXES = MAX_PREFIXES;
    }
}

int main(int argc, char *argv[])
{

    // Register the signal handler for SIGINT
    if (signal(SIGINT, myExitFunction) == SIG_ERR) {
        std::cerr << "Error registering signal handler for SIGINT." << std::endl;
        return 1;
    }

    //Setting up program parameters
    struct CommandLineArgs args;
    int argCheckResult = argumentCheck(argc, argv, &args);
    if (argCheckResult != 0)
    {
        myExitFunction(1);
        return argCheckResult;
    }

    initializePrefixes(args.ipPrefixCount, args.ipPrefixes);

    if (args.filename != NULL)
    {
        int file = openFile(args.filename);
        if (file != 0)
        {
            myExitFunction(2);
        }
        
    }
    if (args.interface != NULL)
    {
        int file = openLiveCapture(args.interface);
        if (file != 0)
        {
            myExitFunction(2);
        }
    }

    getch();

    myExitFunction(0);
    return 0;
}