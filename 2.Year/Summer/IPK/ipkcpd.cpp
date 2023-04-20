/**
* IPK Project 2   
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
#include <stack>
#include <csignal>
#include <atomic>
#include <fcntl.h>

#define MAX_PAYLOAD_LENGTH 1024
#define RESPONSE_OPCODE 1
#define REQUEST_OPCODE 0
#define STATUS_OK 0
#define STATUS_ERR 1
#define HELLO_MESSAGE "HELLO"
#define SOLVE_MESSAGE "SOLVE "
#define BYE_MESSAGE "BYE"

// Global flag to indicate termination signal received
bool terminationSignalReceived = false;

void exit_app_UDP(int sockfd);
void exit_app_TCP(int sockfd);

enum State{
    INIT,
    ESTABLISHED
};

/**
 * @brief A struct with server parameters
 * 
 */
typedef struct {
    std::string mode;       /**< Connection mode TCP/UDP. */
    std::string port;       /**< Port server will listen on. */
    std::string host;       /**< IP address of server. */
} SERVERParameters;

/**
 * @brief A struct for the IPKCP binary protocol request message
 * 
 */
typedef struct {
    bool valid = true;
    uint8_t opcode;       /**< Opcode for the recieve message. */
    uint8_t message_length;       /**< Length of the whole message. */
    std::string recieveMessage; /**< Recieved message from client. */
    struct sockaddr_in client_addr;
} IPKCPRecieve;

/**
 * @brief A struct for the IPKCP binary protocol response message
 * 
 */
typedef struct {
    uint8_t opcode = 1;         /**< opcode for the response message. */
    uint8_t status_code;        /**< status code for the response message. */
    uint8_t payload_length;     /**< length of the payload data in bytes. */
    std::vector<uint8_t> payload_data; /**< payload data (maximum size of MAX_PAYLOAD_LENGT). */
} IPKCPResponse;

/**
* @brief Function to handle SIGNINT
* @param signum signal
*/
void signalHandler(int signum) {
    std::cout << "Received SIGINT signal (Ctrl+C). Gracefully exiting..." << std::endl;

    // Set the termination signal flag
    terminationSignalReceived = true;
    return;
}

/**
* @brief Function for recieve UDP message from server using IPKCP protocol
* @param sockfd Open socket
* @return Message from client stored in IPKCPRecieve structure
*/
IPKCPRecieve receive_request(int sockfd) {

    IPKCPRecieve recieve;
    char recieveMessage[MAX_PAYLOAD_LENGTH];
    socklen_t addr_len = sizeof(recieve.client_addr);

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    struct timeval timeout;
    timeout.tv_sec = 10; // Set timeout to 5 seconds
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    int num_bytes = 0;
    while (true)
    {
        // Recieve message from server
        num_bytes = recvfrom(sockfd, &recieveMessage, MAX_PAYLOAD_LENGTH, 0, reinterpret_cast<struct sockaddr *>(&recieve.client_addr), &addr_len);
        
        if (terminationSignalReceived)
        {
            break;
        }
        if (num_bytes > 0)
        {
            break;
        } 
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            continue;
        }
           
    }
    
    if (num_bytes < 1) {
        recieve.opcode = -23;
        return recieve;
    }

    // Store data from message into IPKCPRecieve structure
    recieve.opcode = recieveMessage[0];
    recieve.message_length = recieveMessage[1];
    std::string msg(&recieveMessage[2], recieve.message_length);
    recieve.recieveMessage = msg;

    if(num_bytes != recieve.message_length + 2 || recieve.opcode != REQUEST_OPCODE)
    {
        recieve.valid = false;
    }
    
  return recieve;
}

/**
* @brief Function that perform arithmetic operation
* @param op         Operator
* @param operand1   Left operand of expression 
* @param operand2   Right operand of expression
* @return           Result of aritmethic operation
*/
int performOperation(char op, int operand1, int operand2, uint8_t& status) {
    switch(op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 == 0)
            {
                status = STATUS_ERR;
                return 0;
            }
            
            return operand1 / operand2;
        default:
            status = STATUS_ERR;
            return 0;
    }
}

/**
* @brief Function for check if given char is valid operator
* @param c  Given character
* @return   True if given character is valid operator
*/
bool is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

