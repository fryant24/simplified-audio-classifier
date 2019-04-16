# cnn.py
# Python 3.5.1, TensorFlow 1.6.0, Keras 2.1.5
# ========================================================
# 导入模块
import os
import numpy as np
import keras as K
import tensorflow as tf
from keras import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import to_categorical
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelBinarizer
'''
%matplotlib inline
import matplotlib.pyplot as plt
plt.style.use('ggplot')
# basic handling
import os
import glob
import pickle
import numpy as np
# audio
import librosa
import librosa.display
import IPython.display
# normalization
import sklearn
# nn
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import to_categorical
from keras.callbacks import LearningRateScheduler
'''


os.environ['TF_CPP_MIN_LOG_LEVEL']='2'

DATA = 'data_104.npy'
TARGET = 'target_104.npy'


# 获取训练集与测试集
def get_train_test(split_ratio=.6, random_state=42):
    X = np.load(DATA)
    y = np.load(TARGET)
    assert X.shape[0] == y.shape[0]
    X = X.reshape(X.shape[0],8,13)
    train_x, test_x, train_y, test_y = train_test_split(X, y, test_size=(1 - split_ratio), random_state=random_state,
                                                        shuffle=True)
    return train_x, test_x, train_y, test_y


def main():

    # 0. 开始
    print("\nAudio dataset using Keras/TensorFlow ")


    # 1. 读取数据；one_hot编码
    print("Loading audio data into memory")
    train_x, test_x, train_y, test_y = get_train_test()
    train_x = train_x.reshape(train_x.shape[0],train_x.shape[1],train_x.shape[2],1)
    test_x = test_x.reshape(test_x.shape[0],test_x.shape[1],test_x.shape[2],1)
    train_y_hot = to_categorical(train_y)
    test_y_hot = to_categorical(test_y)

    print(train_x.shape)

    print(train_y_hot.shape)
    print(train_x.shape[2:])

    # 2. 定义模型
    model = Sequential()

    model.add(Convolution2D(32, 3, 3, activation='relu',input_shape = train_x.shape[1:]))
    model.add(MaxPooling2D((2, 2), dim_ordering="th"))
    model.add(Dropout(0.5)) 
    model.add(Convolution2D(16, (3, 3),  activation='relu'))
    model.add(MaxPooling2D((2, 2), dim_ordering="th"))
    model.add(Dropout(0.5))
    model.add(Flatten())
    model.add(Dense(5, activation='softmax'))
    model.compile(optimizer='Adam',
                    loss='categorical_crossentropy',
                    metrics=['accuracy'])
    model.summary(line_length=80)


    # 3. 训练模型
    b_size = 50
    max_epochs = 500
    print("Starting training ")
    history = model.fit(train_x, train_y_hot, epochs=max_epochs, batch_size=b_size, shuffle=True, verbose=1)
    print("Training finished \n")

    # 4. 评估模型
    eval = model.evaluate(test_x, test_y_hot, verbose=0)
    print("Evaluation on test data: loss = %0.6f accuracy = %0.2f%% \n" \
          % (eval[0], eval[1] * 100) )
          
    
'''
    # 5. 保存模型
    

'''

main()