# x= 17
# y = 5
# z = x+y

.data
x:17
y:5

.text
lw $s0, x
lw $s1,y 
add $s2, $s0, $s1