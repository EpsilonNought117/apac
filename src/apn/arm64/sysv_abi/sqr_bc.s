//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                        BASECASE SQUARING FUNCTIONS                        |
//   |                                                                           |
//   O---------------------------------------------------------------------------O
    //   Function Arguments
    //
    //   x0 -> result       (apn_seg_t*)
    //   x1 -> op1          (const apn_seg_t*)
    //   x2 -> size         (apn_size_t)

.text
.globl sqr_bc_arm64
.type  sqr_bc_arm64, @function

sqr_bc_arm64:
    // Save registers
    stp     x19, x20, [sp, #-64]!
    stp     x21, x22, [sp, #16]
    stp     x23, x24, [sp, #32]
    stp     x25, x26, [sp, #48]
    
    // Copy size to preserved register
    mov     x19, x2         // x19 = size
    mov     x20, x2         // x20 = copy of size
    sub     x20, x20, #1    // x20 = size - 1 (for triangular matrix)
    
    // Check if size == 1
    cbz     x20, .Larm64_pass3  // If size-1 == 0, skip to diagonal squares
    
    // PASS-1: Accumulate non-diagonal products once
    
    mov     x21, xzr        // x21 = counter (starts at 0)
    mov     x22, xzr        // x22 = previous carry accumulator

.Larm64_pass1_outer_loop:
    mov     x23, x20        // x23 = inner loop counter (size-1)
    mov     x24, xzr        // x24 = temp_reg
    ldr     x25, [x1, x21]  // x25 = op1[i] (base + offset)
    
    // Calculate pointers
    add     x26, x1, x21    // x26 = &op1[i]
    add     x26, x26, #8    // x26 = &op1[i+1]
    lsl     x3, x21, #1     // x3 = counter * 2 (limb offset)
    add     x4, x0, x3      // x4 = &result[2*i]
    add     x4, x4, #8      // x4 = &result[2*i+1]

.p2align 4
.Larm64_pass1_inner_loop:
    // Load op1[j] where j = i+1, i+2, ...
    ldr     x5, [x26], #8   // x5 = op1[j]
    
    // Multiply: op1[i] * op1[j]
    mul     x6, x5, x25     // x6 = low64
    umulh   x7, x5, x25     // x7 = high64
    
    // Add previous temp_reg to low64
    adds    x6, x6, x24     // low64 += temp_reg
    adc     x7, x7, xzr     // high64 += carry
    
    // Load current result and add
    ldr     x8, [x4]        // x8 = result[2*i + j - i] = result[i+j]
    adds    x8, x8, x6      // result += low64
    str     x8, [x4], #8    // Store and increment
    
    // Prepare for next iteration
    adc     x24, x7, xzr    // temp_reg = high64 + carry from addition
    
    // Decrement inner loop counter
    subs    x23, x23, #1
    bne     .Larm64_pass1_inner_loop

.Larm64_pass1_outer_loop_end:
    // Store final carry
    ldr     x8, [x4]        // Load next result limb
    adds    x8, x8, x24     // Add final temp_reg
    str     x8, [x4]        // Store back
    
    // Prepare for next outer iteration
    add     x21, x21, #8    // Increment counter by 1 limb (8 bytes)
    subs    x20, x20, #1    // Decrement outer loop counter
    bne     .Larm64_pass1_outer_loop
    
    // PASS-2: Shift left accumulated non-diagonal products by 1 bit
    
.Larm64_pass2:
    // We need to shift result[1] through result[2n-2] left by 1
    // Process two limbs at a time
    add     x4, x0, #8      // x4 = &result[1]
    mov     x20, x19        // x20 = size
    sub     x20, x20, #1    // x20 = size - 1 (number of pairs to process)
    
    // Clear carry flag to start
    mov     x21, xzr        // Use x21 as carry accumulator

.Larm64_pass2_loop:
    // Load two limbs
    ldp     x5, x6, [x4]    // x5 = result[i], x6 = result[i+1]
    
    // Shift left through carry
    extr    x5, x5, x5, #63  // x5 = (x5 << 1) | carry
    adc     x5, xzr, xzr    // Get new carry
    extr    x6, x6, x6, #63  // x6 = (x6 << 1) | carry
    adc     x6, xzr, xzr    // Get new carry
    
    // Store shifted limbs
    stp     x5, x6, [x4], #16
    
    // Decrement counter
    subs    x20, x20, #1
    bne     .Larm64_pass2_loop

    // Handle final carry if any
    // The final carry goes into result[2n-1]
    adc     x21, x21, xzr   // Capture final carry
    cbz     x21, .Larm64_pass2_end
    ldr     x5, [x4]        // Load result[2n-1]
    adds    x5, x5, x21     // Add carry
    str     x5, [x4]        // Store back

.Larm64_pass2_end:

    // PASS-3: Accumulate diagonal products (squares)
    
.Larm64_pass3:
    mov     x4, x0          // x4 = result pointer
    mov     x5, x1          // x5 = op1 pointer
    mov     x20, x19        // x20 = size (loop counter)
    mov     x21, xzr        // x21 = carry accumulator from previous iteration

.Larm64_pass3_loop:
    // Load op1[i]
    ldr     x22, [x5], #8   // x22 = op1[i]
    
    // Square: op1[i] * op1[i]
    mul     x23, x22, x22   // x23 = low64 of square
    umulh   x24, x22, x22   // x24 = high64 of square
    
    // Add previous carry to low64
    adds    x23, x23, x21   // low64 += carry
    adc     x24, x24, xzr   // high64 += carry from addition
    
    // Load result[2*i] and add low64
    ldr     x25, [x4]       // x25 = result[2*i]
    adds    x25, x25, x23   // result[2*i] += low64
    str     x25, [x4], #8   // Store and increment
    
    // Load result[2*i+1] and add high64 with carry
    ldr     x25, [x4]       // x25 = result[2*i+1]
    adcs    x25, x25, x24   // result[2*i+1] += high64 + carry
    str     x25, [x4], #8   // Store and increment
    
    // Prepare carry for next iteration
    adc     x21, xzr, xzr   // x21 = 1 if carry, else 0
    
    // Decrement loop counter
    subs    x20, x20, #1
    bne     .Larm64_pass3_loop

.Larm64_end_of_func:
    // Restore registers and return
    ldp     x25, x26, [sp, #48]
    ldp     x23, x24, [sp, #32]
    ldp     x21, x22, [sp, #16]
    ldp     x19, x20, [sp], #64
    ret

.size sqr_bc_arm64, .-sqr_bc_arm64

.section .note.GNU-stack,"",@progbits