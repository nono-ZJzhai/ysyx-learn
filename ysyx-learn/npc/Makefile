VERILATOR_FLAGS=--cc --exe --build --trace
SRC_DIR=./csrc
VSR_DIR=./vsrc

SRC=$(wildcard $(SRC_DIR)/*.cpp)
VSR=$(wildcard $(VSR_DIR)/*.v)

all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	verilator $(VERILATOR_FLAGS) $(SRC) $(VSR)

clean:
	rm -rf obj_dir
include ../Makefile
