#include "img_pro.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.1415926
// Utku ACAR 250206062
// gcc 562hw3partb.c -o 562hw3partb img_pro.c -lm
// ./562_562hw3partb Threshold Saturation Intensity mandrill.ppm
int summer(int array[],int len){
    int i,temp=0;
    for (i=0;i<=len;i++){
        temp=temp+array[i];
    }
    return temp;
}
int minfinder(int array[]){
    int i,minval=255;
    for(i=0;i<3;i++){
        if(array[i]<minval){
            minval=array[i];
        }
        else{
            minval=minval;
        }
    }
    return minval;
}
void huefinder(color **img,int NR,int NC,int threshold,float val2,int val3){
    
    int i,j,k,l,m,t,temparxr[9],temparxg[9],temparxb[9],temparyr[9],temparyg[9],temparyb[9],step_num;
    int gxx[NR][NC],gxy[NR][NC],gyy[NR][NC],xr,xg,xb,yr,yg,yb;
    float I[NR][NC],S[NR][NC],H[NR][NC],F[NR][NC],theta[NR][NC],minn=1000.0,maxx=0.0,stepsize;
    color **img2;//x gradient
    img2=(color **)alloc_color_img(NC,NR); /* allocate memory for img2 */
    color **imgx;//x gradient
    imgx=(color **)alloc_color_img(NC,NR); /* allocate memory for imgx */
    color **imgy;//y gradient
    imgy=(color **)alloc_color_img(NC,NR); /* allocate memory for imgy */
    int sobelx[]={-1,0,1,-2,0,2,-1,0,1};
    int sobely[]={1,2,1,0,0,0,-1,-2,-1};
    int R,G,B;
    for (i=1;i<NR-1;i++){
        for(j=1;j<NC-1;j++){
            R=img[i][j].r;
            G=img[i][j].g;
            B=img[i][j].b;
            int minarray[]={R,G,B};
            theta[i][j]=(acos((((R-G)+(R-B))/2.0)/(sqrt((R-G)*(R-G)+(R-B)*(G-B)))));
            if(B<=G){
                H[i][j]=theta[i][j];
            }
            else if(B>G){
                H[i][j]=2.0*PI-theta[i][j];
            }
            S[i][j]=1.0-((3.0*minfinder(minarray))/(R+G+B));
            I[i][j]=(R+G+B)/3.0;
            //printf("O=%f  H=%f  S=%f  I=%f\n",theta[i][j],H[i][j],S[i][j],I[i][j]);
            m=0;
            for(k=i-1;k<=i+1;k++){
                for(l=j-1;l<=j+1;l++){
                    temparxr[m]=img[k][l].r*sobelx[m];
                    temparyr[m]=img[k][l].r*sobely[m];
                    temparxg[m]=img[k][l].g*sobelx[m];
                    temparyg[m]=img[k][l].g*sobely[m];
                    temparxb[m]=img[k][l].b*sobelx[m];
                    temparyb[m]=img[k][l].b*sobely[m];
                    m++;
                }
            }
            xr=imgx[i][j].r=summer(temparxr,m);
            xg=imgx[i][j].g=summer(temparxg,m);
            xb=imgx[i][j].b=summer(temparxb,m);
            yr=imgy[i][j].r=summer(temparyr,m);
            yg=imgy[i][j].g=summer(temparyg,m);
            yb=imgy[i][j].b=summer(temparyb,m);
            gxx[i][j]=(xr*xr)+(xg*xg)+(xb*xb);
            gyy[i][j]=(yr*yr)+(yg*yg)+(yb*yb);
            gxy[i][j]=(xr*yr)+(xg*yg)+(xb*yb);
            F[i][j]=sqrt(0.5*((gxx[i][j]+gyy[i][j])-(gxx[i][j]-gyy[i][j])*cos(2.0*theta[i][j])+2.0*gxy[i][j]*sin(2.0*theta[i][j])));
            //printf("%f\n",F[i][j]);
            if(F[i][j]>maxx){
                maxx=F[i][j];
            }
            if(F[i][j]<minn){
                minn=F[i][j];
            }
            else if(F[i][j]<=maxx) {
                maxx=maxx;
            }
            else if(F[i][j]>=minn) {
                minn=minn;
            }
            if(F[i][j]<threshold){
                img2[i][j].r=0;
                img2[i][j].g=0;
                img2[i][j].b=0;

            }
            else if(F[i][j]>=threshold){
                img2[i][j].r=img[i][j].r;
                img2[i][j].g=img[i][j].g;
                img2[i][j].b=img[i][j].b;
            }
        }
    }
    stepsize=((maxx-threshold)/(2.0*PI/3.0));//1 degree difference
    for(i=0;i<NR;i++){
        for(j=0;j<NR;j++){
            if(F[i][j]>=threshold && F[i][j]<=(maxx-threshold)/2.0){
                R=img2[i][j].r;
                G=img2[i][j].g;
                B=img2[i][j].b;
                step_num=(F[i][j]/stepsize)-(2.0*PI/3.0);
                img2[i][j].g=val3*(1.0-val2);
                img2[i][j].b=val3*(1.0+((val2*cos(step_num))/(cos((PI/3.0)-step_num))));
                img2[i][j].r=3.0*val3-(G+B);
            }
            else if(F[i][j]>(maxx-threshold)/2.0){
                R=img2[i][j].r;
                G=img2[i][j].g;
                B=img2[i][j].b;
                step_num=(F[i][j]/stepsize)-(4.0*PI/3.0);
                img2[i][j].r=val3*(1.0-val2);
                img2[i][j].g=val3*(1.0+((val2*cos(step_num))/(cos((PI/3.0)-step_num))));
                img2[i][j].b=3.0*val3-(R+G);
            }
        }
    }

    img_to_ppm_file(img2,"562_hw3partb.ppm",NC,NR);/*  write to file */
    free_color_img(img2);
    show_pgm_file("562_hw3partb.ppm");// Displays Grayscale image
}

int main(int argc, char **argv)

{
color **img;
char *ppm_file;
int NC,NR,threshold,val3;
float val2;
if(argc!=5) {
  printf("\n Usage: ./562hw3partb Threshold Saturation Intensity [Image file (*.ppm)]\n");
  printf("\n E.g.   ./562hw3partb 150 1.0 1 mandrill.ppm \n");  
  exit(-1); }


threshold=atoi(argv[1]);
val2=atoi(argv[2]);
val3=atoi(argv[3]);
ppm_file=argv[4];
img=(color **)ppm_file_to_img(ppm_file,&NC,&NR);/* read img and its size from file, also allocates memory for img */
show_pgm_file(ppm_file);
huefinder(img,NR,NC,threshold,val2,val3);
free_color_img(img);
return(1);
}