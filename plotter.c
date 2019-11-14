#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include "png.h"
#include <stdlib.h>
#include <string.h>
#define _USING_MATH_DEFINES

#define ERROR 1



void write_png_file(char *filename, int width, int height, int phase) {
    size_t alloC = 2048;
    png_bytep *row_pointers;
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(alloC);
        //printf("%d\n", png_get_rowbytes(png,info));
    }

    const int midY = ceil(height/2);
    printf("midY: %d\n", midY);
    png_byte color[] = {255,255,255};
    png_byte color_black[] = {0,0,0};
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    //printf("debug proc\n");

    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
        for(int i = 0; i < 3; ++i){
            if (y == ceil((midY-sin((x+phase+200)*0.1)*50))){
                //printf("y: %d ceil: %d\n",y, ceil(sin(x)));
                px[i] = color[i];
            }
            else {
                px[i] = color_black[i];
            }
            
        }
    }
  }

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGB,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_set_expand(png);
  png_write_info(png, info);
  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);
}



int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  if(argc > 3){
    exit(1);
  }
  for (int kl = 0; kl < 300; kl++){
    char file_frame[50] = " ";
    printf("num %d\n", kl);
    sprintf(file_frame, "%s%04d.png", argv[1], kl);

    write_png_file(file_frame, 500, 300, kl);
  }


  return 0;
}