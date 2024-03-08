.data
    x: .word 1


.text
    add x12 x0 x0
    potato: addi, x13, x0, 1
    BeQ x12 x13 potato
    #ok hello
    tomato :, sub x14 x13 x12
    mul x15 x13 x14
.data
    y: .asciiz "lol"
.text
    onion: jal x1 tomato