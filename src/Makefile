CC = g++ -Wall -W -g

.SUFFIXES:	.o .cpp

.cpp.o:
	$(CC) -c -o $@ $<

all: ipd

ipd: strategy.o payoff.o evolution.o manager.o ipd.o
	$(CC) -o ../ipd strategy.o payoff.o evolution.o manager.o ipd.o
