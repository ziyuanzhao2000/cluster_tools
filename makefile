CXX = c++
O = 2

server : server.o
	$(CXX) -o server server.o helpers.o

client : client.o 
	$(CXX) -o client client.o helpers.o 

client.o : client.cc helpers.hh 
	$(CXX) -c client.cc

server.o : server.cc helpers.hh 
	$(CXX) -c server.cc

helpers.o : helpers.cc helpers.hh
	$(CXX) -c helper.cc

