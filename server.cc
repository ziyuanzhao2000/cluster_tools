#include "helpers.hh"
#include <cassert>
#include <cstdio>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

void handle_connection(int cfd, std::string remote) {
    (void) remote;

    FILE* fin = fdopen(cfd, "r");  
    FILE* fout = fdopen(cfd, "w");
    char buf[BUFSIZ], key[BUFSIZ];

    printf("Received incoming connection at cfd: %d\n", cfd);
    usleep(1000000);


    while (fgets(buf, BUFSIZ, fin)) {
        if(sscanf(buf, "echo %s ", key) == 1){
            pid_t p = fork();
            if(p == 0) {
                dup2(cfd, 1);
                const char *args[3];
                args[0] = "echo";
                args[1] = key;
                args[2] = NULL;
                execvp("echo", (char **)args);
                perror("Failed to execute the program!");
            }
            assert(p > 0);
            int status;
            pid_t exitpid = waitpid(p, &status, 0);
            assert(exitpid == p);
            assert(WIFEXITED(status));
        }
    }


    printf("Exiting\n");
    close(cfd);
}


int main(int argc, char** argv) {
    int port = 6163;
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
