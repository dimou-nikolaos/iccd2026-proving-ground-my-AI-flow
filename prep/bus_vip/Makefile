# Self-test for the prep bus VIPs + SVA observers (run inside proving-ground/flow:public).
#
#   sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
#     -v $PWD/prep:/prep -w /prep/bus_vip --entrypoint bash proving-ground/flow:public -c "make"
#
# The same pattern is used for the challenge-2 submission tests; only RTL_ROOT and the
# assertion sources change there.
#
# Targets:
#   make            - run the VIP self-tests with protocol assertions bound and enabled
#   make negative   - run a deliberately non-compliant APB master; the build is expected
#                     to fail with an "APB4:" assertion message (proves the SVA is live)

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = vip_harness
COCOTB_TOPLEVEL = vip_harness
COCOTB_TEST_MODULES = test_vip_selftest
MODULE = test_vip_selftest

SVA_DIR := $(PWD)/../sva

VERILOG_SOURCES = $(PWD)/dut_loopback.sv \
                  $(SVA_DIR)/apb4_protocol_sva.sv \
                  $(SVA_DIR)/axi4lite_protocol_sva.sv \
                  $(SVA_DIR)/sva_bind_selftest.sv

# --assert enables SVA checking; without it Verilator silently drops the assertions.
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique --assert

include $(shell cocotb-config --makefiles)/Makefile.sim

# ---------------------------------------------------------------------------
# Negative test: a broken APB master that keeps PENABLE high for an extra ACCESS
# cycle after PREADY. A4 must fire. Success == make FAILS with an "APB4:" message.
# ---------------------------------------------------------------------------
NEG_TOP = apb_negative_harness

negative:
	@rm -rf sim_build results.xml /tmp/sva_negative.log
	@$(MAKE) -f Makefile results.xml \
	    TOPLEVEL=$(NEG_TOP) COCOTB_TOPLEVEL=$(NEG_TOP) \
	    COCOTB_TEST_MODULES=test_sva_negative MODULE=test_sva_negative \
	    VERILOG_SOURCES="$(PWD)/dut_loopback.sv $(SVA_DIR)/apb4_protocol_sva.sv $(SVA_DIR)/axi4lite_protocol_sva.sv $(PWD)/sva_negative.sv" \
	    EXTRA_ARGS="-Wno-fatal --x-assign unique --x-initial unique --assert" \
	    > /tmp/sva_negative.log 2>&1 || true
	@grep -q "Assertion failed.*APB4:" /tmp/sva_negative.log && \
	    echo "NEGATIVE TEST PASSED: assertion fired" || \
	    (echo "NEGATIVE TEST FAILED: assertion did not fire"; tail -20 /tmp/sva_negative.log; exit 1)
	@rm -rf sim_build results.xml
