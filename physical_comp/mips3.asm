.data
x: .word 3, 4, 8, 2, 9, 3, 6, 7, 10, 4, 12, 5, 7, 1, 2
size: .word 15

.text
la $t0, x #load address of x
la $t1, size #load address of size 
lw $t1, 0($t1) #load the size value
li $s0, 0 #sum
li $s1, 0 #count >= 5
li $t2, 0 #loop counter

loop:
    bge $t2, $t1, end_loop #if loop counter >= size jump to end

    lw $t3, 0($t0) #load current element
    add $s0, $s0, $t3 #add sum += x[t2])
    slti $t4, $t3, 5 #set $t4 to 1 if $t3 < 5, else set to 0
    beq $t4, 1, skip_line_21 #if $t4 == 1, jump to skip_line_21
    addi $s1, $s1, 1 #increment count that are  >= 5
skip_line_21:
    addi $t0, $t0, 4 #move to next element
    addi $t2, $t2, 1 #increment loop counter 
    j loop #jump back to loop line 13
end_loop: #quit

