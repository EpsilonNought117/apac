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
.globl addmul_one_arm64
.type  addmul_one_arm64, @function

addmul_one_arm64:
    // Swap val and size to match x86 register usage
    // x2 = val, x3 = size (after swap)
    mov     x4, x2          // x4 = size (temporary)
    mov     x2, x3          // x2 = val
    mov     x3, x4          // x3 = size
    
    // Initialize registers
    mov     x9, x2          // x9 = val (preserve)
    mov     x10, xzr        // x10 = carry accumulator = 0
    mov     x11, xzr        // x11 = additional carry from addition
    
    // Check if size is zero
    cbz     x3, .Larm64_end_of_func

.Larm64_main_loop:
    // Load op1 limb and multiply with val
    ldr     x12, [x1], #8   // x12 = op1[i]
    mul     x13, x12, x9    // x13 = low64 = op1[i] * val
    
    // Get high64 part (64x64->128 multiplication)
    umulh   x14, x12, x9    // x14 = high64 = (op1[i] * val) >> 64
    
    // Add low64 to carry accumulator
    adds    x13, x13, x10   // low64 += carry, set flags
    adc     x14, x14, xzr   // high64 += carry from low addition
    
    // Load current result limb and add
    ldr     x15, [x0]       // x15 = result[i]
    adds    x15, x15, x13   // result[i] += low64, set flags
    adc     x11, x11, xzr   // accumulate additional carry
    
    str     x15, [x0], #8   // Store result and increment pointer
    
    // Prepare for next iteration
    mov     x10, x14        // carry = high64
    // Add additional carry from result addition
    adds    x10, x10, x11   // carry += additional carry
    mov     x11, xzr        // Reset additional carry
    csinc   x11, xzr, xzr, cs  // Set x11 = 1 if carry set, else 0
    
    // Decrement size and loop
    subs    x3, x3, #1
    bne     .Larm64_main_loop

.Larm64_end_of_loop:
    // Handle final carry
    ldr     x12, [x0]       // Load final result limb
    adds    x12, x12, x10   // result[i] += carry
    str     x12, [x0]       // Store final result
    adc     x11, x11, xzr   // Capture final carry

.Larm64_end_of_func:
    // Return final carry (1 if carry, 0 otherwise)
    // x11 has carry flag (0 if no carry, 1 if carry)
    and     x0, x11, #1     // Extract LSB
    ret

.size addmul_one_arm64, .-addmul_one_arm64

.section .note.GNU-stack,"",@progbits