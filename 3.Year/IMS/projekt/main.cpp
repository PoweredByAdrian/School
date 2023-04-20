#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

#include "cell.hpp"
#include "etc.hpp"

void display();

int main(int argc, char* argv[])
{   

    int size;

    if (argc == 2)
        size = atoi(argv[1]);
    else
        return -1;
    
    int meters = 0;


    for (int t = 0; t < 160; t+10)
    {
        meters = t * 300;
        

    }

    return 0;
}

