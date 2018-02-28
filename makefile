CPP=g++
CPP_FLAGS=-Wall -pedantic -std=c++11

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
	
%.o: $(LEX_PATH)/%.cpp $(HEADERS_FILES)
	$(CPP) $(CPP_FLAGS) -o $@

lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

al: $(OBJECT_FILES)
	$(CPP) $(CPP_FLAGS) $(OBJECT_FILES) -o $(EXECUTABLE)

cln_obj:
	rm $(OBJECT_FILES)

cln_exec:
	rm $(EXECUTABLE)
