#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

char *filename;
void err(){
    printf("Usage: [-i filename] [options]\nrun \"kunin --help\"\n");
}

void option_dec(){
    err();
    printf("-f                   frequency"
           "-v                   volume");

}
int main(int argc, char *argv[])
{   
    static struct option long_options[] =
        {
          /* These options set a flag. */
          {"help", no_argument,       0, 'h'},
          {"frequency",   no_argument,       0, 'f'},
          {"volume",     no_argument,       0, 'v'},
          {"input",     no_argument,       0, 'i'},

          {0, 0, 0, 0}
        };
    int opt;
    int option_index = 0;
    while ((opt = getopt(argc, argv, "fvi:", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'f':
            frequency = (int) optarg;
            break;
        case 'v':
            volume = (int) optarg;
            break;
        case 'i':
            flags = 1;
            tfnd = 1;
            break;
        default: /* '?' */
            err();
            exit(EXIT_FAILURE);
        }
        if (flags == 0) {
            err();
            exit(EXIT_FAILURE);

        }
    }
    if (argc == 1){
        err();
    }
}