objects = client.o server.o helpers.o
executables = client server

CXX = c++
O = 2

server : server.o
	$(CXX) -o server server.o helpers.o

client : client.o 
	$(CXX) -o client client.o helpers.o 

client.o : client.cc helpers.hh 
	$(CXX) -std=gnu++11 -c client.cc

server.o : server.cc helpers.hh 
	$(CXX) -std=gnu++11 -c server.cc

helpers.o : helpers.cc helpers.hh
	$(CXX) -std=gnu++11 -c helper.cc

