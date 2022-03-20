#include "img_pro.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Utku ACAR 250206062
// gcc 562hw3parta.c -o 562hw3parta img_pro.c -lm
// ./562hw3parta Rflag Gflag Bflag Iteration mandrill.ppm
int summer(int array[],int len){
    int i,temp=0;
    for (i=0;i<=len;i++){
        temp=temp+array[i];
    }
    return temp;
}


void matrixHandler(color **img, int NC, int NR,int RECURSION,int Rflag,int Gflag,int Bflag){
    color **img2;
    color **img3;
    int SIZE=3;
    int i,j,k,l,m,n,o,temp;
    int gaussian[]={1,2,1};
    int max_arrayr[8],max_arrayg[8],max_arrayb[8];

    int boundary=floor(SIZE/2.0);
    boundary=1;
    img2=(color **)alloc_color_img(NC,NR); /* allocate memory for img2 */
    img3=(color **)alloc_color_img(NC,NR); /* allocate memory for img3 */
    if (RECURSION > 0){
        for(i=boundary; i<NR-boundary; i++)
        {

            for(j=boundary; j<NC-boundary; j++)
            {
                img2[i][j].r=0;
                img2[i][j].g=0;
                img2[i][j].b=0;
                m=0;
                for(l = j-boundary;l<=j+boundary;l++)
                {
                    max_arrayr[m]=img[i][l].r*gaussian[m]/4;
                    max_arrayg[m]=img[i][l].g*gaussian[m]/4;
                    max_arrayb[m]=img[i][l].b*gaussian[m]/4;
                    m=m+1;
                }
                img2[i][j].r=summer(max_arrayr,m);
                img2[i][j].g=summer(max_arrayg,m);
                img2[i][j].b=summer(max_arrayb,m);
            }
        }

        for(i=boundary; i<NR-boundary; i++)
        {
            for(j=boundary; j<NC-boundary; j++)
            {
                img3[i][j].r=0;
                img3[i][j].g=0;
                img3[i][j].b=0;
                m=0;
                for(k = i-boundary;k<=i+boundary;k++)
                {
                    max_arrayr[m]=img2[k][j].r*gaussian[m]/4;
                    max_arrayg[m]=img2[k][j].g*gaussian[m]/4;
                    max_arrayb[m]=img2[k][j].b*gaussian[m]/4;
                    m=m+1;

                }
                img3[i][j].r=summer(max_arrayr,m);
                img3[i][j].g=summer(max_arrayg,m);
                img3[i][j].b=summer(max_arrayb,m);
            }
        }

        for(i=boundary;i<NR-boundary;i++){
            for(j=boundary;j<NC-boundary;j++){
                if(Rflag){
                    img[i][j].r=img3[i][j].r;
                }
                if(Gflag){
                    img[i][j].g=img3[i][j].g;
                }
                if(Bflag){
                    img[i][j].b=img3[i][j].b;
                }
            }
        }
        free_color_img(img2);
        free_color_img(img3);
        img_to_ppm_file(img,"562_hw3parta.ppm",NC,NR);/*  write to file */
        show_pgm_file("562_hw3parta.ppm");// Displays Grayscale image
        matrixHandler(img, NC, NR,RECURSION-1,Rflag,Gflag,Bflag);
    }
}



int main(int argc, char **argv)

{
color **img;
char *ppm_file;
int NC,NR,RECURSION,Rflag,Gflag,Bflag;
if(argc!=6) {
  printf("\n Usage: ./562hw3parta [Rflag] [Gflag] [Bflag] [num. of iter] [Image file (*.ppm)]\n");
  printf("\n E.g.   ./562hw3parta 1 0 1 3 mandrill.ppm \n");  
  exit(-1); }


Rflag=atoi(argv[1]);
Gflag=atoi(argv[2]);
Bflag=atoi(argv[3]);
RECURSION=atoi(argv[4]);
ppm_file=argv[5];


img=(color **)ppm_file_to_img(ppm_file,&NC,&NR);/* read img and its size from file, also allocates memory for img */
show_pgm_file(ppm_file);

matrixHandler(img, NC, NR,RECURSION,Rflag,Gflag,Bflag);

free_color_img(img);
return(1);
}