/**
* @brief Function makes from digits one number
* @param expression  Pointer of string with expression
* @param index  Index in string where is first digit of number
* @return   Whole digit saved as intiger
*/
int digit_to_number(const std::string& expression, int& index){
    
    int num = 0;
    char c = expression[--index];
    
    while (isdigit(c))
    {
        num = num * 10 +(c - '0');
        index++;
        c = expression[index];
    }

    return num; 
}

/**
* @brief Function to recursive evaluate a mathematical expression
* @param expression  Pointer of string with expression
* @param index  Index in string where expression starts
* @param status Status for response message
* @return   Result of expression
*/
int evaluate_expression(const std::string& expression, uint8_t& status, int& index) {

    int maxIndex = expression.length();
    if (index >= (int)expression.length()) {
        status = STATUS_ERR;
        return -1;
    }

    std::stack<int> operands;
    std::stack<char> operators;

    int count = 1;

    char c = expression[index++];

    if (c == '(' && index < maxIndex)
    {
        operators.push(c);
        c = expression[index++];

        if (is_operator(c) && index < maxIndex)
        {
            operators.push(c);
            c = expression[index++];
            if (c == ' ' && index < maxIndex)
            {
                c = expression[index++];
                
                do
                {
                    if (c == '(' && index < maxIndex)
                    {
                        index--;
                        int number = evaluate_expression(expression, status, index);
                         
                        if (status != STATUS_OK)
                        {
                            return -1;
                        }
                        
                        c = expression[index++];
                        if (operands.empty())
                        {
                            count = 1;
                            operands.push(number);
                        }
                        else
                        {
                            count += 1;
                            int result = performOperation(operators.top(), operands.top(), number, status);
                            if (status != STATUS_OK)
                            {
                                return -1;
                            }
                            operands.pop();
                            operands.push(result);
                        } 

                        if (c == ' ' && index < maxIndex)
                        {
                            c = expression[index++];
                        }
                        else if(c == ')')
                        {
                            continue;
                        }
                        else
                        {
                            status = STATUS_ERR;
                            return -1;
                        }  
                    }

                    else if (isdigit(c) && index < maxIndex) {
                        int number = digit_to_number(expression, index);
                        c = expression[index++];

                        if (operands.empty())
                        {
                            count = 1;
                            operands.push(number);
                        }
                        else
                        {
                            count += 1;
                            int result = performOperation(operators.top(), operands.top(), number, status);
                            if (status != STATUS_OK)
                            {
                                return -1;
                            }
                            operands.pop();
                            operands.push(result);
                        }

                        if (c == ' ' && index < maxIndex)
                        {
                            c = expression[index++];
                            if (c == ')')
                            {
                                status = STATUS_ERR;
                                return -1;
                            }
                            
                        }
                        else if(c == ')' && count > 1)
                        {
                            continue;
                        }
                        
                        else
                        {
                            status = STATUS_ERR;
                            return -1;
                        }    
                    }
                    else
                    {
                        status = STATUS_ERR;
                        return -1;
                    }   
                }while (c != ')');

                if (c == ')' && count <= 1)
                {
                    status = STATUS_ERR;
                    return -1;
                }
                
            }
            else
            {
                status = STATUS_ERR;
                return -1;
            } 
        } 
        else
        {
            status = STATUS_ERR;
            return -1;
        }    
    }
    else
    {
        status = STATUS_ERR;
        return -1;
    }

    return operands.top();
}

/**
 * @brief Function to create the IPKCP binary protocol response message
 * 
 * @param input
 * @return IPKCPResponse
 */
IPKCPResponse createResponse(const std::string& input, uint8_t status) {

    IPKCPResponse response;

    //Encode input to ASCII string
    std::vector<uint8_t> arr(input.size());
    for (size_t i = 0; i < input.size(); i++) {
        arr[i] = static_cast<uint8_t>(input[i]);
    }

    //Set up IPKCPRequest structure parameters
    response.payload_length = input.size();
    response.opcode = RESPONSE_OPCODE;
    response.status_code = status;
    response.payload_data = arr;

    // Set up message according IPKCP protocol
    response.payload_data.insert(response.payload_data.begin(), response.payload_length);
    response.payload_data.insert(response.payload_data.begin(), response.status_code);
    response.payload_data.insert(response.payload_data.begin(), response.opcode);

    return response;
}

