#include "File.h"
#include <string>
#include <iostream>

#include <sys/types.h>
#include <cstring>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK_SIZE (4 * 1024)    

bool File::ReadFile(std::string& input) {
    file_name = input;
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr<<"error in opening file\n";
        close(fd);
        return false;
    }
    else {
        struct stat st;
        fstat(fd, &st);
        if (st.st_size > CHUNK_SIZE) {
            std::cerr<<"file size exceeds CHUNK_SIZE = "<<CHUNK_SIZE<<'\n';
            close(fd);
            return false;
        }
        size = st.st_size;
        content.resize(size);
        read(fd, content.data(), size);
        close(fd);
        return true;
    }
}

void File::CreateOutputFile(const std::string& input, std::vector<char>& new_content) {
    file_name = input;
    int fd = open(file_name.c_str(),  O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        std::cerr<<"error in creating file\n";
        close(fd);
    }
    else {
        content = new_content;
        size = new_content.size();
        int res = write(fd, content.data(), size);
        if (res == -1) {
            std::cerr<<"error writing to file "<<std::strerror(errno)<<"\n";
            
        }
        close(fd);
    }
}
unsigned long int File::GetSize() {
    return size;
}

std::vector<char> File::GetContent() {
    return content;
}

void* pSequenceGenerate(void* arg) {
    genSequenceParams* sequenceParams = (genSequenceParams*)arg;
    if (sequenceParams->sequence.size() > 0) {
        sequenceParams->sequence[0] = sequenceParams->params.x;
        for (int i = 1; i < sequenceParams->sequence.size(); ++i) {
            sequenceParams->sequence[i] = (sequenceParams->params.a*sequenceParams->sequence[i-1]+sequenceParams->params.c)%sequenceParams->params.m;
        }
    }
    pthread_exit(NULL);
}
