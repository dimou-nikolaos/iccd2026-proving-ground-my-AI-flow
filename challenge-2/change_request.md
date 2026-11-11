# Change Request CR-D2: Descriptor-complete interrupt

Applies to: `dma_ctrl` specification revision 1.0. Design 2.

## Motivation

Software that runs long descriptor chains needs to track progress without polling. This change adds a per-channel interrupt event at the end of every descriptor.

## Change

1. Section 5, IRQ_STATUS: add field [11:8] DESC[n], W1C, reset value 0.
2. Section 5, IRQ_ENABLE: add field [11:8] DESC_IE[n], RW, reset value 0.
3. New requirement in section 9:
   - **REQ-36** DESC[n] is set once for every descriptor of channel n that completes (REQ-20), including the programmed block, a zero-length descriptor (REQ-18) and the last descriptor of a chain. It is set in the same cycle as DONE[n] when the descriptor is the last. It is not set by a descriptor stopped by an error (REQ-28) or an abort (REQ-31).
4. REQ-33, REQ-34 and REQ-35 apply to the new bits unchanged. Bits [11:8] lie in byte lane 1, so a W1C write or an IRQ_ENABLE write affects them only when `pstrb[1]` is 1.
5. No port changes. The identifier remains `0x444D0100`.

## Acceptance

- A chain of two descriptors sets DESC[n] when each completes. DONE[n] is set only at the end of the second.
- If the fetch of the next descriptor fails, DESC[n] is set for the descriptor that completed, and ERR[n] is set, with no DONE[n].
- `irq` follows (IRQ_STATUS AND IRQ_ENABLE) over bits [11:0].