/**
* @brief Function that send UDP message to client
* @param client_addr Client address
* @param sockfd Open socket
* @param message Message to be send to server
* @param message_size Lenght of message
*/
void send_udp_message(struct sockaddr_in client_addr, int sockfd, uint8_t* message, int message_size) {

    // Send message to client
    int n_sent = sendto(sockfd, message, message_size, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

    if (n_sent < 0) {
        std::cerr << "Send request failure" << std::endl;
        return;
    }
}

/**
* @brief Function set up UDP connection of server and connect clients
* @param settings Structs with server parameters
* @param sockfd Open socket
*/
void upd_connection(SERVERParameters& settings){

    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd <= 0)
    {
        std::cerr << "Socket error" << std::endl;
        exit(-1);
    }

    //Connect to server using host ip and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(settings.port)); 
    inet_pton(AF_INET, settings.host.c_str(),  &server_addr.sin_addr);
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) 
    {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        exit(-1);
    }

    std::string line;

    // Cycle read user input
    while (true) {
        
        uint8_t status = STATUS_OK;
        int index = 0;
        IPKCPRecieve recieveMessage = receive_request(sockfd);
        IPKCPResponse response;
        if (terminationSignalReceived)
        {
            exit_app_UDP(sockfd);
        }
        if (recieveMessage.valid)
        {
            int result = evaluate_expression(recieveMessage.recieveMessage, status, index);
            if (index != (int)recieveMessage.recieveMessage.length())
            {
                status = STATUS_ERR;
            }
            
            if (status == STATUS_OK)
            {
                response = createResponse(std::to_string(result), status);
                send_udp_message(recieveMessage.client_addr, sockfd, response.payload_data.data(), response.payload_length + 3);
            }
            else
            {
                response = createResponse("Expressioin incorect", status);
                send_udp_message(recieveMessage.client_addr, sockfd, response.payload_data.data(), response.payload_length + 3);
            }
        }
        else
        {
            status = STATUS_ERR;
            response = createResponse("IPKCP protocol error", status);
            send_udp_message(recieveMessage.client_addr, sockfd, response.payload_data.data(), response.payload_length + 3);
        }     
    }
}

/**
* @brief Function for send TCP message to client
* @param sockfd Open socket
* @param server_addr Server address
* @param message Message to be send to server
*/
void send_tcp_request(int sockfd, const std::string& message, uint8_t& status) {

    const char* message_ptr = message.c_str();
    // Send message to server
    if (send(sockfd, message_ptr, strlen(message_ptr), 0) < 0) {

        std::cerr << "Send request failure" << std::endl;
        status = STATUS_ERR;
        return;
    }
}

/**
* @brief Function for recieve TCP message from server
* @param sockfd Open sock
* @return Message from client
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
        ssize_t numBytesReceived ;

        while (true)
        {
            numBytesReceived = recv(sockfd, &buffer[totalrecieved], MAX_PAYLOAD_LENGTH, 0);
            if (terminationSignalReceived)
            {  
                return response;
            }
            if (numBytesReceived > 0)
            {
                break;
            }
            
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                continue;
            }
            else if (numBytesReceived < 1) {
                close(sockfd);
                response = "BYE\n";
                return response;
            } 
             
            else
            {
                break;
            }
        }

        // Recieve message from server
            
        if (terminationSignalReceived)
        {  
            response = "BYE\n";
            return response;
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

    // Store message in return string
    response = std::string(buffer, totalrecieved + 1);
    return response;
}

/**
* @brief Function for handle one TCP client
* @param arg Client's socket
*/
void* handle_client(void* arg){

    int newsockfd = *((int*)arg);
    State currentState = INIT;
    bool byeReceived = false;
    uint8_t status = STATUS_OK;

    int flags = fcntl(newsockfd, F_GETFL, 0);
	int rc = fcntl(newsockfd, F_SETFL, flags | O_NONBLOCK);
    if (rc < 0)
	{
		exit(-1);								
	}

    // Input line from standart input
    std::string line;

    while (!byeReceived && status == STATUS_OK) {

        // Receive message from client
        line = receive_textual_response(newsockfd);
        if (terminationSignalReceived)
        {
            exit_app_TCP(newsockfd);
            return nullptr;
        }

        // Process message based on current state
        std::string response;
        if (currentState == INIT) {
            if (line.substr(0, strlen(HELLO_MESSAGE)) == HELLO_MESSAGE) {
                response = "HELLO\n";
                currentState = ESTABLISHED;
            } else {
                response = "BYE\n";
                byeReceived = true;
            }
        } else if (currentState == ESTABLISHED) {
            if (line.substr(0, strlen(SOLVE_MESSAGE)) == SOLVE_MESSAGE) {

                int index = strlen(SOLVE_MESSAGE);
                int result = evaluate_expression(line, status, index);
                response =  "RESULT " + std::to_string(result) + "\n";

                //Error if message is not well formated, if expression is not correct or is expression is negative
                if (line[index] != '\n' || status != STATUS_OK || result < 0) 
                {
                    response = "BYE\n";
                    byeReceived = true;
                }

            } else if (line.substr(0, strlen(BYE_MESSAGE)) == BYE_MESSAGE) {
                response = "BYE\n";
                byeReceived = true;
            } else {
                response = "BYE\n";
                byeReceived = true;
            }
        }
        // Send response to client
        send_tcp_request(newsockfd, response, status);
    }

    // Close connection
    close(newsockfd);
    //std::cout << "Client disconnected" << std::endl;
    return nullptr;
}

