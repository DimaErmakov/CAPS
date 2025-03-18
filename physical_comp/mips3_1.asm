# DIMITRY ERMAKOV

# lab 3 exercise 1
.data
greet: .asciiz "Give me your name right now:\n"
discipline_prompt: .asciiz "Enter your favorite discipline (3-letter code, e.g., CMP, BIO):\n"
course_prompt: .asciiz "\nEnter three favorite course numbers (one at a time):\n"
course1_prompt: .asciiz "Enter first course number:\n"
course2_prompt: .asciiz "Enter second course number:\n"
course3_prompt: .asciiz "Enter third course number:\n"
output_message: .asciiz "\nThanks, "
space: .asciiz " "
course_message: .asciiz "I see you like "
end_message: .asciiz " courses.\nThat is so great. That is all now. Thanks.\n"

name: .word 0:19  # Storage for name
size: .word 20    #  max size

discipline: .space 4  # 3-letter code + null terminator
course1: .word 0  
course2: .word 0 
course3: .word 0  

.text

    # Prompt for user name
    li $v0, 4
    la $a0, greet
    syscall

    # Read user name
    li $v0, 8
    la $a0, name
    lw $a1, size  # Load max size
    syscall

    # Prompt for discipline
    li $v0, 4
    la $a0, discipline_prompt
    syscall

    # Read discipline (3-letter code)
    li $v0, 8
    la $a0, discipline
    li $a1, 4  # 3-letter code + null terminator
    syscall

    # Prompt for course numbers one at a time
    li $v0, 4
    la $a0, course_prompt
    syscall

    # Prompt and read first course number
    li $v0, 4
    la $a0, course1_prompt
    syscall

    li $v0, 5  # Read integer
    syscall
    sw $v0, course1  # Store first course

    # Prompt and read second course number
    li $v0, 4
    la $a0, course2_prompt
    syscall

    li $v0, 5
    syscall
    sw $v0, course2  # Store second course

    # Prompt and read third course number
    li $v0, 4
    la $a0, course3_prompt
    syscall

    li $v0, 5
    syscall
    sw $v0, course3  # Store third course

    # Print output message
    li $v0, 4
    la $a0, output_message
    syscall

    # Print name
    li $v0, 4
    la $a0, name
    syscall

    # Print course message
    li $v0, 4
    la $a0, course_message
    syscall

    # Print discipline
    li $v0, 4
    la $a0, discipline
    syscall

    # Print space
    li $v0, 4
    la $a0, space
    syscall
    
    # Print course numbers
    li $v0, 1
    lw $a0, course1
    syscall

    # Print space
    li $v0, 4
    la $a0, space
    syscall
    
    li $v0, 1
    lw $a0, course2
    syscall
    
    # Print space
    li $v0, 4
    la $a0, space
    syscall

    li $v0, 1
    lw $a0, course3
    syscall

    # Print end message
    li $v0, 4
    la $a0, end_message
    syscall

