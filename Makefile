CXX = g++
CXXFLAGS = -ansi -O3 -pedantic -Wall -std=c++11
LDFLAGS = -lm -lOpenCL

imageSupport: imageSupport.o
	$(CXX) $(CXXFLAGS) -o $@ $+ $(LDFLAGS)


