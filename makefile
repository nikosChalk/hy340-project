CPP=g++
CXXFLAGS=-std=c++11

SRC_DIR=source
BUILD_DIR=build
DIST_DIR=dist

CORE_DIR=.
CORE_H=$(wildcard $(SRC_DIR)/$(CORE_DIR)/*.h)
CORE_SRC=$(wildcard $(SRC_DIR)/$(CORE_DIR)/*.cpp)
CORE_OBJ_FILES=$(patsubst $(SRC_DIR)/$(CORE_DIR)/%.cpp,$(BUILD_DIR)/$(CORE_DIR)/%.o,$(CORE_SRC))	#$(patsubst pattern,replacement,text_to_search)

ALPHA_LEX_DIR=alpha_lex
ALPHA_LEX_H=$(wildcard $(SRC_DIR)/$(ALPHA_LEX_DIR)/*.h)
ALPHA_LEX_SRC=$(wildcard $(SRC_DIR)/$(ALPHA_LEX_DIR)/*.cpp)
ALPHA_LEX_OBJ_FILES=$(patsubst $(SRC_DIR)/$(ALPHA_LEX_DIR)/%.cpp,$(BUILD_DIR)/$(ALPHA_LEX_DIR)/%.o,$(ALPHA_LEX_SRC))

SYNTAX_ANALYZER_DIR=syntax_analyzer
SYNTAX_ANALYZER_H=$(wildcard $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/*.h)
SYNTAX_ANALYZER_SRC=$(wildcard $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/*.cpp)
SYNTAX_ANALYZER_OBJ_FILES=$(patsubst $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/%.cpp,$(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR)/%.o,$(SYNTAX_ANALYZER_SRC))

ICODE_DIR=intermediate_code
ICODE_H=$(wildcard $(SRC_DIR)/$(ICODE_DIR)/*.h)
ICODE_SRC=$(wildcard $(SRC_DIR)/$(ICODE_DIR)/*.cpp)
ICODE_OBJ_FILES=$(patsubst $(SRC_DIR)/$(ICODE_DIR)/%.cpp,$(BUILD_DIR)/$(ICODE_DIR)/%.o,$(ICODE_SRC))

ALL_H_FILES = $(CORE_H) $(ALPHA_LEX_H) $(SYNTAX_ANALYZER_H) $(ICODE_H)

EXEC=$(DIST_DIR)/acc

all: dirs lex bison acc

dirs:
	mkdir -p $(BUILD_DIR)/$(CORE_DIR); \
	mkdir -p $(BUILD_DIR)/$(ALPHA_LEX_DIR); \
	mkdir -p $(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR); \
	mkdir -p $(BUILD_DIR)/$(ICODE_DIR); \
	mkdir -p $(DIST_DIR);

lex: $(SRC_DIR)/$(ALPHA_LEX_DIR)/alpha_flex.l
	cd $(SRC_DIR)/$(ALPHA_LEX_DIR) && \
	flex alpha_flex.l && \
	cd -

bison: $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/parser.y
	cd $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/ && \
	bison parser.y && \
	cd -

$(BUILD_DIR)/$(CORE_DIR)/%.o: $(SRC_DIR)/$(CORE_DIR)/%.cpp $(ALL_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(ALPHA_LEX_DIR)/%.o: $(SRC_DIR)/$(ALPHA_LEX_DIR)/%.cpp $(ALL_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR)/%.o: $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/%.cpp $(ALL_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(ICODE_DIR)/%.o: $(SRC_DIR)/$(ICODE_DIR)/%.cpp $(ALL_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

acc: $(CORE_OBJ_FILES) $(ALPHA_LEX_OBJ_FILES) $(SYNTAX_ANALYZER_OBJ_FILES) $(ICODE_OBJ_FILES)
	$(CPP) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -rf $(BUILD_DIR); \
	rm -rf $(DIST_DIR); \


.PHONY: debug_info
debug_info:
	echo "CORE_H:					$(CORE_H)"; \
	echo "ALPHA_LEX_H:				$(ALPHA_LEX_H)"; \
	echo "SYNTAX_ANALYZER_H:		$(SYNTAX_ANALYZER_H)"; \
	echo "ICODE_H:					$(ICODE_H)"; \
	echo "CORE_SRC:					$(CORE_SRC)"; \
	echo "ALPHA_LEX_SRC:			$(ALPHA_LEX_SRC)"; \
	echo "SYNTAX_ANALYZER_SRC:		$(SYNTAX_ANALYZER_SRC)"; \
	echo "ICODE_SRC:				$(ICODE_SRC)"; \
	echo "CORE_OBJ_FILES:			$(CORE_OBJ_FILES)"; \
	echo "ALPHA_LEX_OBJ_FILES:		$(ALPHA_LEX_OBJ_FILES)"; \
	echo "SYNTAX_ANALYZER_OBJ_FILES:$(SYNTAX_ANALYZER_OBJ_FILES)"; \
	echo "ICODE_OBJ_FILES:			$(ICODE_OBJ_FILES)"; \



