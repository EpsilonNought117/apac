//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                      BASECASE MULTIPLICATION FUNCTIONS                    |
//   |                                                                           |
//   O---------------------------------------------------------------------------O

    //   Function Arguments
    //
    //   x0 -> result           (apn_seg_t*)
    //   x1 -> op1              (const apn_seg_t*)
    //   x2 -> op2              (const apn_seg_t*)
    //   x3 -> size1            (apn_size_t)
    //   x4 -> size2            (apn_size_t)

    //   ASSUMPTION
    //   (size1 >= size2)

.text
.globl mul_bc_arm64
.type  mul_bc_arm64, @function

mul_bc_arm64:
    // Save registers
    stp     x19, x20, [sp, #-32]!
    stp     x21, x22, [sp, #16]
    
    // Save original result pointer for outer loop reset
    mov     x19, x0         // x19 = original result pointer
    mov     x20, x1         // x20 = original op1 pointer
    mov     x21, x2         // x21 = op2 pointer (will be incremented)
    
    // Outer loop: for each limb of op2
.Larm64_outer_loop_start:
    mov     x5, x3          // x5 = inner loop counter (size1)
    mov     x22, xzr        // x22 = carry accumulator = 0
    ldr     x6, [x21], #8   // x6 = op2[i] (load and post-increment)
    
    // Check if inner loop should run
    cbz     x5, .Larm64_outer_loop_end

    // Inner loop: multiply op2[i] with all limbs of op1
.p2align 4
.Larm64_inner_loop:
    // Load op1 limb
    ldr     x7, [x1], #8    // x7 = op1[j]
    
    // Multiply: op2[i] * op1[j]
    mul     x8, x7, x6      // x8 = low64
    umulh   x9, x7, x6      // x9 = high64
    
    // Add previous carry to low64
    adds    x8, x8, x22     // low64 += carry, set flags
    adc     x9, x9, xzr     // high64 += carry from low addition
    
    // Load current result limb and add
    ldr     x10, [x0]       // x10 = result[i + j]
    adds    x10, x10, x8    // result += low64, set flags
    str     x10, [x0], #8   // Store result and increment pointer
    
    // Prepare carry for next iteration
    adc     x22, x9, xzr    // carry = high64 + carry from result addition
    
    // Decrement inner loop counter
    subs    x5, x5, #1
    bne     .Larm64_inner_loop

.Larm64_outer_loop_end:
    // Store final carry
    ldr     x10, [x0]       // Load next result limb
    adds    x10, x10, x22   // Add final carry
    str     x10, [x0]       // Store back
    
    // Reset pointers for next outer iteration
    // result_ptr = original_result_ptr + 8 * outer_iteration
    // op1_ptr = original_op1_ptr
    add     x19, x19, #8    // Advance result base pointer by 1 limb
    mov     x0, x19         // Reset result pointer for next iteration
    mov     x1, x20         // Reset op1 pointer to beginning
    
    // Decrement outer loop counter
    subs    x4, x4, #1
    bne     .Larm64_outer_loop_start

.Larm64_end_of_func:
    // Restore registers and return
    ldp     x21, x22, [sp, #16]
    ldp     x19, x20, [sp], #32
    ret

.size mul_bc_arm64, .-mul_bc_arm64

.section .note.GNU-stack,"",@progbits