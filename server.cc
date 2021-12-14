#include "helpers.hh"

void handle_connection(int cfd, std::string remote) {
    (void) remote;
    printf("Received incoming connection at cfd: %d\n", cfd);
    usleep(1000000);
    printf("Exiting\n");
}


int main(int argc, char** argv) {
    // Usage: ./weensydb [PORT]
    int port = 6162;
    if (argc >= 2) {
        port = strtol(argv[1], nullptr, 0);
        assert(port > 0 && port <= 65535);
    }

    // Prepare listening socket
    int fd = open_listen_socket(port);
    assert(fd >= 0);
    fprintf(stderr, "Listening on port %d...\n", port);

    while (true) {
        struct sockaddr addr;
        socklen_t addrlen = sizeof(addr);

        // Accept connection on listening socket
        int cfd = accept(fd, &addr, &addrlen);
        if (cfd < 0) {
            perror("accept");
            exit(1);
        }

        // Handle connection
        handle_connection(cfd, unparse_sockaddr(&addr, addrlen));
    }
}
