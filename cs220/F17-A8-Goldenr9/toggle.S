.intel_syntax noprefix
.text
.global toggle_bit

toggle_bit:
mov r8, 0x01
mov r9, 0x0
cmp rci, r9
je done
loop:
shl r8, 1
sub rci, 1
cmp rci, r9
jne loop



done:
xor rdi, r8
mov rax, rdi
ret
