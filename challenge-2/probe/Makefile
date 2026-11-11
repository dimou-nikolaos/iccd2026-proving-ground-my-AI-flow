# Probe runs: make RTL_ROOT=<tree-with-rtl/filelist.f> [COCOTB_TEST_FILTER=p1_...]
RTL_ROOT ?= /design
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))
SIM ?= verilator
TOPLEVEL_LANG = verilog
COCOTB_TOPLEVEL = dma_ctrl
COCOTB_TEST_MODULES = test_probe
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
SIM_BUILD ?= sim_build_$(notdir $(RTL_ROOT_ABS))
export PYTHONPATH := $(PYTHONPATH):$(CURDIR)
include $(shell cocotb-config --makefiles)/Makefile.sim
