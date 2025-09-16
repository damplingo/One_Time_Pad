#include "File.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cerrno>
#include <pthread.h>
#include <vector>

struct ThreadContext {
    pthread_barrier_t* barrier;
    const std::vector<char>&  notepad;
    const std::vector<char>& content;
    std::vector<char>& result;
    int left;
    int right;
    int thread_id;
};

void* WorkerFunc(void* context) {
    ThreadContext* thisContext = static_cast<ThreadContext*>(context);
    printf("Поток %d начал работу: индексы %d - %d\n", thisContext->thread_id, thisContext->left, thisContext->right);
    for (int i = thisContext->left; i < thisContext->right; ++i) {
        thisContext->result[i] = thisContext->content[i] ^ thisContext->notepad[i];
    }
    int wait_res = pthread_barrier_wait(thisContext->barrier);
    pthread_exit(NULL);
}

bool CreateThreads(pthread_barrier_t* barrier, int numsThread, const std::vector<char>& notepad, const std::vector<char>& content, std::vector<char>& result, std::vector<pthread_t>& threads) {
    
    std::vector<ThreadContext> contexts;
    int size = content.size();
    int fixSize = size/numsThread;
    //создание контекстов для каждого потока
    int currentStart = 0;
    for (int i = 0; i < numsThread; ++i) {
        int iSize = fixSize;
        if (i < size%numsThread) {
            iSize++;
        }
        ThreadContext context{
            barrier,
            notepad,
            content,
            result,
            currentStart,
            currentStart+iSize,
            i
        };
        contexts.push_back(context);
        currentStart += iSize;
    }

    for (int i = 0; i < numsThread; ++i) {
        if (pthread_create(&threads[i], NULL, WorkerFunc, &contexts[i]) != 0) {
            std::cerr<<"ERROR IN THREAD CREATING\n";
            pthread_barrier_destroy(barrier);
            return false;
        }
        usleep(1000);
    }
    return true;
}

int GetCpuCores() {
    int cores = sysconf(_SC_NPROCESSORS_ONLN);
    return cores;
}


int main(int argc, char** argv)
{   
    std::string input;
    std::string output;
    char opt;
    LKGParams param{};
    while ((opt = getopt(argc, argv, "i:o:x:a:c:m:")) != -1) {

        switch (opt) {
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'x':
                param.x = atoi(optarg);;
                break;
            case 'a':
                param.a = atoi(optarg);
                break;
            case 'c':
                param.c = atoi(optarg);
                break;
            case 'm':
                param.m = atoi(optarg);
                break;
            default:
                std::cout<< opt<<" is unknown param\n";
                return 0;
                //break;
        }
    }
    File inputFile;
    bool openStatus = inputFile.ReadFile(input);

    if (!openStatus) {
        return 1;
    }

    std::vector<char> pSeq(inputFile.GetSize());
    genSequenceParams genThreadParams{param, pSeq};
    pthread_t gen_pthread;
    pthread_create(&gen_pthread, NULL, pSequenceGenerate, &genThreadParams);
    int join_res = pthread_join(gen_pthread, NULL);
    if (join_res != 0) {
        std::cout<<"ERROR IN JOIN "<<std::strerror(errno)<<'\n';
        return 1;
    }
    int numsThreads = GetCpuCores();
    std::vector<char> result(inputFile.GetSize());
    std::vector<pthread_t> threads(numsThreads);
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, numsThreads + 1);
    if (!CreateThreads(&barrier, numsThreads, pSeq, inputFile.GetContent(), result, threads)) {
        return 1;
    }
    printf("Главный поток ждет\n");
    int wait_result = pthread_barrier_wait(&barrier);
    printf("Главный поток продолжает\n");
    for (int i = 0; i < numsThreads; ++i) {
        int join_res = pthread_join(threads[i], NULL);
        if (join_res != 0) {
            std::cout<<"ERROR IN JOIN "<<std::strerror(errno)<<'\n';
            return 1;
        }
    }
    File outputFile;
    outputFile.CreateOutputFile(output, result);
    pthread_barrier_destroy(&barrier);
    return 0;
}
