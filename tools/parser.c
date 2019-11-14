#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct option_properties
{
    char *arglong;
    int tag;
};


int main(int argc, char **argv){

    struct option_properties options[] =
    {
        {"--input",  0},
        {"--output", 1},
        {"NULL", 2}
    };

    enum {

        input = 0,
        output = 1
    };
    printf("%d\n", argc);

    int i;
    int flag;
    int k = 1;
    for (; k < argc; ){
        for (i = 0; i < 3; i++){
            if (!strcmp(argv[k], options[i].arglong)){
                switch (options[i].tag)
                    {
                    case input:
                        printf("argument input: %s\n", argv[k+1]);
                        break;
                    case output:

                        printf("argument output: %s\n", argv[k+1]);
                        break;
                    default:
                        printf("unk %s\n", argv[k]);
                        exit(1);
                    }
                    printf("1\n");
                    k += 2;
                break;  
            }
            else {
                if (i == 2) {
                    printf("Ooooops: %s %d", argv[k], k);
                    exit(1);
                }
            }            
            
        }       
    }
}
