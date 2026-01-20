//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                         NEGATION FUNCTIONS                                |
//   |                                                                           |
//   O---------------------------------------------------------------------------O
    
    //   Function Arguments
    //
    //   x0 -> result       (apn_seg_t*)
    //   x1 -> op1          (const apn_seg_t*)
    //   x2 -> size         (apn_size_t)

.text
.globl neg_arm64
.type  neg_arm64, @function

neg_arm64:
    // Clear carry flag to start (0 - value, no initial borrow)
    mov     x3, #0          // Use x3 as temporary
    
    // Check if size is zero
    cbz     x2, .Larm64_end_of_func

.Larm64_main_loop:
    ldr     x4, [x1], #8    // Load value from op1
    // Compute 0 - value - borrow (1 - C)
    // If C=1 (no borrow), result = 0 - value
    // If C=0 (borrow), result = 0 - value - 1
    sbcs    x5, xzr, x4     // x5 = 0 - x4 - (1 - C)
    str     x5, [x0], #8    // Store result
    
    subs    x2, x2, #1      // Decrement counter
    bne     .Larm64_main_loop  // Continue if not zero

.Larm64_end_of_func:
    // No need to return borrow as this is 2's complement negation
    ret

.size neg_arm64, .-neg_arm64

.section .note.GNU-stack,"",@progbits