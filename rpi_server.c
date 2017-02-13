#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<string.h>

#define FAILURE -1
#define BACKLOG 3
#define BUFFER_LEN 500

extern int errno;

int main(int argc, char* argv[]){
	
	if (argc != 2){
		printf("Usage:\n \t%s <port>\n", argv[0]);
		return FAILURE;
	}

	char port[sizeof argv[1]];
	strcpy(port, argv[1]);

	//investigate port details
	struct addrinfo details, *results;

	memset(&details, 0, sizeof details);
	details.ai_family = AF_UNSPEC;
	details.ai_socktype = SOCK_STREAM;
	details.ai_flags = AI_PASSIVE;

	int status = 0;
	if(status = getaddrinfo(NULL, port, &details, &results) != 0){
		printf("getaddrinfo did not work\n");
		printf("port: %s\n", port);
		printf("Reason: %s\n", gai_strerror(status));
	}
	
	//wow getaddrinfo does a lot of work.
	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (sd == -1){
		printf("Failure to make socket\n");
		printf("Reason: %s\n", strerror(errno));
		return FAILURE;	
	}

	printf("Socket %d\n", sd);
	if(bind(sd, results->ai_addr, results->ai_addrlen) == -1){
		printf("There was a failure binding to the port\n");	
		printf("Reason: %s\n", strerror(errno));
		return FAILURE;
	}

	if(listen(sd, BACKLOG) == -1){
		printf("Failure to listen on socket");
		return FAILURE;
	}

	//everything has gone successfully
	if(results->ai_family == AF_INET){
		int i = 0;
		printf("Starting server on address ");
		for(i = 0; i < 14; i++){
			printf("%d.", results->ai_addr->sa_data[i]);
		}
		printf("Canon:%s\n", results->ai_canonname);
	}
	else{
		printf("starting server on address %s", results->ai_addr->sa_data);
	}

	printf("Starting server on port %s\n", argv[1]);
	
	int packet_seq = 0;
	//Main server loop
	while(1){
		struct sockaddr client;
		socklen_t addr_size = sizeof client;

		int client_fd = accept(sd, (struct sockaddr *)&client, &addr_size);
		printf("Found client!\n");

		char buf[BUFFER_LEN];
		int bytes_received = 0;
		while(bytes_received = recv(client_fd, buf, BUFFER_LEN, 0) != 0){
			++packet_seq;
			printf("Received data! + %d bytes, %d packet\n", bytes_received, packet_seq);
			system("aplay seinfeld.wav");
		}
	}

	printf("Lost client");

	return 0;
}
