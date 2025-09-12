#include "File.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>


//otp  -i input.txt -o -/path/to/cypher.txt -x 4212 -a 84589 -c 45989 -m 217728


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
    File inputFile(input);
    inputFile.ReadFile();
    return 0;
}
