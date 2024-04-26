//new file post mistake
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "cJSON.h"
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>


void cleanExit(){exit(0);} 

cJSON* recv_json(int accID) {
	char buffer[1020];
	int check = read(accID, buffer, sizeof(buffer));
	buffer[check] = '\0';
	printf("%s\n", buffer);
	
	cJSON *config_json = cJSON_Parse(buffer);
	return config_json;
	
}

int make_tcp_socket(int port) {
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
	
	int accID = accept(fdsocket, (struct sockaddr*) &clientAddr, &addr_len);

	if (accID < 0) {
		perror("Error accepting connection");
		return 1;
	}
	
	close(fdsocket);
	signal(SIGTERM, cleanExit);
	signal(SIGINT, cleanExit);
	
	return accID;
	
}

int make_udp_socket(int port, cJSON *config_json) {
	int udp_sfd = socket(PF_INET, SOCK_DGRAM, PF_UNSPEC);
	
	if (udp_sfd == -1) {
		printf("%s\n", "Socket creation failed");
		return 0;
	}
	
	int optval = 1;
	
	int setSockCheck = setsockopt(udp_sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	
	struct timeval timeout;
    		timeout.tv_sec = 3;
    		timeout.tv_usec = 0;
    		if (setsockopt(udp_sfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1) {
        	perror("setsockopt failed");
        	exit(EXIT_FAILURE);
    	}
	
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	const char *ipaddrname = "server_ip_address"; 
	cJSON *server_ipaddr = cJSON_GetObjectItem(config_json, ipaddrname);
	
	serverAddr.sin_addr.s_addr = inet_addr(server_ipaddr->valuestring); 
   	serverAddr.sin_port = htons(port); 
    	serverAddr.sin_family = AF_INET;
    	
    	int bindCheck = bind(udp_sfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    	
    	if (bindCheck < 0) {
		perror("Cannot bind server to socket");
		return 1;
	}
    	
    	struct sockaddr_in clientAddr;
    	bzero(&clientAddr, sizeof(clientAddr));
    	char buffer[1020];
    	
    	int clientAddrLen = sizeof(clientAddr);
    	
    	int mesLength;
    	
    	int mesCount = 0;
    	
    	int lostPackets = 0;
    	while (1) {
    	
    		
    	
    		if (mesCount == 6000) {
    			printf("%s\n", "end of train 1");
    		}
    		
    		if (mesCount == 1200) {
    			printf("%s\n", "end of second train");
    			break;
    		}
    		
    		
    		mesLength = recvfrom(udp_sfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &clientAddr, &clientAddrLen);
    		
    		if (mesLength <= 0) {
    			printf("%s\n", "lost_packet");
    			lostPackets++;
    		} else {
    			printf("%d\n", mesLength);
		}
    		
    		mesCount++;
    		printf("%d\n", mesCount);
    		
    	}
    	
    	printf("%d\n", lostPackets);
    	return udp_sfd;
    	
    	
}

int main(int argc, char* argv[]) {
	int port = atoi(argv[1]);
	
	int accID = make_tcp_socket(port);
	accID;
	cJSON *config_json = recv_json(accID);
	if (config_json == NULL) {
		perror("Error parsing JSON");
		return 1;
	}
	
	const char *udp_destination_port_name = "udp_destination_port";
	cJSON *udp_destination_port_val =  cJSON_GetObjectItem(config_json, udp_destination_port_name);
	
	
	
	int udp_destination_port = udp_destination_port_val->valueint;
	//printf("%d\n", udp_destination_port);
	
	int udp_sfd = make_udp_socket(udp_destination_port, config_json);
	close(udp_sfd);
	

	
	
}