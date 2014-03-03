;Global EQUs
;Local EQUs

; Glue for fenv and fpu emulation,
; DO NOT USE FOR REENTRANT CODE
;
;int Round_Mode      = ROUND_TO_NEAREST_EVEN;
;int FLUSH_TO_ZERO   = False;
;int IEEE_Exceptions = IEEE_No_Exception;
;int EnableFPExceptions = 0;
;int IEEE_Condition_Codes = 0;

		section .data
		sectype progbits
		secflags write
		secflags alloc
		secflags noexecinstr
		align 4
		global	__FE_DFL_ENV
		global	_Round_Mode
		global	_FLUSH_TO_ZERO
		global	_IEEE_Exceptions
		global	_EnableFPExceptions
		global	_IEEE_Condition_Codes
;00000000:
__FE_DFL_ENV
_Round_Mode
		dcb $00
;00000001:
		dcb $00
;00000002:
		dcb $00
;00000003:
		dcb $01
;00000004:
_FLUSH_TO_ZERO
		dcb $00
;00000005:
		dcb $00
;00000006:
		dcb $00
;00000007:
		dcb $00
;00000008:
_IEEE_Exceptions
		dcb $00
;00000009:
		dcb $00
;0000000a:
		dcb $00
;0000000b:
		dcb $00
;0000000c:
_EnableFPExceptions
		dcb $00
;0000000d:
		dcb $00
;0000000e:
		dcb $00
;0000000f:
		dcb $00
;00000010:
_IEEE_Condition_Codes
		dcb $00
;00000011:
		dcb $00
;00000012:
		dcb $00
;00000013:
		dcb $00
;00000014:

		endsec


