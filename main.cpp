#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include "list.h"

using namespace std;

list * users;

void * takeClient(void * reqfd)
{
	int connection = (long) reqfd;
	char buff[MAX_ID_LENGHT + 1];
	
	dprintf(connection, "Enter your username:\n(at most %d character)\n", MAX_ID_LENGHT);
	bool tried = false;
	do {
		if (tried) dprintf(connection, "Username Exist! Please choose another one:\n");
		int bytesRead = 0, result = 0;
		while (0 < (result = read(connection, buff + bytesRead, MAX_ID_LENGHT - bytesRead)))
			bytesRead += result;
		buff[bytesRead] = '\0';
		tried = true;
	} while (users->insert(buff, connection));
	dprintf(connection, "Wellcome %s there are %d online user!\npress h for help!\n", buff, users->getCount());
	printf("%s connected!\n", buff);
	while (true) {
		char userinput[2];
		if (0 == read(connection, &userinput, 2)) {
			break;
		}
		if (userinput[0] == 'q')
			break;
		if (userinput[0] == 'h')
			dprintf(connection, "n: new massage\np: print online users\nq: exit\nh: help\n");
		if (userinput[0] == 'n') {
			char to[MAX_ID_LENGHT];
			read(connection, to, MAX_ID_LENGHT);
			int tofd = users->getConnection(to);
			char buffer[1024];
			int bytesRead = 0;
			while ((bytesRead = read(connection, buffer, 1024)) > 0)
				write(tofd, buffer, bytesRead);
		}	
		if (userinput[0] == 'p') {
			char * b = users->getIDs();
			write(connection, b, users->getCount() * MAX_ID_LENGHT + users->getCount());
			delete b;
		}
	}
	shutdown(connection, SHUT_RDWR);
	close(connection);
	users->remove(buff);
	printf("%s disconnected!\n", buff);
	return NULL;
}

int main(int argc, char * argv[])
{
	users = new list;
	
	write(1,"\E[H\E[2J",7);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in sockadd;
	memset(&sockadd, 0, sizeof(&sockadd));
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = htons(1234);
	sockadd.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, (struct sockaddr *) &sockadd, sizeof(sockadd));
	
	listen(sockfd, 10);
	while (true) {
		int reqfd = accept(sockfd, NULL, NULL);
		pthread_t pthid;
		pthread_create(&pthid, NULL, takeClient, (void *) reqfd);
	}
	
	return 0;
}
