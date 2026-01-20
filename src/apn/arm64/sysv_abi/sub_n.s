//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                   BALANCED SUBTRACTION FUNCS (N LIMBS)                    |
//   |                                                                           |
//   O---------------------------------------------------------------------------O
    
    //   Function Arguments
    //
    //   x0 -> result   (apn_seg_t*)
    //   x1 -> op1      (const apn_seg_t*)
    //   x2 -> op2      (const apn_seg_t*)
    //   x3 -> size     (apn_size_t)

.text
.globl sub_n_arm64
.type  sub_n_arm64, @function

sub_n_arm64:
    // Clear borrow flag (carry flag in subtraction context)
    mov     x4, #0          // Use x4 as temporary
    
    // Check if size is zero
    cbz     x3, .Larm64_end_of_func

    // Main loop
.p2align 4
.Larm64_loop:
    ldr     x5, [x1], #8    // Load op1 limb and post-increment
    ldr     x6, [x2], #8    // Load op2 limb and post-increment
    
    // Subtract with borrow: x5 = x5 - x6 - borrow
    sbcs    x5, x5, x6      // Sets carry flag = NOT borrow
    
    str     x5, [x0], #8    // Store result and post-increment
    
    // Decrement counter and check for zero
    subs    x3, x3, #1
    bne     .Larm64_loop

.p2align 4
.Larm64_end_of_func:
    // Get final borrow (carry clear = borrow occurred)
    cset    x0, cc          // Set x0 = 1 if carry clear (borrow), else 0
    ret

.size sub_n_arm64, .-sub_n_arm64

.section .note.GNU-stack,"",@progbits