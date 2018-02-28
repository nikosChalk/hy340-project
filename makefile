CPP=g++
CXXFLAGS=-std=c++11

LEX=flex

SOURCE_PATH=sources
LEX_PATH=$(SOURCE_PATH)/alpha_lex
DEST_PATH=destination

SOURCE_FILES=$(wildcard $(LEX_PATH)/*.cpp)
HEADERS_FILES=$(wildcard $(LEX_PATH)/*.h)
OBJECT_FILES=$(SOURCE_FILES:.cpp=.o)

EXECUTABLE=$(DEST_PATH)/al
EXECUTABLE_BONUS=$(DEST_PATH)/al_bonus

all: directory lex al al_bonus

clean: cln_obj cln_exec

directory:
	mkdir -p $(DEST_PATH)
	
alpha_token.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_token.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_token.cpp
	
alpha_token_generator.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_token_generator.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_token_generator.cpp
	
alpha_flex.o: $(HEADERS_FILES) $(LEX_PATH)/alpha_flex.cpp
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/alpha_flex.cpp
	
analyzer.o: $(LEX_PATH)/bonus/analyzer.h $(LEX_PATH)/bonus/main.cpp $(SOURCE_PATH)/utilities.h $(HEADERS_FILES)
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/bonus/analyzer.cpp
	
DFA.o: $(LEX_PATH)/bonus/DFA.h $(LEX_PATH)/bonus/DFA.cpp $(SOURCE_PATH)/utilities.h $(HEADERS_FILES)
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/bonus/DFA.cpp

main.o: $(LEX_PATH)/bonus/analyzer.h $(LEX_PATH)/bonus/main.cpp $(HEADERS_FILES)
	$(CPP) -c $(CXXFLAGS) $(LEX_PATH)/bonus/main.cpp
	
utilities.o: $(SOURCE_PATH)/utilities.h $(SOURCE_PATH)/utilities.cpp
	$(CPP) -c $(CXXFLAGS) $(SOURCE_PATH)/utilities.cpp
	
lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

al: $(OBJECT_FILES) $(SOURCE_PATH)/utilities.o
	$(CPP) $(CXXFLAGS) $^ -o $(EXECUTABLE)
	
al_bonus: $(LEX_PATH)/bonus/analyzer.o $(LEX_PATH)/bonus/DFA.o $(LEX_PATH)/bonus/main.o $(SOURCE_PATH)/utilities.o $(LEX_PATH)/alpha_token_generator.o $(LEX_PATH)/alpha_token.o
	$(CPP) $(CXXFLAGS) $^ -o $(EXECUTABLE_BONUS)

cln_obj:
	rm $(OBJECT_FILES) $(LEX_PATH)/bonus/*.o $(SOURCE_PATH)/*.o

cln_exec:
	rm $(EXECUTABLE) $(EXECUTABLE_BONUS)
