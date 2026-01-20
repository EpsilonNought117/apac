//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                   ADD SINGLE-LIMB TO APN-ARR FUNCTIONS                    |
//   |                                                                           |
//   O---------------------------------------------------------------------------O
    
    //   Function Arguments
    //
    //   x0 -> result   (apn_seg_t*)
    //   x1 -> op1      (const apn_seg_t*)
    //   x2 -> size     (apn_size_t)
    //   x3 -> val      (apn_seg_t)

.text
.globl add_one_arm64
.type  add_one_arm64, @function

add_one_arm64:
    // Add first limb with the immediate value
    ldr     x4, [x1], #8    // Load first limb from op1, post-increment pointer
    adds    x4, x4, x3      // Add val to first limb, set flags (carry)
    str     x4, [x0], #8    // Store result, post-increment pointer
    
    // Decrement size and check if done
    subs    x2, x2, #1      // size = size - 1
    beq     .Larm64_end_of_func  // If size == 0, we're done

.Larm64_main_loop:
    ldr     x4, [x1], #8    // Load next limb
    adcs    x4, x4, xzr     // Add carry only (xzr = zero register)
    str     x4, [x0], #8    // Store result
    
    subs    x2, x2, #1      // Decrement size counter
    bne     .Larm64_main_loop  // Continue if size != 0

.Larm64_end_of_func:
    // Get final carry
    cset    x0, cs          // Set x0 = 1 if carry set, else 0
    ret

.size add_one_arm64, .-add_one_arm64

.section .note.GNU-stack,"",@progbits