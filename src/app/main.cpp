#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <asio.hpp>
#include "db/database.h"

void takeClient(asio::ip::tcp::socket socket)
{
    auto &users = db::instance();

    std::string msg = "Enter your username: \n";
    socket.write_some(asio::buffer(msg));

    socket.read_some(asio::buffer(msg));

    users.insert(msg, fd);
    dprintf(fd, "Welcome %s there are %d online user!\npress h for help!\n", buff, users.count());
    std::cout << msg << "connected!" << std::endl;

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
    asio::io_context context;
    asio::ip::tcp::acceptor acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234));

    asio::ip::tcp::socket socket(acceptor.accept());

    return 0;
}
