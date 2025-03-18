	.data
greet: .asciiz "welcome to physical comp\n"
year: .word 2019
wow: .asciiz "\nWow, "
sure: .asciiz " sure was a great year\n"
askyr: .asciiz "give year"

.text	

la $a0, greet
li $v0, 4
syscall 

la $a0, askyr
syscall 

li $v0, 5
syscall 
la $t0, year
sw $v0, 0($t0)



la $a0, wow
li $v0, 4
syscall 

la $t0, year
lw $a0, 0($t0)
li $v0, 1
syscall 

la $a0, sure
li $v0, 4
syscall 