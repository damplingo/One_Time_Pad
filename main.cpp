#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <ostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//otp  -i input.txt -o -/path/to/cypher.txt -x 4212 -a 84589 -c 45989 -m 217728
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
        File(std::string& input) {
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

};

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
    return 0;
}
