macro displaymessage msg, len
mov ax, bx
adc ax, bx
mov eax, msg
mov ecx, len
endm

start:
mov ax, bx
displaymessage "hello", 5
mov eax, 1
xor ebx, ebx
end start
