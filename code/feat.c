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
    const char* fileoutpath="outpath_zcr.txt";
    char infile[100],outfile[100];
    int frames=0,i,it,coeff;
    double spectrum[FRAME_SIZE],mfcc_result;
    FILE *f1,*f2,*fpath,*foutpath;
    fpath = fopen(filepath, "r");
    foutpath = fopen(fileoutpath, "r");


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
        //过零率 每帧一个 （即与x轴交点个数） 体现频率特性  应该有用
        //首先归一化
        double max = tmp[0],min = tmp[0];
        for(i=0;i<FRAME_SIZE;i++){
                max = tmp[i]>max? tmp[i]:max;
                min = tmp[i]<min? tmp[i]:min;
        }
        //统计过零个数
        double zcr = 0;
        double norma = 0.5*(max+min);
        for (i = 0; i<FRAME_SIZE-1; i++){
                zcr += (tmp[i]-norma)*(tmp[i+1]-norma)<0? 1:0;
        }
        fprintf(f2,"过零率%f\n",zcr/FRAME_SIZE);
        //短时能量 即sum(xi^2) 体现信号强弱程度  应该有用 每帧一个 一般使用对数能量（定义：一帧内信号的平方和，再取以10为底的对数值，再乘以10）
        double sum = 0;
        for(i = 0; i < FRAME_SIZE ; i++){
                sum += tmp[i]*tmp[i];
        }
        fprintf(f2,"对数能量%f\n",log10(sum)*10);
        //短时平均幅度差  音频具有周期特性，平稳噪声情况下利用短时平均幅度差可以更好地观察周期特性。 n2复杂度 不建议使用 或者把framesize改很小
        /*
        double amdvec = 0;
        for(i = 0; i < FRAME_SIZE ; i++){
                for(int j = i; j<FRAME_SIZE; j++){
                        amdvec += std::abs(tmp[i]-tmp[j]);
                }
        }
        fprintf(f2,"短时平均幅度差 %f\n",amdvec);*/

        //谱熵
        double ep[FRAME_SIZE],probi,sumep = 0,H = 0;//H是熵 每帧一个
        p=fftw_plan_dft_1d(FRAME_SIZE,in,out,FFTW_FORWARD,FFTW_MEASURE);
        for(i=0;i<FRAME_SIZE;i++){
                in[i][0]=tmp[i];
                in[i][1]=0.0;
        }
        fftw_execute(p);
        for (i=0;i<FRAME_SIZE;i++){
                ep[i] = out[i][0]*out[i][0]+out[i][1]*out[i][1];
                sumep += ep[i];
        }
        for (i =0;i<FRAME_SIZE ;i++){
                probi = ep[i]/sumep;
                H += probi*log(probi)*(-1);
        }
        fprintf(f2,"谱熵%f\n",H);
		
		//自相关函数
		for(i=0;i<FRAME_SIZE; ++i){
			out[i][0] = ep[i]/FRAME_SIZE;
			out[i][1] = 0.0;
		}
		p = fftw_plan_dft_1d(FRAME_SIZE,out,in,FFTW_BACKWARD,FFTW_MEASURE);
		fftw_execute(p);
		int n_size = FRAME_SIZE/8;
		double sc[8];
		fprintf(f2,"自相关");
		for(i=0;i<8;++i){
			int j = i*n_size;
			sc[i]=(in[j][0]);
			fprintf(f2,"%f,",sc[i]);
		}
		fprintf(f2,"\n");
		
		//语谱
		double sg[8];
		fprintf(f2,"语谱");
		for(i=0;i<8;++i){
			int j = i*n_size;
			sg[i]=(100000.0/ep[j]);
			fprintf(f2,"%f,",sg[i]);
		}
		fprintf(f2,"\n");
		
		


    }
    printf("frames totally %d\n",frames);

        fclose(f1);
        fclose(f2);
        frames=0;

    }
        return 0;
}
