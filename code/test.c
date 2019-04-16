#include <complex>
#include <fftw3.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "libmfcc.h"

#define FRAME_SIZE 8192
#define NUM 82
#define STAGE1 1
#define STAGE2 17
#define STAGE3 21
#define STAGE4 50
#define STAGE5 58

int main(){
    const char* filepath="path.txt";
    const char* fileoutpath="outpath.txt";
    char infile[100],outfile[100];
    int frames=0,i,it,coeff;
    double spectrum[FRAME_SIZE],mfcc_result;
    FILE *f1,*f2,*fpath,*foutpath;
    fpath = fopen(filepath, "r");
    foutpath = fopen(fileoutpath, "r");
    //const char* infile="./audio/noise_cn/1_安静环境/ZOOM0001/ZOOM0001_LR.WAV";
    //const char* outfile="./audio/noise_cn/1_安静环境/ZOOM0001/mfcc.dat";



    for(it=1;it<=NUM;it++){

    fscanf(fpath, "%s", infile);
    printf("file #%d\ncurrent file:%s\n",it,infile);

    if(it==STAGE1||it==STAGE2||it==STAGE3||it==STAGE4||it==STAGE5){
        fscanf(foutpath, "%s", outfile);
    }

    f1 = fopen(infile, "r");
    f2 = fopen(outfile, "a");
    fftw_complex in[FRAME_SIZE],out[FRAME_SIZE];
    fftw_plan p;
    printf("successfully open!\n");
    while (1) {
        if (feof(f1)){
          printf("end!\n");
          break;
        }
        short tmp[FRAME_SIZE];
        fread(tmp, sizeof(short), FRAME_SIZE, f1);
	//printf("successfully read!\n");
	frames++;
        //printf("frames no.%d\n",frames);
        //for (i=0;i<FRAME_SIZE;i++) printf("%d:%d\n",i,tmp[i]);
        /*
        rnnoise_process_frame(st, x, x);
        for (i=0;i<FRAME_SIZE;i++) tmp[i] = x[i];
        if (!first) fwrite(tmp, sizeof(short), FRAME_SIZE, fout);
        first = 0;
        */
        p=fftw_plan_dft_1d(FRAME_SIZE,in,out,FFTW_FORWARD,FFTW_MEASURE);
        for(i=0;i<FRAME_SIZE;i++){
                in[i][0]=tmp[i];
                in[i][1]=0.0;
        }
        fftw_execute(p);
        for (i=0;i<FRAME_SIZE;i++){
                spectrum[i]=sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]);
                //printf("%d:%f:%f\n",i,out[i][0],out[i][1]);
        }
        for(coeff = 0; coeff < 13; coeff++)
	{
		mfcc_result = GetCoefficient(spectrum, 16000, 48, 128, coeff);
                fprintf(f2,"%f\n",mfcc_result);
	}
    }
    printf("frames totally %d\n",frames);
    /*
    fftw_complex in[N], out[N];
    fftw_plan p;
    p=fftw_plan_dft_1d(N,in,out,FFTW_FORWARD,FFTW_MEASURE);
    for(int i=0;i <N;i ++) {
        in[i][0]=i;
        in[i][1]=0.0;
    }
   
 
    fftw_execute(p);
 
    for(int i=0;i <N;i ++){
        cout<<out[i][0]<<" "<<out[i][1]<<endl;
    }
    complex<double> temp = 0.0;
    for(int k =0; k < N; k ++){
        double pi = 4*atan(1.0);
        temp += exp(complex<double>(0.0,-2.0*pi*3*k/N))*complex<double>(in[k][0],in[k][1]);
    }
    cout<<"out[3] is "<<temp<<endl;
 
    fftw_complex out1[N];
 
    fftw_plan p1;
 
    p1=fftw_plan_dft_1d(N,out1,in,FFTW_BACKWARD,FFTW_MEASURE);
 
    for(int i=0;i <N;i ++){
        out1[i][0]=out[i][0];
        out1[i][1]=out[i][1];
    }
 
    fftw_execute(p1);
    for(int i=0;i <N;i ++){
        cout<<in[i][0]<<" "<<in[i][1]<<endl;
    }
 
 
 
    fftw_destroy_plan(p);
    fftw_destroy_plan(p1);

	*/
        fclose(f1);
        fclose(f2);
        frames=0;

    }
        return 0;
}
