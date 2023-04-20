/**
* IPK Project 1   
* Autor: Adrian Horvath  
* 2022/2023
* 
*/

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>



#define MAX_PAYLOAD_LENGTH 1024
#define REQUEST_OPCODE 0
#define STATUS_OK 0
#define STATUS_ERR 1

int sockfd;

/**
 * @brief A struct for the IPKCP binary protocol request message
 * 
 */
typedef struct {
  uint8_t opcode = 0;       /**< Opcode for the request message. */
  int message_length;       /**< Length of the whole message. */
  std::vector<uint8_t> requestMessage; /**< Reguest message to be send to server. */
} IPKCPRequest;

/**
 * @brief A struct for the IPKCP binary protocol response message
 * 
 */
typedef struct {
    uint8_t opcode = 1;         /**< opcode for the response message. */
    uint8_t status_code;        /**< status code for the response message. */
    uint8_t payload_length;     /**< length of the payload data in bytes. */
    std::string payload_data; /**< payload data (maximum size of MAX_PAYLOAD_LENGT). */
} IPKCPResponse;

/**
 * @brief Function to create the IPKCP binary protocol request message
 * 
 * @param input
 * @return std::string 
 */
IPKCPRequest createRequest(const std::string& input) {

    IPKCPRequest request;


    //Encode input to ASCII string
    std::vector<uint8_t> arr(input.size());
    for (size_t i = 0; i < input.size(); i++) {
        arr[i] = static_cast<uint8_t>(input[i]);
    }


    //Set up IPKCPRequest structure parameters
    request.message_length = input.size();
    request.opcode = REQUEST_OPCODE;
    request.requestMessage = arr;


    return request;
}

