SYSTEM := $(shell uname -s)
CXX = g++
CXXFLAGS = -pedantic -Wall -std=c++11

ifeq ($(SYSTEM), Darwin)
LDFLAGS = -lm -framework OpenCL
else
LDFLAGS = -lm -lOpenCL
endif

imageSupport: imageSupport.o
	$(CXX) $(CXXFLAGS) -o $@ $+ $(LDFLAGS)


