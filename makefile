CPP=g++
CXXFLAGS=-std=c++11

LEX=flex

SRC_PATH=source
LEX_PATH=$(SOURCE_PATH)/alpha_lex
SYN_AN_PATH=$(SRC_PATH)/syntax_analyzer

BUILD_PATH=build
DIST_PATH=dist

EXECUTABLE=$(DIST_PATH)/acc

H_FILES = $(wildcard $(SRC_PATH)/*.h) $(wildcard $(LEX_PATH)/*.h) $(wildcard $(SYN_AN_PATH)/*.h) $(SYN_AN_PATH)/alpha_bison.hpp
SRC_FILES = $(wildcard $(SRC_PATH)/*.cpp) $(wildcard $(LEX_PATH)/*.cpp) $(wildcard $(SYN_AN_PATH)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)) $(patsubst $(LEX_PATH)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)) $(patsubst $(SYN_AN_PATH)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: directory lex bison acc

clean: cln_obj cln_exec

directory:
	mkdir -p $(DEST_PATH)

VPATH = $(SRC_PATH) $(LEX_PATH) $(SYN_AN_PATH)
$(BUILD_DIR)/%.o: %.cpp $(H_FILES)
	$(CC) -c $(CFLAGS) $< -o $@
	
lex: $(LEX_PATH)/alpha_flex.l
	cd $(LEX_PATH) && \
	flex alpha_flex.l && \
	cd -

bison: $(SYN_AN_PATH)/parser.y
	cd $(SYN_AN_PATH) && \
	bison parser.y && \
	cd -

acc: $(OBJ_FILES)
	$(CPP) $(CXXFLAGS) $^ -o $(EXECUTABLE)

clean:
	rm -rf $(BUILD_PATH); \
	rm -rf $(DIST_PATH); \



