
#define        VIA_PORTB               $0300
#define        VIA_T1CL                $0304
#define        VIA_T1CH                $0305
#define        VIA_T1LL                $0306
#define        VIA_T1LH                $0307
#define        VIA_T2LL                $0308
#define        VIA_T2CH                $0309
#define        VIA_SR                  $030A
#define        VIA_ACR                 $030B
#define        VIA_PCR                 $030C
#define        VIA_IFR                 $030D
#define        VIA_IER                 $030E
#define        VIA_PORTA               $030F

#define        TIMEOUT                 19966     // 20000

.text

_FrameCounter
_FrameCounter_low   .dsb 1
_FrameCounter_high  .dsb 1

_IRQ_swap
_IRQ_swap_lo        .dsb 1
_IRQ_swap_hi        .dsb 1

_IRQ_Flag           .dsb 1


_Install_IRQ_handler
.(
        sei

        lda #<TIMEOUT
        sta VIA_T1LL
        lda #>TIMEOUT
        sta VIA_T1LH

        lda #0
        sta _IRQ_Flag
        sta _FrameCounter_low
        sta _FrameCounter_high

        lda $245
        sta _IRQ_swap_lo
        lda $246
        sta _IRQ_swap_hi

        lda #<_IRQ_handler
        sta $245
        lda #>_IRQ_handler
        sta $246

        cli
        rts
.)

_Restore_IRQ_handler
.(
        php
        pha
        sei

        lda _IRQ_swap_lo
        sta $245
        lda _IRQ_swap_hi
        sta $246

        pla
        plp
        cli

        rts
.)

_Wait_IRQ
        lda _IRQ_Flag
        beq _Wait_IRQ
        lda #0
        sta _IRQ_Flag
        rts


_IRQ_handler
.(
        bit VIA_T1CL
        inc _FrameCounter_low
        bne skip
        inc _FrameCounter_high
skip
        inc _IRQ_Flag
.)
        rti
