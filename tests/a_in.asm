macro summa a, b, c
mov cx, a
adc cx, b
mov c, cx
endm

macro raznost c, x
mov dx, c
sbb dx, x
endm

start:
mov ax, 7
mov bx, 13
summa ax, bx, dx 
raznost cx, bx
end start
