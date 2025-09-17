cc ?= gcc
cxx ?= g++

working_dir			:= ./build
build_dir := $(working_dir)/obj
bin_dir := $(working_dir)/bin

lvgl_dir := ./lv_port_pc_vscode/lvgl
lv_drivers := ./lv_port_pc_vscode/lv_drivers
LV_DRIVERS_PATH := ${lv_drivers}

inc := -I./lv_port_pc_vscode

ld_libs := -lSDL2 -lm

DEFINES := -D USE_SDL

BIN := $(bin_dir)/Simulator

cxx_srcs := $(shell find -L ./src -name "*.cpp")
cxx_objs := $(patsubst ./src/%.cpp,$(build_dir)/%.o,$(cxx_srcs))

lvgl_srcs := $(shell find -L ./lv_port_pc_vscode/lvgl/src -name "*.c")
lvgl_objs := $(patsubst ./%.c,$(build_dir)/%.o,$(lvgl_srcs))

include ./lv_port_pc_vscode/lv_drivers/lv_drivers.mk

CSRCS += $(lvgl_srcs)
CSRCS += ./src/mouse_cursor_icon.c

COBJS := $(patsubst ./%.c,$(build_dir)/%.o,$(CSRCS))

all: default

echo: 
	@echo 'COBJS : $(COBJS)'

$(build_dir)/%.o: ./src/%.cpp
	@echo 'Building project file: $<'
	@mkdir -p $(dir $@)
	@$(cxx) -std=c++11 $(inc) $(DEFINES) -c $< -o $@ 

$(build_dir)/%.o: %.c
	@echo 'Building project file: $<'
	@mkdir -p $(dir $@)
	@$(cc) $(inc) $(DEFINES) -c $< -o $@ 

default: $(cxx_objs) $(COBJS)
	@mkdir -p $(dir $(BIN))
	$(cxx) -o $(BIN) $(cxx_objs) $(COBJS) $(ld_libs)

clean:
	rm -r $(cxx_objs) $(COBJS) $(BIN)