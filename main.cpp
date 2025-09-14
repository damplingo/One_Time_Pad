#include "File.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cerrno>
#include <pthread.h>
#include <vector>

using ll = long long;

//otp  -i input.txt -o output.txt -x 4212 -a 84589 -c 45989 -m 217728

void ChipherFunc(int l, int r, std::vector<char>& pSeq, std::vector<char>& content) {
    for (int i = l; i < r; ++i) {
        pSeq[i] = content[i]^pSeq[i];
    }
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
    inputFile.ReadFile(input);
    std::vector<char> pSeq(inputFile.GetSize());
    genSequenceParams genThreadParams{param, pSeq};
    pthread_t gen_pthread;
    pthread_create(&gen_pthread, NULL, pSequenceGenerate, &genThreadParams);
    int join_res = pthread_join(gen_pthread, NULL);
    if (join_res != 0) {
        int err = errno;
        std::cout<<"ERROR IN JOIN "<<std::strerror(errno)<<'\n';
    }
    std::vector<char> content(inputFile.GetContent(), inputFile.GetContent() + inputFile.GetSize());
    ChipherFunc(0, pSeq.size(), pSeq, content);

    
    File outputFile;
    outputFile.CreateOutputFile(output, pSeq.data());
    return 0;
}
