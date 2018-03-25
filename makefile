CPP=g++
CXXFLAGS=-std=c++11

LEX=flex

SRC_PATH=source
LEX_PATH=$(SRC_PATH)/alpha_lex
SYN_AN_PATH=$(SRC_PATH)/syntax_analyzer

BUILD_PATH=build
DIST_PATH=dist

EXECUTABLE=$(DIST_PATH)/acc

H_FILES = $(wildcard $(SRC_PATH)/*.h) $(wildcard $(LEX_PATH)/*.h) $(wildcard $(SYN_AN_PATH)/*.h) $(SYN_AN_PATH)/alpha_bison.hpp
SRC_FILES = $(wildcard $(SRC_PATH)/*.cpp) $(wildcard $(LEX_PATH)/*.cpp) $(wildcard $(SYN_AN_PATH)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_PATH)/%.cpp,$(BUILD_PATH)/%.o,$(SRC_FILES)) $(patsubst $(LEX_PATH)/%.cpp,$(BUILD_PATH)/%.o,$(SRC_FILES)) $(patsubst $(SYN_AN_PATH)/%.cpp,$(BUILD_PATH)/%.o,$(SRC_FILES))

all: directory lex bison acc

directory:
	mkdir -p $(BUILD_PATH); \
	mkdir -p $(DIST_PATH);

$(BUILD_PATH)/alpha_flex.o: $(LEX_PATH)/alpha_flex.cpp $(H_FILES) 
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/alpha_bison.o: $(SYN_AN_PATH)/alpha_bison.cpp $(H_FILES) 
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/parser_manager.o: $(SYN_AN_PATH)/parser_manager.cpp $(H_FILES) 
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/symbol_table.o: $(SYN_AN_PATH)/symbol_table.cpp $(H_FILES) 
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/syntax_error.o: $(SYN_AN_PATH)/syntax_error.cpp $(H_FILES) 
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/main.o: $(SRC_PATH)/main.cpp $(H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@
	
$(BUILD_PATH)/not_implemented_error.o: $(SRC_PATH)/not_implemented_error.cpp $(H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_PATH)/utilities.o: $(SRC_PATH)/utilities.cpp $(H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@
	
lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

bison: $(SYN_AN_PATH)/parser.y
	cd $(SYN_AN_PATH) && \
	bison parser.y && \
	cd -

VPATH = $(BUILD_PATH)
acc: alpha_flex.o alpha_bison.o parser_manager.o symbol_table.o syntax_error.o main.o not_implemented_error.o utilities.o
	$(CPP) $(CXXFLAGS) $^ -o $(EXECUTABLE)

clean:
	rm -rf $(BUILD_PATH); \
	rm -rf $(DIST_PATH); \



