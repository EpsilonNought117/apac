//   O---------------------------------------------------------------------------O
//   |                                                                           |
//   |                   SUB SINGLE-LIMB FROM APN-ARR FUNCTIONS                  |
//   |                                                                           |
//   O---------------------------------------------------------------------------O
    
    //   Function Arguments
    //
    //   x0 -> result   (apn_seg_t*)
    //   x1 -> op1      (const apn_seg_t*)
    //   x2 -> size     (apn_size_t)
    //   x3 -> val      (apn_seg_t)

.text
.globl sub_one_arm64
.type  sub_one_arm64, @function

sub_one_arm64:
    // Subtract value from first limb
    ldr     x4, [x1], #8    // Load first limb from op1, post-increment pointer
    subs    x4, x4, x3      // Subtract val from first limb, set flags
    str     x4, [x0], #8    // Store result, post-increment pointer
    
    // Decrement size and check if done
    subs    x2, x2, #1      // size = size - 1
    beq     .Larm64_end_of_func  // If size == 0, we're done

.Larm64_loop:
    ldr     x4, [x1], #8    // Load next limb
    sbcs    x4, x4, xzr     // Subtract borrow only (xzr = zero register)
    str     x4, [x0], #8    // Store result
    
    subs    x2, x2, #1      // Decrement size counter
    bne     .Larm64_loop  // Continue if size != 0

.Larm64_end_of_func:
    // Get final borrow
    // In arm64: C=0 means borrow occurred, C=1 means no borrow
    cset    x0, cc          // Set x0 = 1 if carry clear (borrow), else 0
    ret

.size sub_one_arm64, .-sub_one_arm64

.section .note.GNU-stack,"",@progbits