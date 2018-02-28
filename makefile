CPP=g++
CXXFLAGS=-Wall -pedantic -std=c++11

LEX=flex

SOURCE_PATH=sources
LEX_PATH=$(SOURCE_PATH)/alpha_lex
DEST_PATH=destination

SOURCE_FILES=$(wildcard $(LEX_PATH)/*.cpp)
HEADERS_FILES=$(wildcard $(LEX_PATH)/*.h)
OBJECT_FILES=$(SOURCE_FILES:.cpp=.o)

EXECUTABLE=$(DEST_PATH)/al

all: directory lex al

clean: cln_obj cln_exec

directory:
	mkdir -p $(DEST_PATH)
	
alpha_token.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_token.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_token.cpp
	
alpha_token_generator.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_token_generator.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_token_generator.cpp
	
alpha_flex.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_flex.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_flex.cpp
	
lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

al: $(OBJECT_FILES)
	$(CPP) $(CXXFLAGS) $(OBJECT_FILES) -o $(EXECUTABLE)

cln_obj:
	rm $(OBJECT_FILES)

cln_exec:
	rm $(EXECUTABLE)
