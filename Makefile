CXX = g++
CXXFLAGS = -Wall -g

markov: markov.o
	$(CXX) $(CXXFLAGS) -o markov.exe markov.o
