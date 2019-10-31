#include <math.h>
#define _USING_MATH_DEFINES

#ifndef LIB_WAV_FOU
#define LIB_WAV_FOU

extern size_t     iteration;
extern size_t     NUM_SAMPLES;
extern size_t     length;
extern size_t     volume;
extern size_t     duration;
extern short     *waveform;
extern double     frequency;
extern int        sum_;

size_t i;
void saw_func(){
    
}

void square_func(){
    #pragma omp parallel sections
    {
    #pragma omp section
    for(i = 0; i < length; i++) {
        sum_ = 0;
        double t = ((double) i / WAVFILE_SAMPLES_PER_SECOND);	
        for (int n = 0; n < iteration; n++){
            double partialSUM = (((2*(1-(pow(-1,n)))))/n*M_PI)*(volume*sin(frequency*t*n*2*M_PI));
            //printf(" SUB SUM --> %d\n", sub_data);
            sum_ += partialSUM;
        }			
        waveform[ i ] = sum_;
    }
    #pragma omp section
    {
    while (i != length){
            printf("  %d/%d amplitude=\"%d\"\n", i, length, waveform[i]);
            sleep(1);
    }
    }
    }


}

void sine_func(){
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
}
struct waveform_data
{
    char *waves;
    void (*f)();
};

struct waveform_data taG_[] =
{
    {"saw",       saw_func    },
    {"square",    square_func },
    {"sine",      sine_func   },
    {"NULL", }
};

#endif