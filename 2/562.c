#include "img_pro.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Utku ACAR 250206062
// gcc 562.c -o 562 img_pro.c -lm
// ./562 SIZE RECURSION_NUMBER cathedral.pgm 
int median(int array[],int len){
    int i,j,max,maxc = 0;
    for(i=0;i<(len/2.0)+0.5;i++){
        for(j=0;j<len;j++){
            max=array[maxc];
            if(max<array[j]){
                maxc=j;
            }
        } 
        if(i ==(len/2)){
            return array[maxc];
        }
        array[maxc]= 0;  
    }
}
int main(int argc, char **argv)

{
unsigned char **img, **img2;
char *pgm_file;
int i,j,k,l,m,n,NC,NR,ifo=0,RECURSION;
float V_max=0.0,SIZE=0.0;
int max_array[1000];
if(argc!=4) {
  printf("\n Usage: 562 SIZE RECURSION_NUMBER [Image file (*.pgm)]\n");
  printf("\n E.g.   562 5 2 cathedral.pgm \n");  
  exit(-1); }


SIZE=atof(argv[1]);
RECURSION=atoi(argv[2]);
pgm_file=argv[3];


img=pgm_file_to_img(pgm_file,&NC,&NR);
show_pgm_file(pgm_file);
img2=alloc_img(NC, NR);
for(i=0;i<NR;i++){
    for(j=0;j<NC;j++){
        img2[i][j]=img[i][j];
    }
}


for(n=0;n<RECURSION;n++){
    for(i=0; i<NR; i++)
    {
        for(j=0; j<NC; j++)
        {
            int imin = i-floor(SIZE/2.0); // temp index to avoid negatif indexes
            int imax = i+floor(SIZE/2.0); // temp index to avoid negatif indexes
            int jmin = j-floor(SIZE/2.0); // temp index to avoid negatif indexes
            int jmax = j+floor(SIZE/2.0); // temp index to avoid negatif indexes
            m=0;
            // <<<<<  Edge cases index correction
            if (imin < 0)
                while(imin < 0) imin++;

            else if (imax > NR-1)
                while(imax > NR-1) imax--;


            // >>>>> Edge cases index correction

            for(k = imin;k<=imax;k++)
            {

            if (jmin < 0)
                while(jmin < 0) jmin++;
            else if (jmax > NC-1)
                while(jmax > NC-1) jmax--;

                for(l = jmin;l<=jmax;l++)
                {
                    max_array[m]=img2[k][l];
                    m=m+1;
                }
            }
            //printf("m = %d \n",m);
            img2[i][j]=median(max_array,m);
        }
    }
    img_to_pgm_file(img2,"562.pgm",NC,NR);//Converting image(matrix) to Grayscale(actual image)
    show_pgm_file("562.pgm");// Displays Grayscale image
}

free_img(img);
free_img(img2);

return(1);
}

