/* copy_cat - a copy of the `cat` UNIX utility
 *
 * This application mirrors the functionality of the cat
 * UNIX utility, which conCATenates data from standard in
 * and outputs the results to standard out. This often takes
 * the form of files.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct gargs_t {
    int nFlag;       /* -n option */
} gargs;

void copy_file(FILE *file);
void display_usage();

int main(int argc, char *argv[])
{
    char c;
    int i;
    FILE *file;

    while ((c = getopt(argc, argv, "nh?")) != -1) {
        switch(c) {
        case 'n':
            gargs.nFlag = 1;
            break;
        case 'h':	/* fall-through is intentional */
        case '?':
            display_usage();
            break;
        }
    }

    argc -= optind;
    argv += optind;

    for(i=0; i<argc; i++) {
        if(!strcmp(argv[i], "-"))
            file = stdin;
        else {
            file = fopen(argv[i], "r");
        }
        copy_file(file);
        if (file != stdin)
            fclose(file);
        else
            clearerr(file);
    }
    if (!argc)
        copy_file(stdin);

    return 0;
}

/* Output file to stdout line by line 
 */
void copy_file(FILE *file) {
    int linecount = 0;
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, file) != NULL) {
        if(gargs.nFlag){
            printf("%6d  ", ++linecount);
        }
        printf(buffer);
    }
}

/* Display program usage and exit
 */
void display_usage() {
    puts("copycat - clone of cat");
}
