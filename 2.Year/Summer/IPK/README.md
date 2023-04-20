# IPK Calculator Protocol Client

## Introduction

The goal of this project was to implement a server for a remote calculator.The application operates using UDP (binary variant) or TCP (textual variant) protocol. It recieve expressions to be calculated and sends back the results 

### Usage
```
./ipkcpd -h <host> -p <port> -m <mode>
```
Where host is the IP address of the server. Port is the port number on which the server will operate. And mode is the selection of the communication protocol, UDP or TCP.

### UDP

This implementation uses the UDP protocol for communication between the client and the server. The lack of a persistent connection means that the client sends messages in a specific format and the server responds accordingly. The client's message includes an eight-bit opcode (0/1, where 0 indicates a message sent by the client), eight bits for the length of the message, and the expression entered by the user in postfix notation, enclosed in parentheses. The expression can contain operators (+, -, *, /) and numbers. On the other hand, the received message from the server includes an eight-bit opcode (1), eight bits for the status code (0 for OK, 1 for error), eight bits for the length of the message, and the actual message itself. If everything is fine, the server displays "OK: Result" as the response. However, if an error occurs, it displays "ERR: Error message". UDP is known for its efficiency, simplicity, and speed compared to TCP, although it lacks reliability mechanisms such as guaranteed delivery of data and extensive error checking.

### TCP

In this variant, the TCP protocol is utilized for communication between the client and the server. This means that a connection must be established before communication can occur. The client initiates the communication by sending an initial message "HELLO", and upon receiving a greeting from the server, it indicates readiness for communication. The client then sends messages with requests for calculation in the format SOLVE("infix expression"). If everything is fine, the server responds with a message "RESULT: Result". Communication with the server is terminated with a "BYE" message, to which the server responds similarly and ends the communication. In case of an interrupt signal (C-c), the communication is properly terminated.

TCP is a widely used and reliable protocol for transmitting data over networks. It establishes a connection between devices, ensuring data integrity with extensive error-checking mechanisms. However, TCP may have slower transmission speeds compared to other protocols like UDP, which prioritize speed over reliability. Therefore, when selecting a protocol for data transmission, it's crucial to consider the specific needs of the application and weigh the trade-offs between speed and reliability.

### Content Structuring

Code is structured into several functions:
* The `udp_connection` prepare server for recieve udp connections, after Ctrl+c signal server is shutdown
* The `tcp_connection` prepare server for recieve tcp connections, after client is connectet function create new threat which will handling requests from one client, after Ctrl+c signal all connections are closed and server is shutdown
* The `createResponse` function creates a response message in the IPKCP binary protocol format. The function returns the IPKCPResponse structure.
* The `send_udp_message` function sends a UDP message to the client using the IPKCP binary protocol. If the `sendto` system call fails, the function prints an error message to standard error and program continue.
* The `receive_request` function receives a request message from the client using the IPKCP binary protocol. It stores the message data into the IPKCPRequest structure.
* The `send_tcp_request` function sends a textual message to the client using the TCP protocol.
* The `receive_textual_response` function receives a textual message from the client using the TCP protocol.
* The `evaluate_expression` function parse and process recieved expression, function using few helping functions like: `is_operator`, `digit_to_number`and `performOperation`
* At the end there are functions to close all connections after Ctrl + c signal

### Testing
Both UPD and TCP was tested with same collection of expressions, all tests was proceed manually, for testing was used IPKCP Calculator from Project 1
#### UDP
Collection for UDP... All test passed with expected result
```
(+ 2 2)
(* 3 4)
(/ 10 2)
(- 8 5)
(* 2 3 4)
(- 7 3 1)
(+ 5 (* 3 2))
(/ 16 2 2)
(+ 1 (/ 5 5))
(- 10 (/ 20 2))
(* 2 3 4 5)
(/ 100 2 2 5)
(+ 1 (* 2 3) (/ 4 2))
(- 12 4 2 1)
(* 1 2 3 4 5)
(/ 1000 5 2 2 5)
(+ 2 (* 3 4) (- 10 6))
(- 100 50 20 10)
(* 2 (+ 3 4) (- 5 1))
(/ 200 (+ 100 50) (* 2 2))
(* (- 4 2) (+ 3 4) (/ 10 2))
(+ (* 2 3) (/ 8 2) (- 10 6))
(* (- 5 2) (+ 1 2 3) (/ 18 3))
(/ (- 10 6) (+ 1 2 3) (* 3 4))
(- (* 2 3 4) (/ 16 2) (+ 5 4))
(+ (/ 10 2) (* (- 3 4) (+ 1 2)))
(* (+ 3 4 5) (- 8 6) (/ 20 4))
(/ (* 2 3 4) (- 10 5) (+ 2 2 2))
```
#### TCP
Collection for TCP... All test passed with expected result
```
HELLO
SOLVE (+ 2 2)
SOLVE (* 3 4)
SOLVE (/ 10 2)
SOLVE (- 8 5)
SOLVE (* 2 3 4)
SOLVE (- 7 3 1)
SOLVE (+ 5 (* 3 2))
SOLVE (/ 16 2 2)
SOLVE (+ 1 (/ 5 5))
SOLVE (- 10 (/ 20 2))
SOLVE (* 2 3 4 5)
SOLVE (/ 100 2 2 5)
SOLVE (+ 1 (* 2 3) (/ 4 2))
SOLVE (- 12 4 2 1)
SOLVE (* 1 2 3 4 5)
SOLVE (/ 1000 5 2 2 5)
SOLVE (+ 2 (* 3 4) (- 10 6))
SOLVE (- 100 50 20 10)
SOLVE (* 2 (+ 3 4) (- 5 1))
SOLVE (/ 200 (+ 100 50) (* 2 2))
SOLVE (* (- 4 2) (+ 3 4) (/ 10 2))
SOLVE (+ (* 2 3) (/ 8 2) (- 10 6))
SOLVE (* (- 5 2) (+ 1 2 3) (/ 18 3))
SOLVE (/ (- 10 6) (+ 1 2 3) (* 3 4))
SOLVE (- (* 2 3 4) (/ 16 2) (+ 5 4))
SOLVE (+ (/ 10 2) (* (- 3 4) (+ 1 2)))
SOLVE (* (+ 3 4 5) (- 8 6) (/ 20 4))
SOLVE (/ (* 2 3 4) (- 10 5) (+ 2 2 2))
BYE
```
### Bibliography
https://moodle.vut.cz/mod/folder/view.php?id=289124
https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp
https://www.rfc-editor.org/info/std68