#include "filler.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    try {
        if(!argv[1]) {
            throw "No input file. Please try again";
        }
    }
    catch (const char *exc) {
        std::cout << exc << std::endl;
        return 0;
    }
    double seed = 1;
    if(argv[2]) seed = atof(argv[2]);
    else std::cout << "WARNING: Not foung seed value. Using default." << std::endl;
    char *file_name = argv[1];
    Map map(file_name, seed);
    map.process_map();
    Filler new_filler;
    new_filler.fill_map(map);
    map.create_xml();
    return 0;
}
