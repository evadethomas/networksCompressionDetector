//new file post mistake
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    int port = atoi(argv[1]);
    
    int fdsocket = socket(PF_INET, SOCK_STREAM, PF_UNSPEC);
    if (fdsocket == -1) {
        printf("%s\n", "Socket creation failed");
        return 0;
    }
    
    int optval = 1;
    
    int setSockCheck = setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        
        if (setSockCheck < 0) {
            perror("Cannot reuse address");
            return 1;
        }
    
    struct sockaddr_in serverAddr;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    int bindcheck = bind(fdsocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    
    if (bindcheck < 0) {
        perror("Cannot bind server to socket");
        return 1;
    } 
    
    int listenCheck = listen(fdsocket, 5);
    
    if (listenCheck < 0) {
        perror("Error listening");
        return 1;
    }
    
    struct sockaddr_in clientAddr;
    int client_socket, addr_len = sizeof(clientAddr);
    
    
    int accCheck = accept(fdsocket, (struct sockaddr*) &clientAddr, &addr_len);
    if (accCheck < 0) {
        perror("Error accepting connection");
        return 1;
    }
    
    
    
    
    
    
    //printf("%d\n", port);
}