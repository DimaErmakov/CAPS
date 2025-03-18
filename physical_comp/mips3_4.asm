    .text
    jal f              # Jump to  f, store return address in $ra
    
    add $t0, $t0, $t0
    j exit             # Jumps to exit

f:
    add $t0, $t0, $t0  
    add $t0, $t0, $t0  
    jr $ra             

exit:
    # (2a) jal f jumps to f, does lines 8 and 9, and then jr $ra, which returns to the instruction after jal (line 4).
    # line 4 adds and then j exit jumps to line 12, all this occurs due to how functions are set up in assembly.

    # (2b) The jal instruction is called jump-and-link because it jumps to the function f 
    # while also linking the return address (stores PC + 4 in $ra).


	