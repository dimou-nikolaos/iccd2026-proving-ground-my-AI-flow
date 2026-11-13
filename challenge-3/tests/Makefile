# Submission test suite for rv_core (Phase 3a): directed + random ISS lockstep.
#
#   make                                  run on the fixed RTL (..)
#   make RTL_ROOT=../baseline             run on the pristine baseline
#   make RTL_ROOT=/design                 (flow invocation: cd /design/tests)
#   make ASSERTIONS=1                     add ../assertions/*.sv (bind files)
#   make COCOTB_TEST_FILTER=test_dev_d3   run one test
#
# RTL_ROOT only selects the RTL tree; the Python harness is always the vendored
# kit in this dir + ../tb/public (rv_tb/rv_asm).

RTL_ROOT ?= ..
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = rv_core
COCOTB_TOPLEVEL = rv_core
COCOTB_TEST_MODULES = test_rv_submission,test_rv_random

VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))

ASSERTIONS ?= 0
ifeq ($(ASSERTIONS),1)
VERILOG_SOURCES += $(wildcard $(abspath $(CURDIR)/../assertions)/*.sv)
endif

# wb_* pipeline-register probing (rv_core_env) needs --public-flat-rw.
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique --public-flat-rw --assert

SIM_BUILD ?= sim_build_$(notdir $(RTL_ROOT_ABS))

export PYTHONPATH := $(abspath .):$(abspath ../tb/public)

include $(shell cocotb-config --makefiles)/Makefile.sim
