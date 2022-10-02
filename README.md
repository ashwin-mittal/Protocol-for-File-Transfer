# Assignment-6
> By Ashwin Mittal

## Background
- The server and client will be two different programs that will be run in different directories.

- The client will create a connection to the server and send requests to download files to the client directory. It will write the files into its directory.

- Progress is printed while downloading the file.

- Error handling, such as requesting missing files, is handled appropriately.

## Running
```bash
$ cd 2019101105/
$ make
```
This will create two executable files, one for the server and another for the client.

To remove all compiled object files.
```bash
$ make clean
```

### Server
```bash
$ ./server
```

### Client
```bash
$ ./client <server> <port>
```
To exit.
```bash
<client>​ exit
```

## Interface
### Server
```bash
Server listening on port 12345.
```
### Client
```bash
<client> get 1gb.txt 2gb.txt
Download Status (1gb.txt):
100.00%
Download Status (2gb.txt):
100.00%
```

## Note
In the implementation, I am using **12345** as the default port on which the server is listening, but if you want to change, you can change it in the **server.cc** file.