/**
* @brief 
* @param server_addr Server address
* @param sockfd Open socket
* @param message Message to be send to server
* @param message_size Lenght of message
*/
void send_udp_message(struct sockaddr_in server_addr, int sockfd, uint8_t* message, int message_size) {

    // Send message to server
    int n_sent = sendto(sockfd, message, message_size, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (n_sent < 0) {
        std::cerr << "Send request failure" << std::endl;
        close(sockfd);
        std::exit(-1);
    }
}
/**
* @brief Function for recieve UDP message from server using IPKCP protocol
* @param sockfd Open socket
* @param server_addr Server address
* @return Response message from server stored in IPKCPResponse structure
*/
IPKCPResponse receive_response(int sockfd, struct sockaddr_in server_addr) {

    IPKCPResponse response;
    char recieveMessage[MAX_PAYLOAD_LENGTH];

    socklen_t addr_len = sizeof(server_addr);

    // Recieve message from server
    int num_bytes = recvfrom(sockfd, &recieveMessage, MAX_PAYLOAD_LENGTH, 0, reinterpret_cast<struct sockaddr *>(&server_addr), &addr_len);

  if (num_bytes == -1) {
        std::cerr << "Recieve response failure" << std::endl;
        close(sockfd);
        std::exit(-1);
  }

    // Store data from message into IPKCPResponse structure
    response.opcode = recieveMessage[0];
    response.status_code = recieveMessage[1];
    response.payload_length = recieveMessage[2];
    std::string msg(&recieveMessage[3], response.payload_length);
    response.payload_data = msg;

  return response;
}

/**
* @brief Function for send TCP message to server
* @param sockfd Open socket
* @param server_addr Server address
* @param message Message to be send to server
*/
void send_tcp_request(int sockfd, struct sockaddr_in server_addr, const std::string& message) {

    std::string msg =  "HELLO\nSOLVE (+ 1 2)\nBYE\n";

    //const char* message_ptr = message.c_str();
    const char* message_ptr = msg.c_str();

    // Send message to server
    if (send(sockfd, message_ptr, strlen(message_ptr), 0) < 0) {
        std::cerr << "Send request failure" << std::endl;
        close(sockfd);
        std::exit(-1);
    }
    
}
/**
* @brief Function for recieve TCP message from server
* @param sockfd Open sock
* @return Message from server
*/
std::string receive_textual_response(int sockfd) {

    ssize_t totalrecieved = 0;

    std::string response;
    char buffer[MAX_PAYLOAD_LENGTH];
    std::memset(buffer, 0, MAX_PAYLOAD_LENGTH);

    bool endline = false;
    

    
    // Asking for packets till get LF
    while (!endline)
    {
        ssize_t numBytesReceived = recv(sockfd, &buffer[totalrecieved], MAX_PAYLOAD_LENGTH, 0);

        //printf("%d>>%s", totalrecieved, buffer);

        if (numBytesReceived == -1) {
            std::cerr << "Recieve response failure" << std::endl;
            close(sockfd);
            std::exit(-1);
        }   
        // Iterating cross buffer looking for /n
        for (int i = 0; i <= numBytesReceived; i++)
        {
            if (buffer[totalrecieved + i] == '\n')
            {
                totalrecieved += i;
                endline = true;
                break;
            }
            else
            {
                continue;
            }
        }
        if (!endline)
        {
            totalrecieved += numBytesReceived;
        }
        
        

    };
        

    

    //printf("%d>%s", totalrecieved, buffer);

    // Store message in return string
    response = std::string(buffer, totalrecieved + 1);
    return response;
}

void intHandler(int dummy) {
    
    close(sockfd);
}

int main(int argc, char* argv[]){



    int opt;
    std::string host, port, mode;

    //Loop for parsing input characters
    /* while ((opt = getopt(argc, argv, "h:p:m:")) != -1) {
        switch (opt) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -h <host> -p <port> -m <mode>" << std::endl;
                return -1;
        }
    }

    if (host.empty() || port.empty() || mode.empty()) {
        std::cerr << "Usage: " << argv[0] << " -h <host> -p <port> -m <mode>" << std::endl;
        return -1;
    }
     */
    host = "0.0.0.0";
    port = "2023";
    mode = "tcp";

    std::cout << host << std::endl;
    std::cout << port << std::endl;
    std::cout << mode << std::endl;



    
    
    // Choose between udp or tcp 
    if (mode == "udp")
    {
        // Create socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0)
        {
            std::cerr << "Socket error" << std::endl;
            return -1;
        }

        //Connect to server using host ip and port
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(stoi(port)); 
        if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0)
        {
            std::cerr << "IP error" << std::endl;
            return -1;
        }
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failure " << std::endl;
            return -1;
        
        }

        // Input line from standart input
        std::string line;

        // Cycle read user input
        while (std::getline(std::cin, line)) {

            IPKCPRequest reqMessage = createRequest(line);

            // Set up message according IPKCP protocol
            reqMessage.requestMessage.insert(reqMessage.requestMessage.begin(), reqMessage.message_length);
            reqMessage.requestMessage.insert(reqMessage.requestMessage.begin(), reqMessage.opcode);


            send_udp_message(server_addr, sockfd, reqMessage.requestMessage.data(), reqMessage.message_length + 2);


            // Handle response from server
            IPKCPResponse response = receive_response(sockfd, server_addr);
            if (response.status_code == STATUS_OK)
            {
                std::cout << "OK:" << response.payload_data << std::endl;
            }
            else if (response.status_code == STATUS_ERR)
            {
                std::cout << "ERR:" << response.payload_data << std::endl;
            }
            
        }

    }
    else if (mode == "tcp")
    {
        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            std::cerr << "Socket error" << std::endl;
            return -1;
        }

        //Connect to server using host ip and port
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(stoi(port));
        if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0)
        {
            std::cerr << "IP error" << std::endl;
            return -1;
        } 
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failure " << std::endl;
            return -1; 
        }

        // Input line from standart input
        std::string line;

        // Cycle read user input
       while (std::getline(std::cin, line)){
            line.push_back('\n');
            send_tcp_request(sockfd, server_addr, line);
            std::string out = receive_textual_response(sockfd);
            std::cout << out;

            // If server send BYE close connection
            if (out == "BYE\n")
            {
                close(sockfd);
                break;
            }
        }
    }
    else
    {
        std::cerr << "Mode not supported" << std::endl;
        return -1;
    }

    close(sockfd);
    return 1;
}
