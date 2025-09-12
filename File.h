#pragma once

#include <string>

struct LKGParams {
    int x;
    int a;
    int c;
    int m;
};

class File {
    private:
        unsigned long int size;//размер файла в байтах
        char* content;
        const char* file_name;
    public:
        File(std::string& input);
        void ReadFile();//функция чтения файла в память
};