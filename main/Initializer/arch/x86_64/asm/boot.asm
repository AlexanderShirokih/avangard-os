%include "mboot2hdr.inc.asm"

global _initializer_start

section .text

; C++ entry point
extern start

; Bootloader boots us in 32-bit mode, so we start in it
bits 32

_initializer_start:
    ;Setup stack
    mov esp, stack_top

    mov edi, ebx ; boot pointer

    call cls             ; clear screen
    call check_multiboot ; check mboot v2 magic
    call check_cpuid     ; check CPUID support
    call check_long_mode ; check 64-bit mode availability

    call setup_page_tables  ; init paging tables
    call enable_paging      ; and enable it!

    lgdt [gdt64.pointer]    ; load the 64-bit GDT

    jmp gdt64.code:long_mode_start ; gdt64.code - code selector

; Start 64-bit code!
bits 64

long_mode_start:
	;Clear legacy registers
    xor rax, rax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call start
_stop: ; if something happened, halt the processor
    hlt
    jmp _stop

; gets paging table location pointer
global get_cr3
get_cr3:
	mov rax, cr3
	ret

; ---- misc functions
bits 32

cls:
    push ebx
    push ecx
    mov ebx, 0x000b8000
    mov cx, 40*25
_cls_loop:
    mov dword [ebx], 0x00
    add ebx , 4
    loop _cls_loop
    pop ecx
    pop ebx
    ret

errmsg:
	mov ebx, 0x000b8000
_errmsg_loop:
	mov ecx, [eax]
	mov ch, 0x4f
	mov [ebx], cx
	add ebx, 2
	inc eax
	cmp byte [eax], 0x00
	jne _errmsg_loop
	hlt

%include "mboot.inc.asm"
%include "cpuid.inc.asm"
%include "longmode.inc.asm"


; first setting of page tables
setup_page_tables:
	; recoursive map last PML4 entry
	mov eax, pml4_table
	or eax, 0b11 ; present + writable
	mov [pml4_table + 511 * 8], eax

    ; map first PML4 entry to PDP table
    mov eax, pdp_table
    or eax, 0b11 ; present + writable
    mov [pml4_table], eax

    ; map first PDP entry to PD table
    mov eax, pd_table
    or eax, 0b11 ; present + writable
    mov [pdp_table], eax

    ; map each PD entry to a huge 2MiB page
    mov ecx, 0         ; counter variable

.map_p2_table:
   ; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
   mov eax, 0x200000  ; 2MiB
   mul ecx            ; start address of ecx-th page
   or eax, 0b10000011 ; present + writable + huge
   mov [pd_table + ecx * 8], eax ; map ecx-th entry

   inc ecx            ; increase counter
   cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
   jne .map_p2_table  ; else map the next entry
   ret

; enables paging mode
enable_paging:
       ; load PML4 to cr3 register (cpu uses this to access the P4 table)
       mov eax, pml4_table
       
       ;debugging info
       mov long [pml4_table-8], 'HUMA'
       mov long [pml4_table-4], 'N-OK'

       mov cr3, eax

       ; enable PAE-flag in cr4 (Physical Address Extension)
       mov eax, cr4
       or eax, 1 << 5
       mov cr4, eax

       ; set the long mode bit in the EFER MSR (model specific register)
       mov ecx, 0xC0000080
       rdmsr
       or eax, 1 << 8
       wrmsr

       ; enable paging in the cr0 register
       mov eax, cr0
       or eax, 1 << 31
       mov cr0, eax

       ret

section .rodata

; Default GDT
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; new
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; Error messages
no_mboot_msg:     db 'ERROR: Invalid multiboot magic value', 0
no_long_mode_msg: db 'ERROR: CPU is too old for long mode', 0
no_cpuid_msg:     db 'ERROR: CPUID is not supported', 0

section .bss
align 4096

; reserve space for paging tables
pml4_table:
    resb 4096
pdp_table:
    resb 4096
pd_table:
    resb 4096
pt_table:
    resb 4096

; reserve space for 64 kB stack
    resb 64*1024
stack_top: