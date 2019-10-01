/*
 * Written by Al-buharie Amjari
 * Copyright (C) 2019, Project Kunin
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, this permission notice, and the following
 * disclaimer shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OF OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#define _USE_MATH_DEFINES

#include "wavfile.h"


int __SUM;
int iteration = 100;


void err(){
    printf("Usage: [-i filename] [options]\nrun \"kunin --help\"\n");
}

void option_dec(){
    printf("-f                   frequency (default = 100)\n"
           "-v                   volume (default = 2200)\n");
}

void summation(int volume, double frequency, double t){
    for (int n = 0; n < iteration; n++){
        double partialSUM = (((2*(1-(pow(-1,n)))))/n*M_PI)*(volume*sin(frequency*t*n*2*M_PI));
        //printf(" SUB SUM --> %d\n", sub_data);
        __SUM += partialSUM;
    }
    

}
int main(int argc, char *argv[]){

    if (argc == 1){
        err();
        exit(1);
    }

	const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*2);
	int length = NUM_SAMPLES;
	char *filename = NULL;
	int volume = 2200;
	short waveform[NUM_SAMPLES];
	double frequency = 100.0;
	
	int opt;
    int option_index = 0;

    static struct option long_options[] =
        {
          /* These options set a flag. */
          {"help", no_argument,       0, 'h'},
          {"frequency",   required_argument,       0, 'f'},
          {"volume",     required_argument,       0, 'v'},
          {"output",     required_argument,       0, 'o'},
		  {"iteration",     required_argument,       0, 0},

          {0, 0, 0, 0}
        };

    while ((opt = getopt_long(argc, argv, "f:v:o:h0:", long_options, &option_index)) != -1) {
        switch (opt) {
		case 0:
			iteration = atoi(optarg);
			break;
        case 'f':
            frequency = atoi(optarg);
            break;
        case 'v':
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
        case '?': /* '?' */
            err();
            exit(EXIT_FAILURE);
    
        }   
    }	
	
	for(int x = 0; x < length; x++) {
		double t = ((double) x / WAVFILE_SAMPLES_PER_SECOND);	
		summation(volume, frequency, t);
		waveform[x] = __SUM;		
		__SUM = 0;
	}
	for (int p = 0; p < 3; p++){
		printf("Value at %d is %d\n", p, waveform[p]);
	}	


	FILE * f = wavfile_open(filename);
	if(!f) {
		printf("couldn't create wavfile for writing: %s",strerror(errno));
		return 1;
	}

	wavfile_write(f,waveform,length);
	wavfile_close(f);

	return 0;
}
