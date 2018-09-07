
; Check Multiboot 2 magic code
; If it's bad, show error message and stop booting

check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret

.no_multiboot:
    mov eax, no_mboot_msg
    jmp errmsg
