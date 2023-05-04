//
// Created by Erik Peterson on 2/10/21.
//

#ifndef INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H
#define INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H


#include <pthread.h>

class Project3NetworkingModifier {
public:
    ~Project3NetworkingModifier();
    void modifyAndCopyFile(const char* sourceFile, const char* destFile);

private:
    static void* threadEntry(void* arg) noexcept;
    void listeningThreadMethod();

    bool readyToReceive = false;
    static const unsigned short portNumber = 12345;

    pthread_t listeningThread;
    pthread_mutex_t* mutexPtr = new pthread_mutex_t();
    pthread_cond_t* conditionPtr = new pthread_cond_t();
};


#endif //INC_21S_CS3377_PROJECT_PART4SOCKETMODIFIER_H
