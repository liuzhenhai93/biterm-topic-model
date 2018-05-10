CC=g++
CFLAGS=-c -std=c++11 -fPIC -pipe -O3
MODEL_OBJ=model.o BTM.o
MODEL_EXE=BTM.so
INCLUDE=-I /usr/include/python2.7
all:$(MODEL_EXE) 

$(MODEL_EXE):$(MODEL_OBJ)
	$(CC) -shared --std=c++11 $(MODEL_OBJ) -o $@

model.o:model.h biterm.h doc.h
BTM.o:model.h BTM.h

%.o:%.cpp
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(MODEL_OBJ) $(MODEL_EXE)

