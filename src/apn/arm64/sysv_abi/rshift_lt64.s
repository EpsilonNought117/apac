// O---------------------------------------------------------------------------O
// |                                                                           |
// |                     RIGHT BITWISE SHIFT FUNCTIONS                         |
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
.globl rshift_lt64_arm64
.type rshift_lt64_arm64, @function

rshift_lt64_arm64:
    // Load first limb
    ldr     x4, [x1]        // x4 = current limb
    // Calculate complement for left shift
    mov     x5, #64
    sub     x5, x5, x3      // x5 = 64 - bit_cnt (left shift amount)
    
    // Handle first element specially
    lsr     x6, x4, x3      // x6 = first limb shifted right
    
    // If size == 1, we're done after processing first limb
    subs    x2, x2, #1
    beq     .Larm64_single_limb
    
    // Process middle limbs
    // We need to process size-2 middle limbs if size > 1
    subs    x2, x2, #1      // Subtract one more for the last limb
    
    // Load second limb for the loop
    ldr     x7, [x1, #8]!   // Load second limb, pre-increment rsi
    b       .Larm64_loop_check

.Larm64_main_loop:
    // Current limb in x4, next limb in x7
    lsl     x8, x4, x5      // Shift left to get bits from current limb
    lsr     x9, x7, x3      // Shift right to get bits from next limb
    orr     x8, x8, x9      // Combine to get final shifted value
    str     x8, [x0], #8    // Store result
    
    // Move to next pair
    mov     x4, x7          // Current limb = previous next limb
    ldr     x7, [x1, #8]!   // Load next limb, pre-increment

.Larm64_loop_check:
    subs    x2, x2, #1
    bne     .Larm64_main_loop
    
    // Process last pair (x4 = second-to-last, x7 = last)
    lsl     x8, x4, x5      // Shift left bits from second-to-last
    lsr     x9, x7, x3      // Shift right bits from last
    orr     x8, x8, x9      // Combine
    str     x8, [x0], #8    // Store
    
    // Process final limb
    lsr     x7, x7, x3      // Shift last limb right
    str     x7, [x0]        // Store final result
    ret

.Larm64_single_limb:
    // Handle case where size == 1
    str     x6, [x0]        // Store shifted first limb
    ret

.size rshift_lt64_arm64, .-rshift_lt64_arm64

.section .note.GNU-stack,"",@progbits