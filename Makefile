CXX = g++
CXXFLAGS = -std=c++17

main: main.cpp board.cpp
	$(CXX) $(CXXFLAGS) main.cpp board.cpp -o main
