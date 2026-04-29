# effective_mobile_cpp
test

g++ -std=c++11 -I. -c Serializer.cpp -o Serializer.o
g++ -std=c++11 -I. -c main.cpp -o main.o
g++ Serializer.o main.o -o test
