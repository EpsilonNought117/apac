// O---------------------------------------------------------------------------O
// |                                                                           |
// |                         LEFT BITWISE SHIFT FUNCTIONS                      |
// |                                                                           |
// O---------------------------------------------------------------------------O
    
    // Function Arguments
    //
    // x0 -> result (apn_seg_t*)
    // x1 -> op1 (const apn_seg_t*)
    // x2 -> size (apn_size_t)
    // x3 -> bit_cnt (apn_seg_t)
    
    // The wrapper calling this will assert that (bit_cnt < 64 && bit_cnt > 0)
    // also asserts that size >= 1

.text
.globl lshift_lt64_arm64
.type lshift_lt64_arm64, @function

lshift_lt64_arm64:
    // Calculate pointers to end of arrays (process from high to low)
    lsl     x4, x2, #3      // x4 = size * 8 (bytes)
    sub     x5, x4, #8      // x5 = offset to last element
    add     x1, x1, x5      // x1 points to last element of op1
    add     x0, x0, x5      // x0 points to last element of result
    
    // Calculate complement for right shift
    mov     x6, #64
    sub     x6, x6, x3      // x6 = 64 - bit_cnt (right shift amount)
    
    // Load last limb
    ldr     x7, [x1], #-8   // Load last limb, post-decrement pointer
    
    // If size == 1, we're done after processing last limb
    subs    x2, x2, #1
    beq     .Larm64_single_limb
    
    // Process middle limbs
    // We need to process size-2 middle limbs if size > 1
    subs    x2, x2, #1      // Subtract one more for the first limb
    
    // Load second-to-last limb for the loop
    ldr     x8, [x1], #-8   // Load second-to-last limb, post-decrement
    b       .Larm64_loop_check

.Larm64_main_loop:
    // Current limb in x7, previous limb in x8 (processing high to low)
    lsr     x9, x8, x6      // Shift right to get bits from previous limb
    lsl     x10, x7, x3     // Shift left to get bits from current limb
    orr     x9, x9, x10     // Combine to get final shifted value
    str     x9, [x0], #-8   // Store result, post-decrement pointer
    
    // Move to next pair (going backwards)
    mov     x7, x8          // Current limb = previous previous limb
    ldr     x8, [x1], #-8   // Load previous limb, post-decrement

.Larm64_loop_check:
    subs    x2, x2, #1
    bne     .Larm64_main_loop
    
    // Process first pair (x7 = second limb, x8 = first limb)
    lsr     x9, x8, x6      // Shift right bits from first limb
    lsl     x10, x7, x3     // Shift left bits from second limb
    orr     x9, x9, x10     // Combine
    str     x9, [x0], #-8   // Store
    
    // Process first limb
    lsl     x8, x8, x3      // Shift first limb left
    str     x8, [x0]        // Store final result
    ret

.Larm64_single_limb:
    // Handle case where size == 1
    lsl     x7, x7, x3      // Shift last/only limb left
    str     x7, [x0]        // Store result
    ret

.size lshift_lt64_arm64, .-lshift_lt64_arm64

.section .note.GNU-stack,"",@progbits