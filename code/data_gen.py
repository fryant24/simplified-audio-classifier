import os
import math
import numpy as np


DATA = 'data_64fb.npy'
TARGET = 'target_64fb.npy'
datafile0 = './features_64fb/0.txt'
datafile1 = './features_64fb/1.txt'
datafile2 = './features_64fb/2.txt'
datafile3 = './features_64fb/3.txt'
datafile4 = './features_64fb/4.txt'

cols=8*64

def txt_to_matrix(filename):
    file=open(filename)
    lines=file.readlines()
    #print lines
    #['0.94\t0.81\t...0.62\t\n', ... ,'0.92\t0.86\t...0.62\t\n']形式
    rows=math.ceil(float(len(lines))/cols) #矩阵行数

    datamat=np.zeros((rows,cols))#初始化矩阵
 
    row=0
    col=0
    for line in lines:
        datamat[row,col]=float(line)
        col+=1
        col=col%cols
        if col==0:
            row+=1

    
    return datamat


def main():
    datamat0=txt_to_matrix(datafile0)
    print("0-done!")
    datamat1=txt_to_matrix(datafile1)
    print("1-done!")
    datamat2=txt_to_matrix(datafile2)
    print("2-done!")
    datamat3=txt_to_matrix(datafile3)
    print("3-done!")
    datamat4=txt_to_matrix(datafile4)
    print("4-done!") 
    '''
    datamat0=datamat0.reshape(-1,104)
    datamat1=datamat1.reshape(-1,104)
    datamat2=datamat2.reshape(-1,104)
    datamat3=datamat3.reshape(-1,104)
    datamat4=datamat4.reshape(-1,104)
    '''
    len0=datamat0.shape[0]
    len1=datamat1.shape[0]
    len2=datamat2.shape[0]
    len3=datamat3.shape[0]
    len4=datamat4.shape[0]
    tarmat0=np.zeros((len0,1))
    tarmat1=np.zeros((len1,1))
    tarmat2=np.zeros((len2,1))
    tarmat3=np.zeros((len3,1))
    tarmat4=np.zeros((len4,1))
    for tarmat in tarmat0:
        tarmat[0]=0
    for tarmat in tarmat1:
        tarmat[0]=1
    for tarmat in tarmat2:
        tarmat[0]=2
    for tarmat in tarmat3:
        tarmat[0]=3
    for tarmat in tarmat4:
        tarmat[0]=4

    print(tarmat0[0][0])
    print(tarmat1[0][0])
    print(tarmat2[0][0])
    print(tarmat3[0][0])
    print(tarmat4[0][0])

    data=np.vstack((datamat0,datamat1,datamat2,datamat3,datamat4))
    target=np.vstack((tarmat0,tarmat1,tarmat2,tarmat3,tarmat4))
    np.save(DATA, data)
    np.save(TARGET, target)
    

if __name__ == "__main__":
    main()
