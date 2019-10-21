// Written by Al-buharie Amjari
// Copyright (C) 2019 Project Kunin

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
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
int b = 0;

// func that deals with harmonics.
void summation(int volume, double frequency, double t);
int wfile(short *waveform, char *filename, int length, int argc, char *argv[]);

double fq_val[] = {
	130.81, 146.83, 164.81, 174.61, 196.0,  220.0,  246.94, -1
};
char *fq_key =
	"C" "D" "E" "F" "G" "A" "B";
double get_fq(char *key){
	return fq_val[strcspn(fq_key, key)];
}

void err(){printf("Usage: [options] [outfile]\nrun \"kunin -h\"\n");}

void option_dec(){
    printf("-f                   frequency (default = 100)\n"
           "-v                   volume (default = 2200)\n");
}

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
}

void pprogress(int f, int x, int length){ 
	while (x != length)
	{
		delay(1);
		printf("Writing: %d/%d amplitude=\"%d\"\n", x, length, __SUM);
	}
	

}
int main(int argc, char *argv[]){

    if (argc == 1){
        err();
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
        {"help",        no_argument,             0, 'h'},
        {"frequency",   required_argument,       0,   1},
        {"volume",      required_argument,       0,   2},
        {"output",      required_argument,       0, 'o'},
		{"iteration",   required_argument,       0,   5},
		{"duration",    required_argument,       0,   3},
		{"waveform",    required_argument,       0,   4},
        {0, 0, 0, 0}
        };

    while ((opt = getopt_long(argc, argv, "f:v:o:hd:0:", long_options, &option_index)) != -1) {
		printf("%d\n", opt);
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
				
				printf("%.2f\n", get_fq(optarg));
				break;			
			}
			else{
				frequency = atoi(optarg);
				printf("%.2f\n", frequency);
				break;
			}
			
            break;
        case 2:
            volume = atoi(optarg);
            break;
        case 'o':
            filename = optarg;
            break;
        case 'h':
            err();
            option_dec();
            exit(1);
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

	const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*duration);
    int length = NUM_SAMPLES;
	short waveform[NUM_SAMPLES];

	if (strcmp(waveform_type, "sine") == 0){
					
		int i;
		for(i=0;i<length;i++) {
			double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
			waveform[i] = volume*sin(frequency*t*2*M_PI);
			int progress = ( 900 )*b;
			if (progress == i){
				b++;
				printf("Writing: %d/%d amplitude=\"%d\"\n", i, length, waveform[i]);
			} 			
		}	
		return wfile(waveform, filename, length, argc, argv);

	}
	
	if (strcmp(waveform_type, "square") == 0){
	
		int x;
		for(x = 0; x < length; x++) {
			double t = ((double) x / WAVFILE_SAMPLES_PER_SECOND);	
			for (int n = 0; n < iteration; n++){
				double partialSUM = (((2*(1-(pow(-1,n)))))/n*M_PI)*(volume*sin(frequency*t*n*2*M_PI));
				//printf(" SUB SUM --> %d\n", sub_data);
				__SUM += partialSUM;
			}			
			waveform[ x ] = __SUM;
			int progress = ( 900 )*b;
			if (progress == x){
				b++;
				printf("Writing: %d/%d amplitude=\"%d\"\n", x, length, __SUM);
			} 
			__SUM = 0;

		}
		return wfile(waveform, filename, length, argc, argv);		

	}

	else
	{
		printf("waveform \"%s\" not found :(", waveform_type);
		return 1;
	}

}

int wfile(short *waveform, char *filename, int length, int argc, char *argv[]){
	for (int p = 0; p < 3; p++){
		printf("Value at %d is %d\n", p, waveform[p]);
	}
	if (filename == NULL){
		filename = argv[argc-1];
	}


	FILE * f = wavfile_open(filename);
	if(!f) {
		printf("couldn't create wavfile for writing: %s :(",strerror(errno));
		return 1;
	}
	wavfile_write(f,waveform,length);
	wavfile_close(f);
	puts("Done Writing!! :)\n");
	free(f);
	free(filename);
	return 0;
}
