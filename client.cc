#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "client-socket.h"

using namespace std;

static const int kTimeServerInaccessible = 1;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage:\n\t" << argv[0] << " server port" << endl;
        cout << "e.g.,\n\t" << argv[0] << " 127.0.0.1 12345" << endl;

        return 0;
    }
    // Clears screen
    cout << "\e[1;1H\e[2J";
    while (true) {
        cout << BOLDCYAN << "<client> " << RESET;
        char *line = NULL;
        size_t len = 0;
        ssize_t NumRead;
        NumRead = getline(&line, &len, stdin);
        if (NumRead == -1) {
            break;
        }
        line[--NumRead] = '\0';
        if (strcmp(line, "exit") == 0) {
            break;
        }
        int size = 0;
        char **cmds = trim(line, &size, (char *)" ");
        if (size > 0 && strcmp(cmds[0], "get") != 0) {
            cerr << "* Error: Wrong command" << endl;
            continue;
        }
        int client = createClientSocket(argv[1], atoi(argv[2]));
        if (client == kClientSocketError) {
            cerr << "Server could not be reached" << endl;
            cerr << "Aborting... " << endl;
            continue;
        }
        char s[128] = {'\0'};
        sprintf(s, "%d", size - 1);
        write(client, s, strlen(s));
        read(client, s, 128);
        for (int file = 1; file < size; file++) {
            char *filename = cmds[file];
            write(client, filename, strlen(filename));
            char res[128] = {'\0'};
            read(client, res, 128);
            if (strcmp(res, "error") == 0) {
                cerr << "* Error: File not found on the server side" << endl;
                continue;
            }
            long long int FileSize = atoll(res);
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if (fd < 0) {
                perror(filename);
                write(client, "error", strlen("error"));
                read(client, res, 128);
                continue;
            }
            write(client, "received", strlen("received"));
            cout << "Download Status (" << filename << "):" << endl;
            double downloaded = 0;
            char bytes[BUFFER_SIZE];
            long long int numBytesRead = 0, ReadBytes = 0, NumBytes = 0;
            while (numBytesRead < FileSize) {
                NumBytes = read(client, bytes + ReadBytes, BUFFER_SIZE - ReadBytes);
                numBytesRead += NumBytes;
                ReadBytes += NumBytes;
                downloaded = (double)(numBytesRead) / FileSize * 100;
                sprintf(res, "\r%0.2lf", downloaded);
                cout << res << "%";
                if (numBytesRead == FileSize || ReadBytes == BUFFER_SIZE) {
                    write(fd, bytes, ReadBytes);
                    ReadBytes = 0;
                }
            }
            cout << "\r100.00%" << endl;
            close(fd);
        }
        close(client);
    }
    return 0;
}