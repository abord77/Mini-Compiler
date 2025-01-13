lis $4
.word 4
lis $5
.word 0 ; tracker for address
lis $11
.word 1
sw $31, -4($30)
sub $30, $30, $4
lis $20
.word 0x00008000 ; 2^16
lis $10
.word readWord
lis $12
.word printHex
jalr $10
add $5, $5, $4

multu $3, $20 ; extracting the immediate from beq
mflo $3
divu $3, $20
mflo $2

loopPreamble:
    jalr $10
    add $5, $5, $4
    sub $2, $2, $11
    bne $2, $11, loopPreamble
add $2, $3, $4 ; stores the final adress we are reading
jalr $10
add $5, $5, $4


loopCode:
    jalr $10
    add $1, $3, $0
    add $5, $5, $4
    beq $5, $2, cleanUp
    jalr $12
    beq $0, $0, loopCode

cleanUp:
    add $30, $30, $4
    lw $31, -4($30)
    beq $0, $0, endCodeP6

readWord:
sw $1,  -4($30)
sw $2,  -8($30)
sw $4, -12($30)
sw $5, -16($30)
sw $6, -20($30)
sw $7, -24($30)
sw $8, -28($30)
lis $8
.word 28
sub $30, $30, $8
lis $4
.word 0x01000000
lis $3
.word 0x00010000
lis $2
.word 0x00000100
lis $1
.word 0xffff0004
lw $8, 0($1)
lw $7, 0($1)
lw $6, 0($1)
lw $5, 0($1)
multu $8, $4
mflo $8
multu $7, $3
mflo $7
multu $6, $2
mflo $6
add $4, $8, $7
add $3, $6, $5
add $3, $4, $3
lis $8
.word 28
add $30, $30, $8
lw $1,  -4($30)
lw $2,  -8($30)
lw $4, -12($30)
lw $5, -16($30)
lw $6, -20($30)
lw $7, -24($30)
lw $8, -28($30)
jr $31

printHex:
sw $1,  -4($30)
sw $2,  -8($30)
sw $3, -12($30)
sw $4, -16($30)
sw $5, -20($30)
sw $6, -24($30)
sw $7, -28($30)
sw $8, -32($30)
lis $8
.word 32
sub $30, $30, $8
lis $2
.word 0x10000000
lis $3
.word 0x10
lis $4
.word 0x30
lis $6
.word 10
lis $7
.word 7  
lis $8
.word 0xffff000c

lis $5
.word 119 ; w 
sw $5, 0($8)
lis $5
.word 111 ; o
sw $5, 0($8)
lis $5
.word 114 ; r
sw $5, 0($8)
lis $5
.word 100 ; d
sw $5, 0($8)
lis $5
.word 32 ; space
sw $5, 0($8)

PHloop:
divu $1, $2
mflo $1
slt $5, $1, $6 
bne $5, $0, PHskip
        add $1, $1, $7
PHskip: add $1, $1, $4 
sw $1, 0($8)
mfhi $1
divu $2, $3
mflo $2
bne $2, $0, PHloop

lis $5
.word 32 ; space
sw $5, 0($8)
lis $5
.word 48 ; 0
sw $5, 0($8)
lis $5
.word 32 ; space
sw $5, 0($8)
lis $5
.word 48 ; 0
sw $5, 0($8)

sw $6, 0($8)



lis $8
.word 32
add $30, $30, $8
lw $1,  -4($30)
lw $2,  -8($30)
lw $3, -12($30)
lw $4, -16($30)
lw $5, -20($30)
lw $6, -24($30)
lw $7, -28($30)
lw $8, -32($30)
jr $31

endCodeP6:
    jr $31