/**
* @brief Function set up TCP connection of server and connect clients
* @param settings Structs with server parameters
* @param sockfd Open socket
*/
void tcp_connection(SERVERParameters& settings){
    int newsockfd;

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        exit(-1);
    }

    int on = 1;
    int rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if (rc < 0)
    {
        exit(-1);
    }

    //Connect to server using host ip and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(settings.port));
    inet_pton(AF_INET, settings.host.c_str(),  &server_addr.sin_addr);
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) 
    {
        exit(-1);
    }

    if (listen(sockfd, 10) < 0) {
        close(sockfd);
        exit(-1);
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
	rc = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    if (rc < 0)
	{
		exit(-1);								
	}
    
    struct sockaddr_in client_addr;

    while (!terminationSignalReceived) {
        // Accept connection
        socklen_t client_len = sizeof(client_addr);
        
        while (true)
        {
            // Recieve message from server
            newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (terminationSignalReceived)
            {  
                break;
            }  
            if (newsockfd > 0)
            {  
                break;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            {  
                continue;
            }  
        }

        if (newsockfd < 0) {
            continue;;
        }
        int nwsck = newsockfd;
        pthread_t thread;

        if (pthread_create(&thread, nullptr, handle_client, &nwsck) != 0) {
            close(newsockfd);
            continue;
        }

        // Detach the thread to allow it to run independently
        pthread_detach(thread);
        
    }
    close(sockfd);
    std::cout << "Server shutdown" << std::endl;
    exit(1);
}

/**
* @brief Parse command line arguments
* @param argc
* @param argv
* @param settings Structs with server parameters
*/
void parse_arguments(int argc, char* argv[], SERVERParameters& settings){

    int opt;

    //Loop for parsing input characters
    while ((opt = getopt(argc, argv, "h:p:m:")) != -1) {
        switch (opt) {
            case 'h':
                settings.host = optarg;
                break;
            case 'p':
                settings.port = optarg;
                break;
            case 'm':
                settings.mode = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -h <host> -p <port> -m <mode>" << std::endl;
                exit(-1);
        }
    }

    if (settings.host.empty() || settings.port.empty() || settings.mode.empty()) {
        std::cerr << "Usage: " << argv[0] << " -h <host> -p <port> -m <mode>" << std::endl;
        exit(-1);
    }
}

/**
* @brief Function terminate UDP connection 
* @param sockfd Open socket
* @param client_addr IP address of client
*/
void exit_app_UDP(int sockfd) {

    std::cout << "Server shutdown" << std::endl;
    close(sockfd);  
    exit(-1);
}

/**
* @brief Function terminate TCP connection 
* @param sockfd Open socket
*/
void exit_app_TCP(int sockfd) {
    uint8_t status = STATUS_ERR;
    send_tcp_request(sockfd, "BYE\n", status);
}

int main(int argc, char* argv[]){

    signal(SIGINT, signalHandler);
    SERVERParameters parameters;
    //parse_arguments(argc, argv, parameters);

    parameters.host = "0.0.0.0";
    parameters.port = "2023";
    parameters.mode = "tcp";

    std::cout << parameters.host << std::endl;
    std::cout << parameters.port << std::endl;
    std::cout << parameters.mode << std::endl;
    
    
    // Choose between udp or tcp 
    if (parameters.mode == "udp")
    {
        upd_connection(parameters);
    }
    else if (parameters.mode == "tcp")
    {
        tcp_connection(parameters);

    }
    else
    {
        std::cerr << "Mode not supported" << std::endl;
        return -1;
    }
    return 1;
}