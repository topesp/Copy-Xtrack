cc ?= gcc
cxx ?= g++

BIN := Simulator

PROJECT_DIR := ${shell pwd}

WORKING_DIR			:= ./build
build_dir := $(WORKING_DIR)/build
obj_dir := $(WORKING_DIR)/obj
bin_dir := $(WORKING_DIR)/bin

inc := -I./lv_port_pc_vscode

ld_libs := -lSDL2 -lm

cxx_srcs := $(shell find -L $(PROJECT_DIR)/src -name "*.cpp")

cxx_objs := $(cxx_srcs:.cpp=.o)

all: default

echo: 
	@echo "root_path: $(PROJECT_DIR)"
	@echo "cxx_srcs: $(cxx_srcs)"
	@echo "cxx_objs: $(cxx_objs)"
	@echo "cxx_compile: $(cxx_compile)"
	@echo "build_dir: $(build_dir)"



%.o: %.cpp
	$(cxx_compile) -c $^ -o $@ 

default: $(cxx_objs)
	$(cxx) -o $(BIN) $(cxx_objs)

clean:
	rm $(cxx_objs) $(BIN)