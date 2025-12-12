# By Dimitry Ermakov, all parts except interrupts for addition errors
.text
main:
    # RCR at 0xffff0000, set bit 1 for interrupt
    li $t0, 0xffff0000      # load RCR 
    lw $t1, 0($t0)          # read RCR value
    ori $t1, $t1, 0x2       # set bit 1
    sw $t1, 0($t0)          # write to RCR


    mfc0 $t0, $12           # read into $t0
    ori $t0, $t0, 0x800     # set bit 11
    mtc0 $t0, $12           # write back into status register

wait: j wait

.ktext 0x80000180
interrupt_handler:

    li $t0, 0xffff0004      # RDR address
    lw $a0, 0($t0)          # load byte from RDR into $a0, read the keyboard input

    li $v0, 11
    syscall                 # print the char
    
    eret
