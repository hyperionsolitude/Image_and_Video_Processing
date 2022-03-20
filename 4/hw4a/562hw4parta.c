// Utku ACAR 250206062
// gcc 562hw4parta.c -o 562hw4parta img_pro.c -lm
// ./562hw4parta [row] [distance] [Image file (*.ppm)]
// E.g.   ./562hw4parta 200 20 tilt_shift_test1.ppm

#include "img_pro.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


int summer(int array[],int len){
    int i,temp=0;
    for (i=0;i<=len;i++){
        temp=temp+array[i];
    }
    return temp;
}


void matrixHandler(color **img, int NC, int NR,int width,int height,int stepnum,int step){
    color **img2;
    color **img3;
    int SIZE=5;
    int i,j,k,l,m,n,o,temp;
    int gaussian[]={1,4,7,4,1};
    int max_arrayr[25],max_arrayg[25],max_arrayb[25];
    int boundary=floor(SIZE/2.0);
    img2=(color **)alloc_color_img(NC,NR); /* allocate memory for img2 */
    img3=(color **)alloc_color_img(NC,NR); /* allocate memory for img3 */
    if (step != 0){
        for(i=boundary; i<NR-boundary; i++)
        {

            for(j=boundary; j<NC-boundary; j++)
            {
                img2[i][j].r=0;
                img2[i][j].g=0;
                img2[i][j].b=0;
                m=0;
                if((i>width-height*(stepnum)) && (i<width+height*(stepnum)))
                {
                    img2[i][j].r=img[i][j].r;
                    img2[i][j].g=img[i][j].g;
                    img2[i][j].b=img[i][j].b;
                }
                else
                {
                    for(l = j-boundary;l<=j+boundary;l++)
                    {
                        max_arrayr[m]=img[i][l].r*gaussian[m]/17;
                        max_arrayg[m]=img[i][l].g*gaussian[m]/17;
                        max_arrayb[m]=img[i][l].b*gaussian[m]/17;
                        m=m+1;
                    }
                    img2[i][j].r=summer(max_arrayr,m);
                    img2[i][j].g=summer(max_arrayg,m);
                    img2[i][j].b=summer(max_arrayb,m);
                }
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
                if((i>width-height*(stepnum)) && (i<width+height*(stepnum)))
                {
                    img3[i][j].r=img2[i][j].r;
                    img3[i][j].g=img2[i][j].g;
                    img3[i][j].b=img2[i][j].b; 
                }
                else
                {
                    for(k = i-boundary;k<=i+boundary;k++)
                    {
                        max_arrayr[m]=img2[k][j].r*gaussian[m]/17;
                        max_arrayg[m]=img2[k][j].g*gaussian[m]/17;
                        max_arrayb[m]=img2[k][j].b*gaussian[m]/17;
                        m=m+1;

                    }
                    img3[i][j].r=summer(max_arrayr,m);
                    img3[i][j].g=summer(max_arrayg,m);
                    img3[i][j].b=summer(max_arrayb,m);
                }
            }
        }

        for(i=boundary;i<NR-boundary;i++){
            for(j=boundary;j<NC-boundary;j++){
                img[i][j].r=img3[i][j].r;
                img[i][j].g=img3[i][j].g;
                img[i][j].b=img3[i][j].b;
            }
        }
        free_color_img(img2);
        free_color_img(img3);
        img_to_ppm_file(img,"562_hw4parta.ppm",NC,NR);/*  write to file */
        show_pgm_file("562_hw4parta.ppm");// Displays Grayscale image
        matrixHandler(img, NC, NR,width,height,++stepnum,--step);
    }
}



int main(int argc, char **argv)
{
color **img;
char *ppm_file;
int NC,NR,width,height,step;
if(argc!=4) {
  printf("\n Usage: ./562hw4parta [row] [distance] [Image file (*.ppm)]\n");
  printf("\n E.g.   ./562hw4parta 200 20 mandrill.ppm \n");  
  exit(-1); }

width=atoi(argv[1]);
height=atoi(argv[2]);
ppm_file=argv[3];
img=(color **)ppm_file_to_img(ppm_file,&NC,&NR);/* read img and its size from file, also allocates memory for img */
show_pgm_file(ppm_file);
int lowerstep = ((NR-width)/height)-1;
int higherstep = ((width)/height)-1;
    if(lowerstep > higherstep){
        step = lowerstep;
    }
    else{
        step = higherstep;
    }
matrixHandler(img, NC, NR,width,height,1,step);

free_color_img(img);
return(1);
}