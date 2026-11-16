# Probe-per-REQ sweep for rv_core (Phase 1c). Same harness as tb/public.
#
#   make                                  run all probes on the fixed RTL (../)
#   make RTL_ROOT=../baseline             run them on the pristine baseline
#   make COCOTB_TEST_FILTER=test_dev_d3   run one test
#
# RTL_ROOT only selects the RTL tree; the Python harness is always ../tb/public.

RTL_ROOT ?= ..
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = rv_core
COCOTB_TOPLEVEL = rv_core
COCOTB_TEST_MODULES = test_probes
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
SIM_BUILD ?= sim_build_$(notdir $(RTL_ROOT_ABS))

export PYTHONPATH := $(abspath ../tb/public):$(abspath .)

include $(shell cocotb-config --makefiles)/Makefile.sim
