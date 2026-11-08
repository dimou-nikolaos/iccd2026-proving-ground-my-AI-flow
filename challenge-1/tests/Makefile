# Submission testbench for dsp_stream (cocotb + Verilator).
#
#   make RTL_ROOT=/design            run all tests on fixed RTL
#   make RTL_ROOT=/design/baseline   run against baseline (expect failures)
#   make ASSERTIONS=1                include assertion files
#   make COCOTB_TEST_FILTER=name     run one test

RTL_ROOT ?= ..
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = dsp_stream
COCOTB_TOPLEVEL = dsp_stream
COCOTB_TEST_MODULES = test_submission
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
ASSERTIONS ?= 0
ifeq ($(ASSERTIONS),1)
VERILOG_SOURCES += $(wildcard $(abspath ..)/assertions/*.sv)
EXTRA_ARGS += --assert
endif
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
SIM_BUILD ?= sim_build_$(notdir $(RTL_ROOT_ABS))

include $(shell cocotb-config --makefiles)/Makefile.sim
