#include "img_pro.h"
#include <math.h>
#include <stdlib.h>
// Utku ACAR 250206062
// gcc 562hw1.c -o 562hw1 img_pro.c -lm
// ./562hw1 GAMMA cathedral.pgm 

int main(int argc, char **argv)

{
unsigned char **img, **img2;
char *pgm_file;
int i,j,NC,NR;
float V_max=0.0,GAMMA=0.0;
if(argc!=3) {
  printf("\n Usage: 562 GAMMA [Image file (*.pgm)]\n");
  printf("\n E.g.   562 GAMMA cathedral.pgm \n");
  exit(-1); }


GAMMA=atof(argv[1]); /* 2nd input as float
Because user could want the image brighter with using gamma values such as 0.4*/

pgm_file=argv[2]; // 3rd input for which image should be corrected

img=pgm_file_to_img(pgm_file,&NC,&NR); //to save uncorrected pixel values
show_pgm_file(pgm_file);//shows uncorrected image

img2=alloc_img(NC, NR);// to allocate space for corrected pixel values (same size with img)

for(i=0; i<NR; i++)// to Determine maximum gray value (It can be maximum of 255)(White)
{
	for(j=0; j<NC; j++)
	{
		img2[i][j]=0;
		if(V_max < img[i][j])
		{
			V_max = img[i][j];
		}

	}
}
for(i=0; i<NR; i++)
{
	for(j=0; j<NC; j++)
	{
		img2[i][j]=(pow((img[i][j]/V_max),GAMMA)*V_max);//Gamma Correction Equation
	}
}
img_to_pgm_file(img2,"562hw1.pgm",NC,NR);//Converting image(matrix) to Grayscale(actual image)
show_pgm_file("562hw1.pgm");// Displays Grayscale image

free_img(img);
free_img(img2);

return(1);
}

