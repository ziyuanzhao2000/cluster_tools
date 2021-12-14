#include "helpers.hh"
#include <netdb.h>

// usage: -h [host] -p [port]
int main(int argc, char** argv) {
    const char* host = "10.250.64.177"; // use the server's ip here.
    const char* port = "6162";

    // parse arguments
    int opt;
    while ((opt = getopt(argc, argv, "h:p:")) >= 0) {
        if (opt == 'h') {
            host = optarg;
        } else if (opt == 'p') {
            port = optarg;
        }
    }

    // look up host and port
    struct addrinfo hints, *ais;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // use TCP
    hints.ai_flags = AI_NUMERICSERV;
    // if (strcmp(host, "cs61") == 0) {
    //     host = "cs61.seas.harvard.edu";
    // }
    int r = getaddrinfo(host, port, &hints, &ais);
    if (r != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        exit(1);
    }

    // connect to server
    int fd = -1;
    for (auto ai = ais; ai && fd < 0; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, 0);
        if (fd < 0) {
            perror("socket");
            exit(1);
        }

        r = connect(fd, ai->ai_addr, ai->ai_addrlen);
        if (r < 0) {
            close(fd);
            fd = -1;
        }
    }
    if (fd < 0) {
        perror("connect");
        exit(1);
    }
    freeaddrinfo(ais);

    // 
    printf("Connection established at fd %d\n", fd);

    
}