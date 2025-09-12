#include "File.h"
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

File::File(std::string& input) {
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
        std::cout<<size<<'\n';
        close(fd);
    }
}

void File::ReadFile() {
    content = new char[size];
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        std::cerr<<"error in opening file\n";
        close(fd);
    }
    read(fd, content, size);
    /*for (int i = 0; i < size; ++i) {
        std::cout<<content[i]<<'\n';
    }*/
}
