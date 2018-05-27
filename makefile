CPP=g++
CXXFLAGS=-std=c++11 -g

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

COMM_INTR_DIR=common_interface
COMM_INTR_H=$(wildcard $(SRC_DIR)/$(COMM_INTR_DIR)/*.h)
COMM_INTR_SRC=$(wildcard $(SRC_DIR)/$(COMM_INTR_DIR)/*.cpp)
COMM_INTR_OBJ_FILES=$(patsubst $(SRC_DIR)/$(COMM_INTR_DIR)/%.cpp,$(BUILD_DIR)/$(COMM_INTR_DIR)/%.o,$(COMM_INTR_SRC))

COMM_INTR_ERR_DIR=$(COMM_INTR_DIR)/errors
COMM_INTR_ERR_H=$(wildcard $(SRC_DIR)/$(COMM_INTR_ERR_DIR)/*.h)
COMM_INTR_ERR_SRC=$(wildcard $(SRC_DIR)/$(COMM_INTR_ERR_DIR)/*.cpp)
COMM_INTR_ERR_OBJ_FILES=$(patsubst $(SRC_DIR)/$(COMM_INTR_ERR_DIR)/%.cpp,$(BUILD_DIR)/$(COMM_INTR_ERR_DIR)/%.o,$(COMM_INTR_ERR_SRC))

ICODE_DIR=intermediate_code
ICODE_H=$(wildcard $(SRC_DIR)/$(ICODE_DIR)/*.h)
ICODE_SRC=$(wildcard $(SRC_DIR)/$(ICODE_DIR)/*.cpp)
ICODE_OBJ_FILES=$(patsubst $(SRC_DIR)/$(ICODE_DIR)/%.cpp,$(BUILD_DIR)/$(ICODE_DIR)/%.o,$(ICODE_SRC))

SYNTAX_ANALYZER_DIR=syntax_analyzer
SYNTAX_ANALYZER_H=$(wildcard $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/*.h)
SYNTAX_ANALYZER_SRC=$(wildcard $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/*.cpp)
SYNTAX_ANALYZER_OBJ_FILES=$(patsubst $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/%.cpp,$(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR)/%.o,$(SYNTAX_ANALYZER_SRC))

TCODE_DIR=target_code
TCODE_H=$(wildcard $(SRC_DIR)/$(TCODE_DIR)/*.h)
TCODE_SRC=$(wildcard $(SRC_DIR)/$(TCODE_DIR)/*.cpp)
TCODE_OBJ_FILES=$(patsubst $(SRC_DIR)/$(TCODE_DIR)/%.cpp,$(BUILD_DIR)/$(TCODE_DIR)/%.o,$(TCODE_SRC))

TCODE_GFUNC_DIR=$(TCODE_DIR)/generate_funcs
TCODE_GFUNC_H=$(wildcard $(SRC_DIR)/$(TCODE_GFUNC_DIR)/*.h)
TCODE_GFUNC_SRC=$(wildcard $(SRC_DIR)/$(TCODE_GFUNC_DIR)/*.cpp)
TCODE_GFUNC_OBJ_FILES=$(patsubst $(SRC_DIR)/$(TCODE_GFUNC_DIR)/%.cpp,$(BUILD_DIR)/$(TCODE_GFUNC_DIR)/%.o,$(TCODE_GFUNC_SRC))

VM_DIR=virtual_machine
VM_H=$(wildcard $(SRC_DIR)/$(VM_DIR)/*.h)
VM_SRC=$(wildcard $(SRC_DIR)/$(VM_DIR)/*.cpp)
VM_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_DIR)/%.cpp,$(BUILD_DIR)/$(VM_DIR)/%.o,$(VM_SRC))

VM_AVM_DIR=$(VM_DIR)/AVM
VM_AVM_H=$(wildcard $(SRC_DIR)/$(VM_AVM_DIR)/*.h)
VM_AVM_SRC=$(wildcard $(SRC_DIR)/$(VM_AVM_DIR)/*.cpp)
VM_AVM_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_AVM_DIR)/%.cpp,$(BUILD_DIR)/$(VM_AVM_DIR)/%.o,$(VM_AVM_SRC))

VM_AVM_BUILT_IN_FUNCS_DIR=$(VM_AVM_DIR)/builtin_funcs
VM_AVM_BUILT_IN_FUNCS_H=$(wildcard $(SRC_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/*.h)
VM_AVM_BUILT_IN_FUNCS_SRC=$(wildcard $(SRC_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/*.cpp)
VM_AVM_BUILT_IN_FUNCS_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/%.cpp,$(BUILD_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/%.o,$(VM_AVM_BUILT_IN_FUNCS_SRC))

VM_AVM_DISPATCHER_DIR=$(VM_AVM_DIR)/dispatcher
VM_AVM_DISPATCHER_H=$(wildcard $(SRC_DIR)/$(VM_AVM_DISPATCHER_DIR)/*.h)
VM_AVM_DISPATCHER_SRC=$(wildcard $(SRC_DIR)/$(VM_AVM_DISPATCHER_DIR)/*.cpp)
VM_AVM_DISPATCHER_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_AVM_DISPATCHER_DIR)/%.cpp,$(BUILD_DIR)/$(VM_AVM_DISPATCHER_DIR)/%.o,$(VM_AVM_DISPATCHER_SRC))

VM_AVM_ERRS_DIR=$(VM_AVM_DIR)/errors
VM_AVM_ERRS_H=$(wildcard $(SRC_DIR)/$(VM_AVM_ERRS_DIR)/*.h)
VM_AVM_ERRS_SRC=$(wildcard $(SRC_DIR)/$(VM_AVM_ERRS_DIR)/*.cpp)
VM_AVM_ERRS_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_AVM_ERRS_DIR)/%.cpp,$(BUILD_DIR)/$(VM_AVM_ERRS_DIR)/%.o,$(VM_AVM_ERRS_SRC))

VM_AVM_PSTACK_DIR=$(VM_AVM_DIR)/Program_stack
VM_AVM_PSTACK_H=$(wildcard $(SRC_DIR)/$(VM_AVM_PSTACK_DIR)/*.h)
VM_AVM_PSTACK_SRC=$(wildcard $(SRC_DIR)/$(VM_AVM_PSTACK_DIR)/*.cpp)
VM_AVM_PSTACK_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_AVM_PSTACK_DIR)/%.cpp,$(BUILD_DIR)/$(VM_AVM_PSTACK_DIR)/%.o,$(VM_AVM_PSTACK_SRC))

VM_RDP_DIR=$(VM_DIR)/RDP
VM_RDP_H=$(wildcard $(SRC_DIR)/$(VM_RDP_DIR)/*.h)
VM_RDP_SRC=$(wildcard $(SRC_DIR)/$(VM_RDP_DIR)/*.cpp)
VM_RDP_OBJ_FILES=$(patsubst $(SRC_DIR)/$(VM_RDP_DIR)/%.cpp,$(BUILD_DIR)/$(VM_RDP_DIR)/%.o,$(VM_RDP_SRC))

COMMON_H_FILES = $(COMM_INTR_H) $(COMM_INTR_ERR_H)
ACC_H_FILES = $(CORE_H) $(ALPHA_LEX_H) $(SYNTAX_ANALYZER_H) $(ICODE_H) $(TCODE_H) $(TCODE_GFUNC_H) $(COMMON_H_FILES)
AVM_H_FILES = $(COMMON_H_FILES) $(VM_H) $(VM_AVM_H) $(VM_AVM_BUILT_IN_FUNCS_H) $(VM_AVM_DISPATCHER_H) $(VM_AVM_ERRS_H) $(VM_AVM_PSTACK_H) $(VM_RDP_H)

EXEC_ACC=$(DIST_DIR)/acc
EXEC_AVM=$(DIST_DIR)/avm

all: dirs lex bison acc avm

dirs:
	mkdir -p $(BUILD_DIR)/$(CORE_DIR); \
	mkdir -p $(BUILD_DIR)/$(ALPHA_LEX_DIR); \
	mkdir -p $(BUILD_DIR)/$(COMM_INTR_DIR); \
	mkdir -p $(BUILD_DIR)/$(COMM_INTR_ERR_DIR); \
	mkdir -p $(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR); \
	mkdir -p $(BUILD_DIR)/$(ICODE_DIR); \
	mkdir -p $(BUILD_DIR)/$(TCODE_DIR); \
	mkdir -p $(BUILD_DIR)/$(TCODE_GFUNC_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_AVM_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_AVM_DISPATCHER_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_AVM_ERRS_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_AVM_PSTACK_DIR); \
	mkdir -p $(BUILD_DIR)/$(VM_RDP_DIR); \
	mkdir -p $(DIST_DIR);

lex: $(SRC_DIR)/$(ALPHA_LEX_DIR)/alpha_flex.l
	cd $(SRC_DIR)/$(ALPHA_LEX_DIR) && \
	flex alpha_flex.l && \
	cd -

bison: $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/parser.y
	cd $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/ && \
	bison parser.y && \
	cd -

$(BUILD_DIR)/$(CORE_DIR)/%.o: $(SRC_DIR)/$(CORE_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(ALPHA_LEX_DIR)/%.o: $(SRC_DIR)/$(ALPHA_LEX_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(COMM_INTR_DIR)/%.o: $(SRC_DIR)/$(COMM_INTR_DIR)/%.cpp $(COMMON_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(COMM_INTR_ERR_DIR)/%.o: $(SRC_DIR)/$(COMM_INTR_ERR_DIR)/%.cpp $(COMMON_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(SYNTAX_ANALYZER_DIR)/%.o: $(SRC_DIR)/$(SYNTAX_ANALYZER_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(ICODE_DIR)/%.o: $(SRC_DIR)/$(ICODE_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(TCODE_DIR)/%.o: $(SRC_DIR)/$(TCODE_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(TCODE_GFUNC_DIR)/%.o: $(SRC_DIR)/$(TCODE_GFUNC_DIR)/%.cpp $(ACC_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@



$(BUILD_DIR)/$(VM_DIR)/%.o: $(SRC_DIR)/$(VM_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_AVM_DIR)/%.o: $(SRC_DIR)/$(VM_AVM_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/%.o: $(SRC_DIR)/$(VM_AVM_BUILT_IN_FUNCS_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_AVM_DISPATCHER_DIR)/%.o: $(SRC_DIR)/$(VM_AVM_DISPATCHER_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_AVM_ERRS_DIR)/%.o: $(SRC_DIR)/$(VM_AVM_ERRS_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_AVM_PSTACK_DIR)/%.o: $(SRC_DIR)/$(VM_AVM_PSTACK_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(VM_RDP_DIR)/%.o: $(SRC_DIR)/$(VM_RDP_DIR)/%.cpp $(AVM_H_FILES)
	$(CPP) -c $(CXXFLAGS) $< -o $@



acc: $(COMM_INTR_OBJ_FILES) $(COMM_INTR_ERR_OBJ_FILES) $(CORE_OBJ_FILES) $(ALPHA_LEX_OBJ_FILES) $(SYNTAX_ANALYZER_OBJ_FILES) $(ICODE_OBJ_FILES) $(TCODE_OBJ_FILES) $(TCODE_GFUNC_OBJ_FILES)
	$(CPP) $(CXXFLAGS) $^ -o $(EXEC_ACC)

avm: $(COMM_INTR_OBJ_FILES) $(COMM_INTR_ERR_OBJ_FILES) $(VM_OBJ_FILES) $(VM_AVM_OBJ_FILES) $(VM_AVM_BUILT_IN_FUNCS_OBJ_FILES) $(VM_AVM_DISPATCHER_OBJ_FILES) $(VM_AVM_ERRS_OBJ_FILES) $(VM_AVM_PSTACK_OBJ_FILES) $(VM_RDP_OBJ_FILES)
	$(CPP) $(CXXFLAGS) $^ -o $(EXEC_AVM)

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



