//#include <stdio.h>

//#include "paramParser.h"
#include <getopt.h>

#include <string.h>
#include <stdio.h>



typedef struct prog_args {
	char server[16];

	char base[];

    char dstfile[];
    char srcfile[];
} Prog_args;






int parse_args(int argc, char* argv[], Prog_args* prog_args) {

    //parse args using getopt 
    int opt;
    while ((opt = getopt(argc, argv, ":s:f:p:")) != -1) {
        switch (opt)
        {
        case 's':
            prog_args->server = optarg;
            break;
        case 'p':
            // check port num validity (conversion etc)
            if (strcmp(optarg, "0") != 0 && atoi(optarg) <= 0) {
                printf("Neplatne cislo portu.\n");
                return EXIT_FAILURE;
            }
            prog_args->base = atoi(optarg);
            break;
    }
    return 0;
}











int main(int argc, char* argv[]) {


    parse_args(argc, argv, prog_args);

    return 0;
	

}