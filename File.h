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
        std::vector<char>  content;
        std::string file_name;
    public:
        bool ReadFile(std::string& input);//функция чтения файла в память
        unsigned long int GetSize();
        void CreateOutputFile(const std::string& name, std::vector<char>& new_content);
        std::vector<char> GetContent();
};

void* pSequenceGenerate(void* arg);//генерация