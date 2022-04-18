#include "db/database.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_ID_LENGHT 6

void takeClient(int fd)
{
    char buff[MAX_ID_LENGHT + 1];

    auto &users = db::instance();

    std::string msg = "Enter your username:\n(exactly " + std::to_string(MAX_ID_LENGHT) + " character)\n";
    write(fd, msg.c_str(), msg.size());

    int bytesRead = 0, result = 0;
    while (0 < (result = read(fd, buff + bytesRead, MAX_ID_LENGHT - bytesRead)))
        bytesRead += result;
    buff[bytesRead] = '\0';

    users.insert(buff, fd);
    dprintf(fd, "Welcome %s there are %d online user!\npress h for help!\n", buff, users.count());
    std::cout << buff << "connected!" << std::endl;

    while (true)
    {
        char input[2];
        if (0 == read(fd, &input, 2))
        {
            break;
        }
        if (input[0] == 'q')
            break;
        if (input[0] == 'h')
        {
            msg = "n: new massage\np: print online users\nq: exit\nh: help\n";
            write(fd, msg.c_str(), msg.size());
        }
        if (input[0] == 'n')
        {
            char to[MAX_ID_LENGHT + 1];
            to[MAX_ID_LENGHT] = '\0';
            read(fd, to, MAX_ID_LENGHT);
            int tofd;
            if (users.is_registered(to))
            {
                tofd = users.get_fd(to);
            }
            else
            {
                std::cout << "name not found! (" << to << ")" << std::endl;
                continue;
            }
            char buffer[1024];
            int bytesRead = 0;
            while ((bytesRead = read(fd, buffer, 1024)) > 0)
                write(tofd, buffer, bytesRead);
        }
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
    users.remove_fd(fd);
    std::cout << buff << "disconnected" << std::endl;
}

int main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockadd;
    memset(&sockadd, 0, sizeof(&sockadd));
    sockadd.sin_family = AF_INET;
    sockadd.sin_port = htons(1234);
    sockadd.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&sockadd, sizeof(sockadd)) < 0)
    {
        std::cerr << "bind failed!" << std::endl;
        return -1;
    }
    listen(sockfd, 10);

    while (true)
    {
        int reqfd = accept(sockfd, NULL, NULL);
        std::thread(takeClient, reqfd).detach();
    }

    return 0;
}
