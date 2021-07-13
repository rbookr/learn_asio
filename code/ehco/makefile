CXX=g++
CFLAGS=-lpthread -g -std=c++2a

SRC=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp,%,$(SRC))
INCLUDE=-I../../asio-1.18.2/include

CXX=g++

SRC=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp,%,$(SRC))

all: $(TARGET)
	@echo done

% : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET)
