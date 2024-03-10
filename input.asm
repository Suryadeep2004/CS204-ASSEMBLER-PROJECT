.data
    array: .word 34 1 12 32 98 17 65 100

.text
    beq x0 x0 main
    bubble_sort: addi x2 x2 -4
    sw x1 0(x2) 
    beq x11 x12 return1
    addi x6 x0 0
    addi x7 x11 -1

    loop1: beq x6 x7 exit1
    lw x28 0(x5)
    lw x29 4(x5)
    blt x28 x29 label1
    sw x28 4(x5)
    sw x29 0(x5)

    label1: addi x5 x5 4
    addi x6 x6 1
    jal x0 loop1

    exit1: addi x5 x10 0
    addi x11 x11 -1
    jal x1 bubble_sort

    return1: lw x1 0(sp)
    addi sp sp 4
    jalr x0 x1 0 

    main: auipc x10 65536
    addi x10 x10 -84
    add x5 x10 x0
    addi x11 x0 8
    addi x12 x0 1
    jal x1 bubble_sort