# Cryptography I DES Homework 1

This assignement consists of two programs, one to encrypt and send a file, and the other to receive and decrypt the file. This program uses a basic two round DES encryption, on 8 bit chunks with a 10 bit key.

##Project Details
The program is written in C++, and makes use of the WinSock2 Library.

### Toydes Class
The actual DES implementation is contained in a class called Toydes, which consists of two files: toydes.h and toydes.cpp. The class has two public functions encrypt() and decrypt(). Each function takes two arguments, a message to encrypt or decrypt, in the form of an unsigned char, and a key with which to encrypt or decrypt the message, in the form of an unsigned int. The result is returned as an unsigned char.

### Server Implementation
The server is implemented in server.cpp which imports the Toydes Class mentioned above. Using the Winsock2 Library, the server listens on a port for incomming connections which it receives and decrypts using a key provided by the user. The bytes it receives are saved to a file after being decrypted.

### Client Implentaion
The client is implemented in client.cpp, which works very much the same way as the server, using the Toydes Class and WinSock2 Library. The client creates a socket and attempts to connect to the port and server provided by the user, and with a successful connection, sends the file specified by the user that is encrypted byte by byte using the key provided.


## Usage

### Building the Project
The project is compiled into two separate programs, the client and the server, using g++. The client is compiled with
```
g++ client.cpp toydes.cpp -o client.exe -lws2_32
```
The server is compiled the same way
```
g++ server.cpp toydes.cpp -o server.exe -lws2_32
```

### Running the Program
Each program is run from commandline, specifying the arguments to use. The server must be started first.
```
$ ./server <port> <key>
```
This designates the port for the server to listen on, as well as the key that it will use to decrypt any incomming messages. Next the client is run.
```
$ ./client <server-address> <port> <file-name> <key>
```
This tells the client the address to which to send the file, the port to connect with, the file to be opened and encrypted, and the key with which to encrypt it. Both the client and server will accept keys that are longer than 10 bits, however all but the last 10 bits will be cut off and unsued by the program. Once the client is started, it will connect to the server, transmit the file, and terminate itself. Likewise, the server will wait until it has received the entire file, which it will save, and the terminate itself. The client looks for files in the folder it is installed in, and the server saves files to the folder it is installed in as well.


## Authors

* **Jacob Doskocil** 