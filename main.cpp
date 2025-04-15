#include "gerp.h"
#include <iostream>

int main(int argc, char *argv[]){

    gerp gerp; 

    string input = argv[1];
    string output = argv[2];

    gerp.run(input, output);
     
    return 0;
}