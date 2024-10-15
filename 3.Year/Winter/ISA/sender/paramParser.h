#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>

/*
 * program arguments structure
 */
typedef struct prog_args {
	char server[];

	char base[];

	char dstfile[]
	char srcfile[];
} Prog_args;

/*
 * parse args
 * argc - arg count, arg - arguments, *prog_args - structer pointer to save flags in
 * return 0 on success, 1 on failure
 */

 /**
  * Tato metoda je volána klientem (odesílatelem) při odeslání části dat serveru (příjemci).
  *
  * @param argc pocet argumentov
  * @param arg argumenty
  * @param prog_args pointer na strukturu uchovavajucu prepinace
  */
int parse_args(int argc, char* argv[], Prog_args* prog_args);