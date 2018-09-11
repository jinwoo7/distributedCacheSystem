// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <string.h>

int get_listening_socket(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)))  { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
       
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

    return new_socket;
}

int get_sending_socket (int port) {
    int sock = 0; 
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    return sock;
}
 
int main(int argc, char const *argv[]) {
    int listen_socket, send_socket, valread, s_port, r_port;

    // Argument number checking
    if (argc != 3) {
        fprintf(stderr, 
        "Please provide following inputs: <server port> <remote port>\n");
        exit(EXIT_FAILURE);
    }

    // Argument validation
    s_port = atoi(argv[1]);
    r_port = atoi(argv[2]);
    if (s_port == 0 || r_port == 0) {
        fprintf(stderr, 
        "Please provide proper port numbers\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        char buffer[1024] = {0}, cmd[1024] = {0};
        int cmd_num;
        fprintf(stdout, "Which command should I run?: ");
        fgets(cmd, sizeof(cmd), stdin);
        cmd_num = atoi(cmd);

        switch (cmd_num) {
            case 1:          
                send_socket = get_sending_socket (r_port);
                fprintf(stdout, "Type your message: \n");
                fgets(buffer, sizeof(buffer), stdin);
                send(send_socket, buffer, strlen(buffer), 0);
                break;
            case 2:
                listen_socket = get_listening_socket(s_port);
                valread = read(listen_socket , buffer, 1024);
                fprintf(stdout, "Message received: \n%s", buffer);
                break;
            default:
                fprintf(stdout, "Unrecognized command\n");
                break;
        }
    }

    return 0; 
} 