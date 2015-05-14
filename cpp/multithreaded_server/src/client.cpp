#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string>
#include "defines.hpp"

using namespace std;

int create_client_socket()
{
	int sd;
	if (-1 == (sd = socket(AF_INET, SOCK_STREAM, 0))) {
		printf("ERROR: Failed to create client socket [%s]\n", strerror(errno));
		exit(1);
	}
	return sd;
}

void connect_client_socket(int sd, const char *hostname, int portnum)
{
	struct sockaddr_in temp_sa;

	memset(&temp_sa, 0x0, sizeof(temp_sa));
	temp_sa.sin_family = AF_INET;
	temp_sa.sin_port   = (unsigned short) htons(portnum);
//#ifdef DEBUG
	printf("Host Name: %s:%d\n", hostname, portnum);
//#endif
	inet_aton(hostname, &(temp_sa.sin_addr));

	memset(&(temp_sa.sin_zero), '\0', 8);
//#ifdef DEBUG
	printf("Trying to connect..\n");
//#endif
	if ( -1 == connect(sd, (struct sockaddr *) &temp_sa, sizeof(temp_sa))) {
		printf("ERROR: Failed to connect loc server [%s]\n", strerror(errno));
		exit(1);
	}
//#ifdef DEBUG
	printf("Connected to server\n");
//#endif
}

int start_client(const char *hostname, int portnum)
{
	int sd = create_client_socket();
	connect_client_socket(sd, hostname, portnum);
	return sd;
}

int main(int argc, char *argv[])
{
	char username[MAX_USER_NAME];
	getlogin_r(username, MAX_USER_NAME);
	printf("User: %s\n", username);
	int sd = start_client(HOST_NAME, PORT_NUMBER);
	string args = "Hello from " + string(username);
	if (argc > 1) {
		for (int i=1; i<argc; i++)	args += " " + string(argv[i]);
	}
	args += " " + string(END_OF_DATA);

	printf("Sendind arg string [%s]\n", args.c_str());
	int length = args.size();
	if (length >= MAX_PACKET_LENGTH) {
		printf("ERROR: Arguments length is too long. Supported %d bytes\n", MAX_PACKET_LENGTH);
		exit(1);
	}
	send(sd, args.c_str(), length, 0);
	int num_bytes = 0;
	std::string rcv_data;
	char buff[MAX_PACKET_LENGTH];
	while(1) {
		if (-1 == (num_bytes=recv(sd, buff, MAX_PACKET_LENGTH-1, 0))) {
			printf("ERROR: Failed to receive data from server\n");
			break;
		} else if (num_bytes == 0) {
			break;
		} else {
			buff[num_bytes] = '\0';
			rcv_data +=  string(buff);
		}
	}
	printf("Rcvd [%s]\n", rcv_data.c_str());
	close(sd);
	if(strstr(rcv_data.c_str(), "ERROR")) return 1;
	return 0;
}
