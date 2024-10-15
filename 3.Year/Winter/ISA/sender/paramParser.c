#include <string>
#include <iostream>
#include <stdio.h>
#include <getopt.h>

#include "paramParse.h"

#define EXIT_HELP 2

int parse_args(int argc, char* argv[], Prog_args* prog_args) {

    // print help
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("Pouzitie: ./dns_sender [-u UPSTREAM_DNS_IP] {BASE_HOST} {DST_FILEPATH} [SRC_FILEPATH]\n\n\
Popis parametrov:\n\
    -u slouží k vynucení vzdáleného DNS serveru\n\
    {BASE_HOST} slouží k nastavení bázové domény všech přenosů\n\
    {DST_FILEPATH} cesta pod kterou se data uloží na serveru\n\
    [SRC_FILEPATH] cesta k souboru který bude odesílán\n");

        return EXIT_HELP;
    }

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
            prog_args->port = atoi(optarg);
            break;
        case 'f':
            prog_args->unwantedDomainFile = optarg;
            break;
        case ':':

            std::cerr << "Prepinac -" << char(optopt) << " vyzaduje hodnotu\n";
            return EXIT_FAILURE;
            break;
        case '?':
            std::cerr << "Neznamy prepinac -" << char(optopt) << "\n";
            return EXIT_FAILURE;
            break;
        }
    }

    // check for mandatory parameters
    if (prog_args->server.empty() || prog_args->unwantedDomainFile.empty()) {
        std::cerr << "Parametry -s a -f jsou povinne.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int checkUnwanted(std::vector<std::string>* unwanted) {
    if (unwanted->empty()) {
        return EXIT_FAILURE;
    }
    // pop safety line if empty file was read
    unwanted->pop_back();
    return EXIT_SUCCESS;
}

std::vector<std::string> getUnwantedDomains(std::string fileName) {

    std::vector<std::string> s;
    std::fstream file;

    // open file with unwanted domains
    file.open(fileName, std::fstream::in);

    // check if file opened successfully
    if (!file.is_open()) {

        std::cerr << "Soubor \"" << fileName << "\" se nepodarilo otevrit.\n";
        return s;
    }
    else {
        std::string line;

        // read from file line by line
        while (getline(file, line)) {
            // skip lines beggining with # and empty lines
            if (line[0] == '#' || line == "")
                continue;

            s.push_back(line);
        }
        file.close();
    }
    s.push_back("!!!existing file!!!");
    return s;
}