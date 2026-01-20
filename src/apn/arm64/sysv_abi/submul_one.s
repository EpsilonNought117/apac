//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                 APN-ARR BY ONE LIMB MULTIPLICATION FUNCTIONS              |
//   |                                                                           |
//   O---------------------------------------------------------------------------O

    //   Function Arguments
    //
    //   x0 -> result       (apn_seg_t*)
    //   x1 -> op1          (const apn_seg_t*)
    //   x2 -> size         (apn_size_t)
    //   x3 -> val          (apn_seg_t)

.text
.globl submul_one_arm64
.type  submul_one_arm64, @function

submul_one_arm64:
    // Swap val and size to match x86 register usage
    // x2 = val, x3 = size (after swap)
    mov     x4, x2          // x4 = size (temporary)
    mov     x2, x3          // x2 = val
    mov     x3, x4          // x3 = size
    
    // Initialize registers
    mov     x9, x2          // x9 = val (preserve)
    mov     x10, xzr        // x10 = temp_reg (like r10) = 0
    mov     x11, xzr        // x11 = borrow accumulator
    
    // Check if size is zero
    cbz     x3, .Larm64_final_borrow

.Larm64_main_loop:
    // Load op1 limb and multiply with val
    ldr     x12, [x1], #8   // x12 = op1[i]
    mul     x13, x12, x9    // x13 = low64 = op1[i] * val
    
    // Get high64 part (64x64->128 multiplication)
    // We need umulh for high part of unsigned multiply
    umulh   x14, x12, x9    // x14 = high64 = (op1[i] * val) >> 64
    
    // Add low64 to temp_reg (with carry from previous)
    adds    x13, x13, x10   // low64 += temp_reg, set flags
    adc     x14, x14, xzr   // high64 += carry
    
    // Load current result limb and subtract
    ldr     x15, [x0]       // x15 = result[i]
    subs    x15, x15, x13   // result[i] -= low64
    sbc     x11, x11, xzr   // accumulate borrow (0 if no borrow, -1 if borrow)
    
    str     x15, [x0], #8   // Store result and increment pointer
    
    // Prepare for next iteration
    mov     x10, x14        // temp_reg = high64
    // Add borrow to temp_reg
    adds    x10, x10, x11   // temp_reg += borrow (adds sets flags)
    mov     x11, xzr        // Reset borrow accumulator (negated by next sbc)
    csinc   x11, xzr, xzr, cc  // Set x11 = 1 if borrow (C=0), else 0
    
    // Decrement size and loop
    subs    x3, x3, #1
    bne     .Larm64_main_loop

.Larm64_loop_end:
    // Handle final borrow with remaining temp_reg
    ldr     x12, [x0]       // Load final result limb
    subs    x12, x12, x10   // result[i] -= temp_reg
    str     x12, [x0]       // Store final result
    sbc     x11, x11, xzr   // Final borrow accumulation

.Larm64_final_borrow:
    // Return final borrow (1 if borrow, 0 otherwise)
    // x11 has borrow flag (0 if no borrow, 1 if borrow)
    and     x0, x11, #1     // Extract LSB
    ret

.size submul_one_arm64, .-submul_one_arm64

.section .note.GNU-stack,"",@progbits