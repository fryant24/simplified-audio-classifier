rnn_test:rnn_test.o libmfcc.o rnn.o rnn_data.o
	g++ rnn_test.o libmfcc.o rnn.o rnn_data.o -o rnn_test -lfftw3 -L/usr/local/fftw/lib -I/usr/local/fftw/include
rnn_test.o:rnn_test.c
	g++ -c rnn_test.c -lfftw3 -L/usr/local/fftw/lib -I/usr/local/fftw/include
libmfcc.o:libmfcc.c
	g++ -c libmfcc.c
#rnn.o:rnn_data.c rnn_data.o
#	g++ -c rnn_data.c -rnn_data.o
rnn_data.o:rnn_data.c rnn.c
	g++ -c rnn_data.c rnn.c
clean:
	rm -rf *.o rnn_test
