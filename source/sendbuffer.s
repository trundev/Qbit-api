    AREA asm_func, CODE, READONLY
    EXPORT sendBufferAsm

; sendBufferAsm(const void*buf, size_t len, DigitalOut *pin)
sendBufferAsm

    push {r4,r5,r6,r7,lr}

    mov r4, r0 ; save buff
    mov r5, r1 ; save len
    mov r6, r2 ; save pin

    ; load pin address (DigitalOut)
    mov r0, r6
    ldr r1, [r0, #4] ; r1-mask for this pin
    ldr r2, [r0, #16] ; r2-clraddr
    ldr r3, [r0, #12] ; r3-setaddr

    cpsid i ; disable irq

    b _start

_nextbit                ;            C0
    str r1, [r3, #0]    ; pin := hi  C2
    tst r6, r0          ;            C3
    bne _islate         ;            C4
    str r1, [r2, #0]    ; pin := lo  C6
_islate
    lsrs r6, r6, #1     ; r6 >>= 1   C7
    bne _justbit        ;            C8

    ; not just a bit - need new byte
    adds r4, #1         ; r4++       C9
    subs r5, #1         ; r5--       C10
    bcc _stop           ; if (r5<0) goto .stop  C11
_start
    movs r6, #0x80      ; reset mask C12
    nop                 ;            C13

_common                ;             C13
    str r1, [r2, #0]   ; pin := lo   C15
    ; always re-load byte - it just fits with the cycles better this way
    ldrb r0, [r4, #0]  ; r0 := *r4   C17
    b _nextbit         ;             C20

_justbit  ; C10
    ; no nops, branch taken is already 3 cycles
    b _common ; C13

_stop
    str r1, [r2, #0]   ; pin := lo
    cpsie i            ; enable irq

    pop {r4,r5,r6,r7,pc}
