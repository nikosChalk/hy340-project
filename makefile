CFLAGS=-Wall -pedantic -std=c++11
SOURCE_PATH=sources
DEST_PATH=destination

all: $DEST_PATH/al

al: alpha_token.o alpha_token_generator.o
	g++ $(CFLAGS) -o alpha_token.o alpha_token_generator.o al

alpha_token.o: alpha_token.cpp alpha_token.h
	g++ $(CFLAGS) -c alpha_token.cpp

alpha_token_generator.o: alpha_token_generator.cpp alpha_token_generator.h
	g++ $(CFLAGS) -c alpha_token_generator.cpp

lex: $SOURCE_PATH/alpha_flex.l
	flex< $<
 

clean:
	rm *o al