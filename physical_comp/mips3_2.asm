# lab 3 exercise 2    
    .data
prompt: .asciiz "Enter an integer:\n"
output_message: .asciiz "Binary representation: "
newline: .asciiz "\n"
bit_string: .space 33  # 32 bits + null 


.text

    # Prompt user for input
    li $v0, 4
    la $a0, prompt
    syscall

    # Read integer input and store in $t0
    li $v0, 5
    syscall
    move $t0, $v0

    
    la $t1, bit_string  # Pointer to bit string
    li $t2, 0x80000000  # Mask
    li $t3, 32  # loop counter

bit_loop:
    # Isolate  MSB,  AND operation
    and $t4, $t0, $t2  # t4 = t0 & mask

    # Store 1 or 0 in bit string
    li $t5, 1
    beqz $t4, store_zero  # if t4 = 0, store 0
    sb $t5, 0($t1)        # Store 1
    j continue_loop

store_zero:
    li $t5, 0
    sb $t5, 0($t1) # Store 0

continue_loop:
    addi $t1, $t1, 1  # move to next character in string
    srl $t2, $t2, 1   # Shift mask to right
    subi $t3, $t3, 1  # dec counter
    bnez $t3, bit_loop  # Repeat until all 32 bits processed

    # Null-terminate bit string
    li $t5, 0
    sb $t5, 0($t1)

    #print
    li $v0, 4
    la $a0, output_message
    syscall


    li $v0, 4
    la $a0, bit_string
    syscall

    
    li $v0, 4
    la $a0, newline
    syscall

    # quit
    li $v0, 10
    syscall
