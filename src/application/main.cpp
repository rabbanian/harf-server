#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#define MAX_ID_LENGHT 6

#include "db/database.h"

using namespace std;

void *takeClient(void *reqfd)
{
    int connection = (long)reqfd;
    char buff[MAX_ID_LENGHT + 1];

    db &users = db::instance();

    dprintf(connection, "Enter your username:\n(at most %d character)\n", MAX_ID_LENGHT);
    bool tried = false;
    {
        if (tried)
            dprintf(connection, "Username Exist! Please choose another one:\n");
        int bytesRead = 0, result = 0;
        while (0 < (result = read(connection, buff + bytesRead, MAX_ID_LENGHT - bytesRead)))
            bytesRead += result;
        buff[bytesRead] = '\0';
        tried = true;
    }
    users.insert(buff, connection);
    dprintf(connection, "Wellcome %s there are %d online user!\npress h for help!\n", buff, users.count());
    printf("%s connected!\n", buff);
    while (true)
    {
        char userinput[2];
        if (0 == read(connection, &userinput, 2))
        {
            break;
        }
        if (userinput[0] == 'q')
            break;
        if (userinput[0] == 'h')
            dprintf(connection, "n: new massage\np: print online users\nq: exit\nh: help\n");
        if (userinput[0] == 'n')
        {
            char to[MAX_ID_LENGHT + 1];
            to[MAX_ID_LENGHT] = '\0';
            read(connection, to, MAX_ID_LENGHT);
            int tofd;
            if (users.is_registered(to))
            {
                tofd = users.get_fd(to);
            }
            else
            {
                printf("name not found! (%s)\n", to);
                continue;
            }
            char buffer[1024];
            int bytesRead = 0;
            while ((bytesRead = read(connection, buffer, 1024)) > 0)
                write(tofd, buffer, bytesRead);
        }
        /*
        if (userinput[0] == 'p') {
            for (auto & u : users)
                write(connection, u.name.c_str(), sizeof(u.name.c_str()));
        }
        */
    }
    shutdown(connection, SHUT_RDWR);
    close(connection);
    users.remove_fd(connection);
    printf("%s disconnected!\n", buff);
    return NULL;
}

int main(int argc, char *argv[])
{
    write(1, "\E[H\E[2J", 7);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockadd;
    memset(&sockadd, 0, sizeof(&sockadd));
    sockadd.sin_family = AF_INET;
    sockadd.sin_port = htons(1234);
    sockadd.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&sockadd, sizeof(sockadd));

    listen(sockfd, 10);
    while (true)
    {
        int reqfd = accept(sockfd, NULL, NULL);
        pthread_t pthid;
        pthread_create(&pthid, NULL, takeClient, (void *)reqfd);
    }

    return 0;
}
