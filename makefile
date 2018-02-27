CPP=g++
CPP_FLAGS=-Wall -pedantic -std=c++11

LEX=flex

SOURCE_PATH=sources
LEX_PATH=$(SOURCE_PATH)/alpha_lex
DEST_PATH=destination

SOURCES=$(wildcard $(LEX_PATH)/*.cpp)
HEADERS=$(wildcard $(LEX_PATH)/*.h)
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=$(DEST_PATH)/al

all: directory lex al

directory:
	mkdir -p $(DEST_PATH)
	
%.o: %.cpp $(HEADERS)
	$(CPP) $(CPP_FLAGS) -o $@

lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

al: $(OBJECTS)
	$(CPP) $(CPP_FLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm $(OBJECTS) $(EXECUTABLE)
