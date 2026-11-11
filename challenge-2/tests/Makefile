# Evidence test suite for dma_ctrl (cocotb + Verilator).
#
#   make                     run all tests on the RTL in ..
#   make RTL_ROOT=<dir>      run them on another tree that has rtl/filelist.f
#   make COCOTB_TEST_FILTER=<name>   run one test
#
# Requires Verilator 5 and cocotb 2 (both are in the flow image).

RTL_ROOT ?= ..
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = dma_ctrl
COCOTB_TOPLEVEL = dma_ctrl
COCOTB_TEST_MODULES = test_dma_reqs,test_bus_protocol,test_outstanding,test_cr_desc
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
SIM_BUILD ?= sim_build_tests_$(notdir $(RTL_ROOT_ABS))
export PYTHONPATH := $(PYTHONPATH):$(CURDIR)

include $(shell cocotb-config --makefiles)/Makefile.sim
