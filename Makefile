dinnerparty: DinnerParty.o main.cpp
	g++ -o dinnerparty DinnerParty.o main.cpp

dinnerparty.o: DinnerParty.cpp DinnerParty.h
	g++ -c DinnerParty.cpp

clean:
	rm dinnerparty dinnerparty.o
