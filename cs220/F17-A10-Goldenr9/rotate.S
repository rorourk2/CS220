.intel_syntax noprefix
.text
.global rotate

rotate:
mov rax, 0x0
mov rax, rsi
cmp rdi, 0x0
je right

left:
cmp rdx, 0x0
je done
rol rax,1
sub rdx,1
jmp left


right:
cmp rdx, 0x0
je done
ror rax,1
sub rdx,1
jmp right


done:
ret

