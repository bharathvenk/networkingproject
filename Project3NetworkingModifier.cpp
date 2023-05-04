//
// Created by Erik Peterson on 2/10/21.
//

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "FileCopier.h"
#include "Project3NetworkingModifier.h"
#include "FileModifyException.h"

Project3NetworkingModifier::~Project3NetworkingModifier() {
    delete mutexPtr;
    mutexPtr = nullptr;
    delete conditionPtr;
    conditionPtr = nullptr;
}

void Project3NetworkingModifier::modifyAndCopyFile(const char* sourceFile,
                                                   const char* destFile) {
    //initializing mutex and condition
    pthread_mutex_init(mutexPtr, nullptr);
    pthread_cond_init(conditionPtr, nullptr);
    //creating new thread
    if(pthread_create(&listeningThread, nullptr, threadEntry, this) != 0) {
        throw FileModifyException("Failed to create thread");
    }
    //lock mutex
    pthread_mutex_lock(mutexPtr);
    //waiting on mutex
    while (!readyToReceive) {
        pthread_cond_wait(conditionPtr, mutexPtr);
    }
    //creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        pthread_mutex_unlock(mutexPtr);
        throw FileModifyException("Failed to create socket");
    }

    // set up connection information
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(portNumber);

    // call connect to establish connection
    if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        throw FileModifyException("Failed to connect socket");
    }

    // transfer data from source file to socket using FileCopier
    FileCopier::makeCopy(sourceFile, serverSocket);

    // close the socket
    close(serverSocket);

    // unlock mutex
    pthread_mutex_unlock(mutexPtr);

    // wait for thread to finish
    pthread_join(listeningThread, nullptr);

    // Clean up the mutex and condition variables
    pthread_mutex_destroy(mutexPtr);
    pthread_cond_destroy(conditionPtr);

}

// Static method for thread startup
void* Project3NetworkingModifier::threadEntry(void* arg) noexcept{
    auto* modifier = (Project3NetworkingModifier*) arg;
    modifier->listeningThreadMethod();
    return nullptr;
}


void Project3NetworkingModifier::listeningThreadMethod() {
    // creating new socket
    int serverSocket2 = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket2 < 0) {
        throw FileModifyException("Failed to create socket");
    }

    // set up connection information
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(portNumber);

    // call bind to assign the address to the socket
    if (bind(serverSocket2, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        throw FileModifyException("Failed to bind socket");
    }

    // call listen to wait for incoming connections
    if (listen(serverSocket2, 1) < 0) {
        throw FileModifyException("Failed to listen on socket");
    }

    // Lock mutex
    pthread_mutex_lock(mutexPtr);

    // Signal main thread that listening socket is ready
    readyToReceive = true;
    pthread_cond_signal(conditionPtr);

    // Unlock mutex
    pthread_mutex_unlock(mutexPtr);

    // Accept incoming connections
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    int clientSocket = accept(serverSocket2, (struct sockaddr*) &clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        throw FileModifyException("Failed to accept incoming connection");
    }

    // copy data from socket to destination file using FileCopier
    FileCopier::makeCopy(clientSocket, Util::outputFilename);

    // close client socket
    close(clientSocket);

    // close server socket
    close(serverSocket2);
}

