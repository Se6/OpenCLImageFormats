SYSTEM := $(shell uname -s)
CXX = g++
CXXFLAGS = -pedantic -Wall -std=c++11

ifeq ($(SYSTEM), Darwin)
LDFLAGS = -framework OpenCL
else
LDFLAGS = -lOpenCL
endif

imageSupport: imageSupport.o
	$(CXX) $(CXXFLAGS) -o $@ $+ $(LDFLAGS)

all: imageSupport

clean: 
	rm *.o
	rm imageSupport
