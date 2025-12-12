# By Dimitry Ermakov, interrupts for addition errors
.data
overflow_msg: .asciiz "Overflow occurred\n" 
prompt1: .asciiz "Enter first integer: "    
prompt2: .asciiz "Enter second integer: "    
sum_msg: .asciiz "Sum: "                       

.text
main:
    li $t0, 0xffff0000          # load RCR address
    lw $t1, 0($t0)              # Read current RCR value into $t1
    ori $t1, $t1, 0x2           # set bit 1
    sw $t1, 0($t0)              # write back to RCR

    mfc0 $t0, $12 
    ori $t0, $t0, 0x800        # set bit 11 (keyboard interrupt)
    mtc0 $t0, $12               
input_loop:

    li $v0, 4               
    la $a0, prompt1            
    syscall
    li $v0, 5                  
    syscall
    move $t0, $v0                
    li $v0, 4                   
    la $a0, prompt2              
    syscall
    li $v0, 5                   
    syscall
    move $t1, $v0            

    add $t2, $t0, $t1  # if overflow occurs, go to interrupt handler

    # if no overflow, print sum
    li $v0, 4                 
    la $a0, sum_msg      
    syscall

    li $v0, 1                   #  print sum
    move $a0, $t2              
    syscall

    li $v0, 11                  
    li $a0, '\n'                
    syscall

    # Loop back to prompt for new integers
    j input_loop

.ktext 0x80000180
interrupt_handler:

    # print overflow error message
    li $v0, 4                  
    la $a0, overflow_msg      
    syscall

    # adjust EPC to skip over instruction with overflow
    mfc0 $t0, $14                # move EPC into $t0
    addi $t0, $t0, 4             # EPC += 4
    mtc0 $t0, $14                # write EPC back

    la $t0, input_loop           # load address of input_loop
    jr $t0                       # jump
