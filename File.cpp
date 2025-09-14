#include "File.h"
#include <string>
#include <iostream>

#include <sys/types.h>
#include <cstring>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

/*File::File(std::string& input) {
    file_name = input.c_str();
    size = 0;
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        std::cerr<<"error in opening file\n";
        close(fd);
    }
    else {
        struct stat st;
        fstat(fd, &st);
        size = st.st_size;
        close(fd);
    }
}*/

void File::ReadFile(std::string& input) {
    file_name = input.c_str();
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        std::cerr<<"error in opening file\n";
        close(fd);
    }
    else {
        struct stat st;
        fstat(fd, &st);
        size = st.st_size;
        content = new char[size];
        read(fd, content, size);
        close(fd);
    }
    /*for (int i = 0; i < size; ++i) {
        std::cout<<content[i]<<'\n';
    }*/
}

void File::CreateOutputFile(const std::string& input, char* new_content) {
    file_name = input.c_str();
    int fd = open(file_name, O_RDWR|O_CREAT);
    if (fd == -1) {
        std::cerr<<"error in creating file\n";
        close(fd);
    }
    else {
        content = new_content;
        size = strlen(new_content);
        write(fd,new_content, size);
        close(fd);
    }
}
unsigned long int File::GetSize() {
    return size;
}

char* File::GetContent() {
    return content;
}
void* pSequenceGenerate(void* arg) {
    genSequenceParams* sequenceParams = (genSequenceParams*)arg;
    sequenceParams->sequence[0] = sequenceParams->params.x;
    for (int i = 1; i < sequenceParams->sequence.size(); ++i) {
        sequenceParams->sequence[i] = (sequenceParams->params.a*sequenceParams->sequence[i-1]+sequenceParams->params.c)%sequenceParams->params.m;
        //std::cout<<sequenceParams->sequence[i]<<'\n';
    }
    pthread_exit(NULL);
}
