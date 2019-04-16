#include <complex>
#include <fftw3.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "libmfcc.h"
#include "rnn_data.h"
#include "rnn.h"

#define FRAME_SIZE 8192
#define CATEGORYS  5
#define MFCCNUMS   13
#define FB_MFCC    64
#define TIMESTEPS  8
#define SAMPLING   1
#define FFT        0
#define BEGIN      2
#define END        2

int main(int argc, char **argv){
    #if FFT

    if (argc!=2) {
        fprintf(stderr, "usage: %s <audio path>\n", argv[0]);
        return 0;
    }
    int frames=0,i,coeff,cot=0;
    float sum[CATEGORYS],re[CATEGORYS],mfcc[MFCCNUMS],show[MFCCNUMS];
    double spectrum[FRAME_SIZE],mfcc_result;
    FILE *f1 = fopen(argv[1], "r");
    fftw_complex in[FRAME_SIZE],out[FRAME_SIZE];
    fftw_plan p;
    RNNState rs;
    memset(sum, 0.0, sizeof(double)*CATEGORYS);
    memset(&rs, 0.0, sizeof(rs));
    printf("successfully open!\n");
    while (1) {
        if (feof(f1)){
          printf("end!\n");
          break;
        }
        short tmp[FRAME_SIZE];
        fread(tmp, sizeof(short), FRAME_SIZE, f1);
        frames++;
        #if SAMPLING

        
        if(frames<(TIMESTEPS*(BEGIN-1)+1))
            continue;
        else if(frames>TIMESTEPS*END)
            break;

        #endif


        p=fftw_plan_dft_1d(FRAME_SIZE,in,out,FFTW_FORWARD,FFTW_MEASURE);
        for(i=0;i<FRAME_SIZE;i++){
                in[i][0]=tmp[i];
                in[i][1]=0.0;
        }
        fftw_execute(p);
        for (i=0;i<FRAME_SIZE;i++){
                spectrum[i]=sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]);
        }
        for(coeff = 0; coeff < MFCCNUMS; coeff++)
	    {
		    mfcc_result = GetCoefficient(spectrum, 16000, 48, 128, coeff);
            mfcc[coeff]=mfcc_result;
	    }
        compute_rnn(&rs, re, mfcc);

        if(frames%TIMESTEPS==0){
            for(i=0;i<CATEGORYS;i++){
                sum[i]+=re[i];
            }
            cot++;
            memset(&rs, 0.0, sizeof(rs));
            /*
            for(i=0;i<CATEGORYS;i++){
                show[i]=re[i];
                printf("%f ",show[i]);
            }
            printf("\n");
            */
        }



    }
    printf("computing samples totally %d\n",cot);
    fclose(f1);
    for(i=0;i<CATEGORYS;i++){
        sum[i]=sum[i]/cot;
        printf("%f ",sum[i]); 
    }
    printf("\n");


    #else

    if (argc!=3) {
        fprintf(stderr, "usage: %s <audio feature path> <sampling begin point>\n", argv[0]);
        return 0;
    }
    RNNState rs;
    int id=0,flag=0,FB_BEGIN=0;
    float tmp,re[CATEGORYS],mfcc[TIMESTEPS][FB_MFCC];
    memset(&rs, 0.0, sizeof(rs));
    FILE *f1 = fopen(argv[2], "r");
    for(int j=0;j<strlen(argv[1]);j++){
        FB_BEGIN+=((int)argv[1][j]-48)*pow(10,strlen(argv[1])-j-1);
    }
    printf("%d\n",FB_BEGIN);
    while(1){
        fscanf(f1,"%f",&tmp);
        id++;
        if(id<(FB_BEGIN-1)*TIMESTEPS*FB_MFCC)
            continue;
        else{
            flag=1;
            break;
        }
    }
    if(flag==1){
        for(int i=0;i<TIMESTEPS;i++){
            for(int j=0;j<FB_MFCC;j++){
                fscanf(f1,"%f",&tmp);
                mfcc[i][j]=tmp;
            }
        }
    }
    for(int lp=0;lp<TIMESTEPS;lp++){
        compute_rnn(&rs,re,mfcc[lp]);
    }
    for(int i=0;i<CATEGORYS;i++){
        printf("%f ",re[i]); 
    }
    printf("\n");

    #endif
    return 0;
}
