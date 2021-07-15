#include "header.hpp"

//  The send() call:
//  int send(int sockfd, const void *msg, int len, int flags);
//  sockfd - the socket where we send data to
//  msg - pointer to message to send
//  Jsut setting flags to 0 for now


//	The recv() call:
//	int recv(int sockfd, void *buf, int len, int flags);
// -1 on error et.
// 0 is returned when remote side has closed the connection on us
int main() {
	std::string msg = "Beej was here!";
	char	buffer[2048];
	int len = strlen(msg.c_str());
	int sockfd = socket(1, 1, 1);
	int bytes_sent = send(sockfd, msg.c_str(), len, 0);
	int bytes_received = recv(sockfd, buffer, 2048, 0);
	//send returns the number of bytes actually sent
	return 0;
}