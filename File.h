#pragma once

#include <string>
#include <vector>
using ll = long long;

struct LKGParams {
    int x;
    int a;
    int c;
    int m;
};

struct genSequenceParams {
    LKGParams& params;
    std::vector<char>& sequence;
};

class File {
    private:
        unsigned long int size = 0;//размер файла в байтах
        char* content;
        const char* file_name;
    public:
        //File(std::string& input);
        void ReadFile(std::string& input);//функция чтения файла в память
        unsigned long int GetSize();
        void CreateOutputFile(const std::string& name, char* new_content);
        char* GetContent();
};

void* pSequenceGenerate(void* arg);//генерация