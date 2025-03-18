# lab 3 exercise 4-1
		.text
	add $t0, $t0, $t0
	jal f
	# jump back here

f: 	add $t0, $t0, $t0
	add $t0, $t0, $t0
	jr $ra
	
# (a) jal f stores the return address in $ra and jumps to f
# (b) jr $ra sets the PC to the value stored in $ra
# (c) Overall, the program uses function calls with jal and returning with jr.
    #  jal f saves the return address in $ra and jumps to f.
    #  jr $ra brings execution to line 5 (after jal).
    #  This program runs the way it does because of how functions are set up in assembly. 