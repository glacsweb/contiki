; *** Copyright Freescale Semiconductor, Inc, 2005 *** 

	section .text

; ___clz64
; counts the number of leading 0 bits before the most significant 1 bit
; of an unsigned long long number
;
; First parameter passed in d0 d1
; Result returned in d0
;
	global ___clz64
	align 16
___clz64 TYPE func
	SIZE ___clz64,F___clz54_end-___clz64,16
;PRAGMA stack_effect ___clz64,8
	
 [
 	push d6
 	push d7
 	zxt.l d0
 	zxt.l d1
 ]
 	tsteq d0							; test if high part is 0
 	btd <countLZ64_countLo
 [
 	moveu.l #$20,d6
	clb d0,d0							; count leading zeros in high part
 ]  
 	sub #1,d0
 	abs d0
	cmpeq d0,d6
	bf <countLZ64_Fin

countLZ64_countLo:
 [	
 	tfr d6,d0
 	clb d1,d7							; count leading zeros in low part
 ]	
	sub #1,d7
	abs d7
	add d7,d0,d0
	 	
countLZ64_Fin:
 [
 	pop d6
 	pop d7
 ]
 	rts
 	
	global F___clz54_end
F___clz54_end


 
 