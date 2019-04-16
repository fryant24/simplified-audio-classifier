# rnn.py
# Python 3.5.1, TensorFlow 1.6.0, Keras 2.1.5
# ========================================================
# 导入模块
import os
import numpy as np
import keras as K
import tensorflow as tf
from keras.preprocessing import sequence
from keras.models import Sequential
from keras.layers import Dense, Embedding
from keras.layers import LSTM
from keras.layers import GRU
from keras.datasets import imdb
from keras.utils import to_categorical
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelBinarizer

os.environ['TF_CPP_MIN_LOG_LEVEL']='2'

DATA = 'data_64fb.npy'
TARGET = 'target_64fb.npy'





# 获取训练集与测试集
def get_train_test(split_ratio=.6, random_state=42):
    X = np.load(DATA)
    y = np.load(TARGET)
    assert X.shape[0] == y.shape[0]
    train_x, test_x, train_y, test_y = train_test_split(X, y, test_size=(1 - split_ratio), random_state=random_state,
                                                        shuffle=True)

    train_x = train_x.reshape(-1,8,64)
    test_x = test_x.reshape(-1,8,64)
    return train_x, test_x, train_y, test_y


def main():

    # 0. 开始
    print("\nAudio dataset using Keras/TensorFlow ")


    # 1. 读取数据；one_hot编码
    print("Loading audio data into memory") 
    train_x, test_x, train_y, test_y = get_train_test()
    train_y_hot = to_categorical(train_y)
    test_y_hot = to_categorical(test_y)

    # 2. 定义模型
    init = K.initializers.glorot_uniform(seed=1)
    model = Sequential()
    model.add(K.layers.Dense(units=16, kernel_initializer=init, activation='relu', input_shape=(8,64) ))
    model.add(GRU(16, dropout=0.2, recurrent_dropout=0.2))
    #model.add(Dense(1, activation='sigmoid'))
    model.add(K.layers.Dense(units=5, kernel_initializer=init, activation='softmax'))
    model.compile(loss='binary_crossentropy',
                optimizer='adam',
                metrics=['accuracy'])
    model.summary(line_length=80)

    # 3. 训练模型
    b_size = 50
    max_epochs = 500
    print("Starting training ")
    h = model.fit(train_x, train_y_hot, batch_size=b_size, epochs=max_epochs, shuffle=True, verbose=1)
    print("Training finished \n")
    

    # 4. 评估模型
    eval = model.evaluate(test_x, test_y_hot, verbose=0)
    print("Evaluation on test data: loss = %0.6f accuracy = %0.2f%% \n" \
          % (eval[0], eval[1] * 100) )

    # 5. 保存模型
    print('saving model...')
    model.save("rnn_model.hdf5")



main()
