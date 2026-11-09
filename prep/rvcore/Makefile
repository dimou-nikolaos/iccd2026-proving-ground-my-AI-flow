# Self-test for the RV32 stage-contract SVA + retire-stream lockstep.
#
# Run inside the flow image:
#   sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
#     -v $PWD/prep:/prep -w /prep/rvcore --entrypoint bash \
#     proving-ground/flow:public -c "make"
#
# Targets:
#   make            positive: every stage value is checked against the ISA reference
#                   on every cycle AND every retire matches the host ISS.
#   make negative   nine deliberately broken harness builds; each must trip at
#                   least one assertion (proves the observer is live, not dropped).
#   make clean

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = stage_harness
COCOTB_TOPLEVEL = stage_harness
COCOTB_TEST_MODULES ?= test_stage
MODULE ?= test_stage

RVISS := $(PWD)/../rviss
export PYTHONPATH := $(RVISS):$(PWD):$(PYTHONPATH)

VERILOG_SOURCES = $(PWD)/rv_ref_pkg.sv \
                  $(PWD)/rv_stage_sva.sv \
                  $(PWD)/stage_harness.sv \
                  $(PWD)/rv_stage_bind_selftest.sv

EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique --assert

include $(shell cocotb-config --makefiles)/Makefile.sim

LOG := /tmp/rvcore_$(shell date +%s%N).log

# ---------------------------------------------------------------------------
# The nine injected deviations, one per assertion family. Each is a mutation an
# operator class could produce; the point is that the *observer* catches it.
# ---------------------------------------------------------------------------
BUGS := 1 2 3 4 5 6 7 8 10
BUG_NAMES := alu_add_is_sub dec_imm_zeroext branch_sltu_signed crd3_min_is_max \
             wb_writes_x0 load_lh_zeroext fetch_pc_skips decode_rd_slice sub_is_add

negative:
	@set -e; rm -rf sim_build results.xml; \
	i=0; \
	for bug in $(BUGS); do \
	  i=$$((i+1)); name=$$(echo $(BUG_NAMES) | cut -d' ' -f$$i); \
	  echo "--- BUG=$$bug ($$name)"; \
	  rm -rf sim_build results.xml; \
	  $(MAKE) -f Makefile results.xml \
	      COCOTB_TEST_MODULES=test_negative MODULE=test_negative \
	      EXTRA_ARGS="-Wno-fatal --x-assign unique --x-initial unique --assert -GBUG=$$bug" \
	      > $(LOG) 2>&1 || true; \
	  if grep -q "Assertion failed" $(LOG); then \
	    echo "    PASS: observer fired"; \
	  else \
	    echo "    FAIL: no assertion for BUG=$$bug ($$name)"; \
	    tail -25 $(LOG); exit 1; \
	  fi; \
	done; \
	rm -rf sim_build results.xml; echo "ALL NEGATIVE CASES PASSED"

clean_rm:
	rm -rf sim_build results.xml __pycache__

# ---------------------------------------------------------------------------
# Diagnostic: why does a retired value diverge? Run with RVSEED=<seed> to
# reproduce one program from randprog; prints ISS vs DUT operands at the first
# commit whose writeback value differs.
# ---------------------------------------------------------------------------
dbg:
	@rm -rf sim_build results.xml
	@RVSEED=$${RVSEED:-3} $(MAKE) -f Makefile results.xml \
	    COCOTB_TEST_MODULES=dbg_operand_diff MODULE=dbg_operand_diff 2>&1 | \
	    grep -E "MISMATCH|ISS  |DUT  |DESYNC|no mismatch" || true

.PHONY: negative dbg clean_rm
