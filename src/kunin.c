// Written by Al-buharie Amjari
// Copyright (C) 2019 Project Kunin

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <stddef.h>

#include "wavfile.h"
#include "kunin.h"

#define _USE_MATH_DEFINES

double BPM = 123.0;

size_t   iteration   =    50;
size_t   duration    =    2;
size_t   volume      =    2200;
size_t   NUM_SAMPLES;
size_t   length;

double   frequency   =    150.0;
short   *waveform;
int      sum_;

char *waveform_type = "sine";
char *filename = NULL;

int opt;
int option_index = 0;

double   fq_val[] = { 130.81, 146.83, 164.81, 174.61, 196.0,  220.0,  246.94, -1 };
char     *fq_key  = "C" "D" "E" "F" "G" "A" "B";
double   get_fq(char *key){ return fq_val[strcspn(fq_key, key)]; }
void     err(){printf("Usage: [options] [outfile]\n\n");}

char *get_filename_ext(const char *filex) {
	char *dot = strrchr(filex, '.');
	if(!dot || dot == filex) return "";
		return dot + 1;
}

void kunin_free_memory(){
	free(waveform);
	free(filename);
}

void option_dec(){
    printf("-o, --output                  set output filename.\n"
           "-a, --amplitude               set amplitude highest peak.\n"
		   "--waveform                    specify waveform type (sine, square)\n"
		   "--duration                    set duration in seconds (default=2)\n\n");
}

int wfile(short *waveform, char *filename, int length){
	for (int p = 0; p < 3; p++){
		printf("Value at %d is %d\n", p, waveform[p]);
	}

	FILE * f = wavfile_open(filename);
	if(!f) {
		printf("\ncouldn't create wavfile for writing: %s :(",strerror(errno));
		return 1;
	}
	wavfile_write(f,waveform,length);
	wavfile_close(f);
	puts("\nWriting successful! :)");
	printf("\033[0m");
	kunin_free_memory();
	return 0;
}

int kunin_arg_parser(int argc, char **argv){
	printf("kunin version alpha Copyright (c) 2019 Project kunin.\n");
    static struct option long_options[] =
        {
          /* These options set a flag. */
        {"frequency",   required_argument,       0,   1},
        {"amplitude",   required_argument,       0,   2},
        {"output",      required_argument,       0, 'o'},
		{"iteration",   required_argument,       0,   5},
		{"duration",    required_argument,       0,   3},
		{"waveform",    required_argument,       0,   4},
        {0, 0, 0, 0}
        };

    while ((opt = getopt_long(argc, argv, "f:a:o:d:0:", long_options, &option_index)) != -1) {
		printf("%d\n", opt);
        switch (opt) {
		case 5:
			iteration = atoi(optarg);
			break;
        case 1:		
			if (strlen(optarg) == 1){
				frequency = get_fq(optarg);
				break;			
			}
			else{
				frequency = atoi(optarg);
				break;
			}

            break;
        case 2:
            volume = atoi(optarg);
            break;
        case 'o':
			if (strcmp(get_filename_ext(optarg), "wav") != 0) {
				printf("\n\033[1;31munknown format \".%s\": not found\n\033[0m", get_filename_ext(optarg));
				exit(1);
			}
			NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*duration);
			length = NUM_SAMPLES;
			waveform = malloc(NUM_SAMPLES*sizeof(short));
            filename = optarg;
            break;
		case 3:
			duration = atoi(optarg);
			break;
		case 4:
			waveform_type = optarg;
			break;
        case '?': /* '?' */
            err();
            exit(EXIT_FAILURE);
        }
    }

}



int waveform_check_and_gen(){

	for (int i = 0; i < 4; i++){
        if (!strcmp(waveform_type, taG_[i].waves)){
            taG_[i].f();
            break;
        }
        if (!strcmp(taG_[i].waves, "NULL")){
            printf("\n\033[1;31mwaveform not found: .%s\n\033[0m", waveform_type);
            return 1;
        }
    }

}
int main(int argc, char **argv){
	
    if (argc<2 || !strcmp(argv[1], "-h")){
        err();
		option_dec();
        exit(1);
    }

	kunin_arg_parser(argc, argv);
	if (!filename){
		printf("\n\033[1;31mplease specify an ouput file: -o\n\033[0m");
		exit(1);
	}
	waveform_check_and_gen();
	wfile(waveform, filename, length);


}

