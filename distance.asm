;;; Code Generated by ticc
;;; Program Name: Distance Formula
;;; Author: Michael K. Pellegrino
;;; Date: 2020 01 25
;;; ======================
#include "ti83plus.inc"
#define progstart $9D95
.org progStart-2
.db $BB,$6D
	bCall( _RunIndicOff )
; Display the Title Screen
	ld hl, TX_000
	bCall( _PutS )
; Get the Coordinates (X1,Y1) & (X2,Y2)
	call getuserinput
	ld hl, FP_bfr
	ld de, FP_x1
	bCall( _Mov9B )
	bCall(_NewLine)
	ld hl, TX_004
	bCall( _PutS )
	call getuserinput
	ld hl, FP_bfr
	ld de, FP_y1
	bCall( _Mov9B )
	bCall(_NewLine)
	ld hl, TX_005
	bCall( _PutS )
	call getuserinput
	ld hl, FP_bfr
	ld de, FP_x2
	bCall( _Mov9B )
	bCall(_NewLine)
	ld hl, TX_006
	bCall( _PutS )
	call getuserinput
	ld hl, FP_bfr
	ld de, FP_y2
	bCall( _Mov9B )
	bCall(_NewLine)

; Now do the math
	ld hl,FP_x1
	rst $0020
	bCall( _OP1ToOP2 )
	ld hl,FP_x2
	rst $0020
	bCall(_FPSub)
	bCall(_FPSquare)
	ld hl, OP1
	ld de, FP_x1
	bCall( _Mov9B )
	ld hl,FP_y1
	rst $0020
	bCall( _OP1ToOP2 )
	ld hl,FP_y2
	rst $0020
	bCall(_FPSub)
	bCall(_FPSquare)
	bCall( _OP1ToOP2 )
	ld hl,FP_x1
	rst $0020
	bCall(_FPAdd)
	bCall(_SqRoot)
	
; Display the value
	call dispOP1

; Store the value in the user variables "X" and "Ans"
	bCall(_StoX)
	bCall(_StoAns)

	ret

dispOP1:
	push bc
	ld a, $09
	bCall( _FormReal )
	ld hl, OP3
	bCall( _PutS )
	bCall( _NewLine )
	pop bc
	ret
;;=======================================
readkeyA:
	push af
	push hl
readkeyA0:
	bCall(_GetCSC)
	or a
	jp z, readkeyA0
	cp sk0
	jp z, readkeyA_zero
	cp sk9
	jp z, readkeyA_nine
	cp sk8
	jp z, readkeyA_eight
	cp sk7
	jp z, readkeyA_seven
	cp sk6
	jp z, readkeyA_six
	cp sk5
	jp z, readkeyA_five
	cp sk4
	jp z, readkeyA_four
	cp sk3
	jp z, readkeyA_three
	cp sk2
	jp z, readkeyA_two
	cp sk1
	jp z, readkeyA_one
	cp skEnter
	jp z, readkeyA_cr
	cp skDecPnt
	jp z, readkeyA_decpt
	cp skSub
	jp z, readkeyA_negative
	cp skChs
	jp z, readkeyA_negative
	cp skLeft
	jp z, readkeyA_backspace
	cp skDel
	jp z, readkeyA_backspace
	jp readkeyA0
readkeyA1:
	bCall( _PutC )
readkeyA2:
	ld (readkeyA_byte), a
	ld hl, (text_buffer_ptr)
	ld (hl), a
	inc hl
	ld (text_buffer_ptr), hl
	ld a, (text_buffer_length)
	inc a
	ld (text_buffer_length), a
	pop hl
	pop af
	ret
readkeyA_zero:
	ld a, $30
	jp readkeyA1
readkeyA_nine:
	ld a, $39
	jp readkeyA1
readkeyA_eight:
	ld a, $38
	jp readkeyA1
readkeyA_seven:
	ld a, $37
	jp readkeyA1
readkeyA_six:
	ld a, $36
	jp readkeyA1
readkeyA_five:
	ld a, $35
	jp readkeyA1
readkeyA_four:
	ld a, $34
	jp readkeyA1
readkeyA_three:
	ld a, $33
	jp readkeyA1
readkeyA_two:
	ld a, $32
	jp readkeyA1
readkeyA_one:
	ld a, $31
	jp readkeyA1
readkeyA_cr:
	ld a, (text_buffer_length)
	or a
	jp z, readkeyA0
	ld a, $00
	jp readkeyA2
readkeyA_decpt:
	ld a, '.'
	bCall( _PutC )
	ld a, tDecPt
	jp readkeyA2
readkeyA_negative:
	ld a, '-'
	bCall( _PutC )
	ld a, tChs
	jp readkeyA2
readkeyA_backspace:
	ld a, (text_buffer_length)
	or a
	jp z, readkeyA0
	dec a
	ld (text_buffer_length), a
	push af
	ld a, (CurCol)
	dec a
	ld (CurCol),a
	ld a, ' '
	bCall(_PutC)
	ld a, (CurCol)
	dec a
	ld (CurCol),a
	pop af
	push hl
	ld hl, (text_buffer_ptr)
	dec hl
	ld (text_buffer_ptr), hl
	pop hl
	jp readkeyA0
create_equation:
	ld hl, equationName
	rst $0020; bCall( _Mov9toOP1 )
	bCall( _FindSym )
	jr c, storeEqu
	bCall( _DelVar )
storeEqu:
	ld a, (text_buffer_length)
	ld h, $00
	ld l, a
	bCall( _CreateEqu ) 
	inc de
	inc de
	ld hl, text_buffer
	ld a, (text_buffer_length)
	ld b, $00
	ld c, a
	ldir
	ret
store9_hl:
.dw $0000
store9_de:
.dw $0000
getuserinput:
	push hl
	push af
	push bc
	push de
	xor a
	ld (text_buffer_length), a
	ld hl, text_buffer
	ld (text_buffer_ptr), hl
	ld hl, prompt_text
	bCall( _PutS )
readmore:
	call readkeyA
	ld a, (text_buffer_length)
	cp $18
	jp z, buffer_filled
	ld a, (readkeyA_byte)
	or a
	jp z, buffer_filled
	jp readmore 
buffer_filled:
	ld a, (text_buffer_length)
	dec a
	ld (text_buffer_length), a
	call create_equation
	ld hl, equationName
	ld de, OP1
	ld bc, $04
	ldir
	bCall( _ParseInp )
	ld hl, OP1
	ld de, FP_bfr
	ld bc, $09
	ldir
	ld hl, equationName
	rst $0020; bCall( _Mov9toOP1 )
	bCall( _FindSym )
	bCall( _DelVar )
	pop de
	pop bc
	pop af
	pop hl
	ret
FP_bfr:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00
readkeyA_byte:
.db $00
equationName:
.db EquObj, tVarEqu, tY3, $00
text_buffer_length:
.db $00
text_buffer_ptr:
.dw $0000
text_buffer:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
prompt_text:
.db "> ",0
;;=======================================
TX_000:
.db "Distance Formulaby mr pellegrino----------------x1",0
TX_004:
.db "y1",0
TX_005:
.db "x2",0
TX_006:
.db "y2",0
FP_x1:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00
FP_y1:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00
FP_x2:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00
FP_y2:
.db $00, $00, $00, $00, $00, $00, $00, $00, $00
