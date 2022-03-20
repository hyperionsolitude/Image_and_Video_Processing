// Utku ACAR 250206062
// gcc 562hw4partb.c -o 562hw4partb img_pro.c -lm
// Usage: ./562hw4partb [center_x] [center_y] [d] [Image file (*.ppm)]
// E.g.   ./562hw4partb 200 150 20 tilt_shift_test1.ppm


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


void matrixHandler(color **img, int NC, int NR,int x, int y,int radius,int stepnum,int step){
    color **img2;
    color **img3;
    int i,j,k,l,m,n,o,temp;
    int gaussian[]={1,4,7,4,1};
    int SIZE=5;
    int max_arrayr[25],max_arrayg[25],max_arrayb[25];
    int boundary=floor(SIZE/2.0);

    printf("step = %d\n",step);
    img2=(color **)alloc_color_img(NC,NR); /* allocate memory for img2 */
    img3=(color **)alloc_color_img(NC,NR); /* allocate memory for img3 */
    if (step != 0){
        for(i=boundary; i<NR-boundary; i++)     // y axis
        {

            for(j=boundary; j<NC-boundary; j++) // x axis
            {
                img2[i][j].r=0;
                img2[i][j].g=0;
                img2[i][j].b=0;
                m=0;
                if(((x-j)*(x-j)) + ((y-i)*(y-i)) < ((radius*stepnum)*(radius*stepnum)))   // x^2 + y^2 = r^2
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

        for(i=boundary; i<NR-boundary; i++)     // y axis
        {

            for(j=boundary; j<NC-boundary; j++) // x axis
            {
                img3[i][j].r=0;
                img3[i][j].g=0;
                img3[i][j].b=0;
                m=0;
                if(((x-j)*(x-j)) + ((y-i)*(y-i)) < ((radius*stepnum)*(radius*stepnum)))   // x^2 + y^2 = r^2
                {
                    img3[i][j].r=img[i][j].r;
                    img3[i][j].g=img[i][j].g;
                    img3[i][j].b=img[i][j].b;
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
        img_to_ppm_file(img,"562_hw4partb.ppm",NC,NR);/*  write to file */
        show_pgm_file("562_hw4partb.ppm");// Displays image
        matrixHandler(img, NC, NR,x,y,radius,++stepnum,--step);
    }
}



int main(int argc, char **argv)
{
color **img;
char *ppm_file;
int NC,NR,x,y,radius,maxstep;
if(argc!=5) {
  printf("\n Usage: ./562hw4partb [center_x] [center_y] [d] [Image file (*.ppm)]\n");
  printf("\n E.g.   ./562hw4partb 200 150 20 tilt_shift_test1.ppm\n");  
  exit(-1); }

x=atoi(argv[1]);
y=atoi(argv[2]);
radius=atoi(argv[3]);
ppm_file=argv[4];
img=(color **)ppm_file_to_img(ppm_file,&NC,&NR);/* read img and its size from file, also allocates memory for img */
show_pgm_file(ppm_file);

int closetoNR = (NR - y >= y) ? 0 : 1;
int closetoNC = (NC - x >= x) ? 0 : 1;

printf("closetoNR %d\n",closetoNR);
printf("closetoNC %d\n",closetoNC);

if (closetoNR && closetoNC){         // Farthest corner is (0,0)   
    printf("farthest = (0,0)");
    maxstep = ceil(sqrt(x*x + y*y)/radius);
}
else if (~closetoNR && closetoNC){   // Farthest corner is (NR,0)
    printf("farthest = (0,NR)");
    maxstep = ceil(sqrt((NR-y)*(NR-y) + y*y )/radius);
}
else if (closetoNR && ~closetoNC){  // Farthest corner is (0,NC)
    printf("farthest = (NC,0)");
    maxstep = ceil(sqrt(x*x + (NC-x)*(NC-x))/radius);
}
else if (~closetoNR && ~closetoNC){  // Farthest corner is (NR,NC)
    printf("farthest = (NC,NR)");
    maxstep = ceil(sqrt((NR-y)*(NR-y) + (NC-x)*(NC-x))/radius);
}

printf("\nNc: %d\tNr: %d\n",NC,NR); 
matrixHandler(img, NC, NR,x,y,radius,1,maxstep);

free_color_img(img);
return(1);
}