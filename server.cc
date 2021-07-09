#include <fcntl.h>
#include <sys/socket.h>  // for socket, bind, accept, listen, etc.
#include <unistd.h>      // for write

#include <climits>   // for USHRT_MAX
#include <cstring>   // for strlen
#include <ctime>     // for time, gmtime, strftim
#include <iostream>  // for cout, cerr, endl

#include "server-socket.h"

using namespace std;

static const short kDefaultPort = 12345;
static const int kWrongArgumentCount = 1;
static const int kServerStartFailure = 2;

static void publish(int client) {
    char s[128] = {'\0'};
    read(client, s, 128);
    long long int NumFiles = atoi(s);
    write(client, "received", strlen("received"));
    for (int file = 0; file < NumFiles; file++) {
        char filename[128] = {'\0'};
        read(client, filename, 128);
        int fd = open(filename, O_RDONLY);
        if (fd < 0) {
            write(client, "error", strlen("error"));
            continue;
        }
        long long int FileSize = lseek(fd, 0, SEEK_END);
        long long int numBytesToWrite, BytesRead = 0;
        char sz[128] = {'\0'};
        sprintf(sz, "%lld", FileSize);
        write(client, sz, strlen(sz));
        int NumRead = read(client, s, 128);
        s[NumRead] = '\0';
        if (strcmp(s, "error") == 0) {
            write(client, "received", strlen("received"));
            continue;
        }
        lseek(fd, 0, SEEK_SET);
        char bytes[BUFFER_SIZE];
        while (FileSize > 0) {
            BytesRead = read(fd, bytes, BUFFER_SIZE);
            numBytesToWrite = BytesRead;
            while (numBytesToWrite > 0) {
                numBytesToWrite -= write(client, bytes + BytesRead - numBytesToWrite, numBytesToWrite);
            }
            FileSize -= BytesRead;
        }
        close(fd);
    }
    close(client);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        cerr << "Usage: " << argv[0] << endl;
        return kWrongArgumentCount;
    }

    int server = createServerSocket(kDefaultPort);
    if (server == kServerSocketFailure) {
        cerr << "Error: Could not start server on port " << kDefaultPort << "." << endl;
        cerr << "Aborting... " << endl;
        return kServerStartFailure;
    }

    cout << "Server listening on port " << kDefaultPort << "." << endl;
    while (true) {
        int client = accept(server, NULL, NULL);
        publish(client);
    }

    return 0;
}
