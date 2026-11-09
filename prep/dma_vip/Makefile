# DMA VIP self-test (prep item E) - run inside proving-ground/flow:public:
#
#   sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
#     -v $PWD/prep:/prep -w /prep/dma_vip --entrypoint bash proving-ground/flow:public -c "make"
#
# Targets:
#   make            - clean fixture: all evidence bodies pass, no assertion fires
#   make negative   - seeded-deviation matrix: every bug bit must be CAUGHT by the
#                     suite (all tests must PASS because each probe expects the
#                     deviation); the bound dma_irq_sva observer must also FIRE
#                     on the bug=32 run - proof the assertion chain is live

SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = dma_ctrl
COCOTB_TOPLEVEL = dma_ctrl
COCOTB_TEST_MODULES = test_dma
MODULE = test_dma

VIP_DIR := $(PWD)/../bus_vip
SVA_DIR := $(PWD)/../sva

export PYTHONPATH := $(VIP_DIR):$(PWD)/tests

VERILOG_SOURCES = $(PWD)/rtl/dma_ctrl.sv \
                  $(SVA_DIR)/apb4_protocol_sva.sv \
                  $(SVA_DIR)/axi4lite_protocol_sva.sv \
                  $(PWD)/tests/dma_assertions.sv \
                  $(PWD)/tests/dma_bind.sv

# --assert enables SVA checking; without it Verilator silently drops assertions
# (docs/skills/flow-internals.md; the flow itself adds --assert for evidence builds).
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique --assert

include $(shell cocotb-config --makefiles)/Makefile.sim

negative:
	@rm -rf sim_build results.xml
	@# stage 1: everything except the level-irq case (which aborts the sim via
	@# the bound dma_irq_sva, by design). All probes must catch; nothing fires.
	@DMA_NEG_SKIP=neg_irq_level $(MAKE) -f Makefile results.xml \
	    COCOTB_TEST_MODULES=test_dma_negative MODULE=test_dma_negative \
	    > /tmp/dma_neg1.log 2>&1 || true
	@echo "stage-1 failures (must be 0):" && (grep -c "<failure\|<error" results.xml || true)
	@grep -c "<failure\|<error" results.xml | { read n; [ "$$n" = "0" ] || \
	    (echo "$$n seeded deviations escaped the suite"; exit 1); }
	@rm -rf sim_build results.xml
	@# stage 2: level-irq only - the bound dma_irq_sva must fire and abort
	@DMA_NEG_ONLY=neg_irq_level $(MAKE) -f Makefile results.xml \
	    COCOTB_TEST_MODULES=test_dma_negative MODULE=test_dma_negative \
	    > /tmp/dma_neg2.log 2>&1 || true
	@grep -q "DMA-IRQ" /tmp/dma_neg2.log && \
	    echo "SVA CHAIN LIVE: dma_irq_sva fired on the level-irq build" || \
	    (echo "dma_irq_sva did NOT fire - assertion wiring broken"; tail -25 /tmp/dma_neg2.log; exit 1)
	@echo "negative matrix: all deviations caught, irq assertion live"
	@rm -rf sim_build results.xml

clean::
	rm -rf sim_build results.xml __pycache__ tests/__pycache__
