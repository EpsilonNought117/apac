//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                   BALANCED ADDITION FUNCS (N LIMBS)                       |
//   |                                                                           |
//   O---------------------------------------------------------------------------O

    //   Function Arguments
    //
    //   x0 -> result   (apn_seg_t*)
    //   x1 -> op1      (const apn_seg_t*)
    //   x2 -> op2      (const apn_seg_t*)
    //   x3 -> size     (apn_size_t)

.text
.globl add_n_arm64
.type  add_n_arm64, @function

add_n_arm64:
    // Set up frame (optional)
    // Save registers if needed
    
    // Clear carry flag
    mov     x4, #0          // Use x4 as temporary for carry
    
    // Check if size is zero
    cbz     x3, .Larm64_end_of_func

    // Main loop
.p2align 4
.Larm64_main_loop:
    ldr     x5, [x1], #8    // Load op1 limb and post-increment
    ldr     x6, [x2], #8    // Load op2 limb and post-increment
    
    // Add with carry: x5 = x5 + x6 + carry
    adcs    x5, x5, x6
    
    str     x5, [x0], #8    // Store result and post-increment
    
    // Decrement counter and check for zero
    subs    x3, x3, #1
    bne     .Larm64_main_loop

.p2align 4
.Larm64_end_of_func:
    // Get final carry
    cset    x0, cs          // Set x0 = 1 if carry set, else 0
    ret

.size add_n_arm64, .-add_n_arm64

.section .note.GNU-stack,"",@progbits