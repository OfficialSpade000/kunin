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
//#include <omp.h>

#include "wavfile.h"

#define _USE_MATH_DEFINES

int __SUM;
int iteration = 50;

int      wfile(short *waveform, char *filename, int length);
double   fq_val[] = { 130.81, 146.83, 164.81, 174.61, 196.0,  220.0,  246.94, -1 };
char     *fq_key  = "C" "D" "E" "F" "G" "A" "B";
double   get_fq(char *key){ return fq_val[strcspn(fq_key, key)]; }
void     err(){printf("Usage: [options] [outfile]\n\nrun \"kunin -h\"\n");}

void option_dec(){
    printf("-o, --output                  set output filename.\n"
           "-a -amplitude                 set amplitude highest peak.\n"
	   "--waveform                    specify waveform type (sine, square)\n"
	   "--duration                    set duration in seconds (default=2)\n\n");
}
int main(int argc, char *argv[]){
	printf("kunin version alpha Copyright (c) 2019 Project kunin.\n");
    if (argc == 1){
        err();
        exit(1);
    }
	int b;
	if (strcmp(argv[1], "-h") == 0) {
		err();
		option_dec();
		exit(1);
	}
	double frequency = 150.0;
	double BPM = 123.0;

	int duration = 2;
	int volume = 2200;

	char *waveform_type = "sine";
	char *filename = NULL;

	int opt;
	int option_index = 0;

    static struct option long_options[] =
        {
          /* These options set a flag. */
        {"frequency",   required_argument,       0,   1},
        {"amplitude",      required_argument,       0,   2},
        {"output",      required_argument,       0, 'o'},
		{"iteration",   required_argument,       0,   5},
		{"duration",    required_argument,       0,   3},
		{"waveform",    required_argument,       0,   4},
        {0, 0, 0, 0}
        };

    while ((opt = getopt_long(argc, argv, "f:a:o:d:0:", long_options, &option_index)) != -1) {
        switch (opt) {
		case 5:
			iteration = atoi(optarg);
			break;
        case 1:		
			if (strlen(optarg) == 1){

				frequency = get_fq(optarg);
				char* _fname = optarg;
				strcat(_fname, ".wav");
				filename = _fname;
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
	if (filename == NULL){
		printf("\n\033[1;31mplease specify an ouput file: -o\n\033[0m");
		exit(1);
}
	const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*duration);
    int length = NUM_SAMPLES;
	short waveform[NUM_SAMPLES];
	int i;
	if (strcmp(waveform_type, "sine") == 0){

		#pragma omp parallel sections
		{
		#pragma omp section
		for(i=0;i<length;i++) {
			double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
			waveform[i] = volume*sin(frequency*t*2*M_PI);
		}
		#pragma omp section
		{
		while (i != length){
				printf("  %d/%d amplitude=\"%d\"\n", i, length, waveform[i]);
				sleep(1);
		}
		}
		}
		return wfile(waveform, filename, length);

	}
	
	if (strcmp(waveform_type, "square") == 0){
	
		int x;
		#pragma omp parallel sections
		{
		#pragma omp section
		for(x = 0; x < length; x++) {
			__SUM = 0;
			double t = ((double) x / WAVFILE_SAMPLES_PER_SECOND);	
			for (int n = 0; n < iteration; n++){
				double partialSUM = (((2*(1-(pow(-1,n)))))/n*M_PI)*(volume*sin(frequency*t*n*2*M_PI));
				//printf(" SUB SUM --> %d\n", sub_data);
				__SUM += partialSUM;
			}			
			waveform[ x ] = __SUM;
		}
		#pragma omp section
		{
		while (x != length){
				printf("  %d/%d amplitude=\"%d\"\n", x, length, __SUM);
				sleep(1);
		}
		}
		}
		return wfile(waveform, filename, length);

	}

	else
	{
		printf("\n\033[1;31munknown waveform \"%s\": not found\n\033[0m", waveform_type);
		return 1;
	}

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
	return 0;
}
