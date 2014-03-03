
AS = "$(CWINSTALL)"/ARM_Tools/Command_Line_Tools/mwasmarm.exe
ARMAR = "$(CWINSTALL)"/ARM_Tools/Command_Line_Tools/mwldarm.exe

SOURCES = _d_add.s _d_sub.s _d_mul.s _d_neg.s _d_dtof.s \
  _d_dtoq.s _d_itod.s _f_cmp.s  _f_ftod.s \
  _f_ftoll.s _f_itof.s _f_lltof.s _f_neg.s \
  _ll_div.s _ll_mul.s _ll_sll.s \
  _ll_sra.s _ull_srl.s _s32_div.s _s32_div_f.s \
  _u32_div.s _u32_div_f.s \
  _f_ftoi.s _f_ftoi_r.s _f_ftou.s _f_ftou_r.s \
  _d_dtoi.s _d_dtoi_r.s _d_dtou.s _d_dtou_r.s \
  _f_ftoll.s _f_ftoll_r.s  _f_ftoull.s _f_ftoull_r.s \
  _d_dtoll.s _d_dtoll_r.s  _d_dtoull.s _d_dtoull_r.s \
  _d_sqrt.s _f_sqrt.s _f_rem.s _d_rem.s \
  _f_rnd.s _d_rnd.s _f_mul.s _f_add.s _f_sub.s \
  _f_div.s _d_div.s _d_div_xm.s _div32_common_f.s ieee_status.s __rt_divs_3.s \
  _ll_cmp.s _ll_lasr.s memory_readwrite.s

  
OBJECTS = $(SOURCES:%.s=%.o)

# Define EXPLICIT_FCMP for Nitro mathlibs to ensure compatibility with ARM2.0 compiler
#
ifeq ($(NITRO),1)
DNITRO=-DEXPLICIT_FCMP
endif


T0 = FP_flush0_xM_LE.a
T0ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DARM_xM -proc arm4xm $(DNITRO)

T1 = FP_flush0_xM_BE.a
T1ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DARM_xM -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)

T2 = FP_flush0_v4t_LE.a
T2ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -proc arm4t $(DNITRO)

T3 = FP_flush0_v4t_BE.a
T3ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -proc arm4t -big -DBIG_ENDIAN $(DNITRO)

T4 = FP_flush0_v5t_LE.a
T4ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DARM5 -proc arm5t $(DNITRO)

T5 = FP_flush0_v5t_BE.a
T5ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DARM5 -proc arm5t -big -DBIG_ENDIAN $(DNITRO)

T6 = FP_fastI_xM_LE.a
T6ASFLAGS = -I. -maxerrors 5 -colons -period -DARM_xM -proc arm4xm $(DNITRO)

T7 = FP_fastI_xM_BE.a
T7ASFLAGS = -I. -maxerrors 5 -colons -period -DARM_xM -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)

T8 = FP_fastI_v4t_LE.a
T8ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -proc arm4t $(DNITRO)

T9 = FP_fastI_v4t_BE.a
T9ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -proc arm4t -big -DBIG_ENDIAN $(DNITRO)

T10 = FP_fastI_v5t_LE.a
T10ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DARM5 -proc arm5t $(DNITRO)

T11 = FP_fastI_v5t_BE.a
T11ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DARM5 -proc arm5t -big -DBIG_ENDIAN $(DNITRO)

T12 = FP_fixedI_xM_LE.a
T12ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DARM_xM -proc arm4xm $(DNITRO)

T13 = FP_fixedI_xM_BE.a
T13ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DARM_xM -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)

T14 = FP_fixedI_v4t_LE.a
T14ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -proc arm4t $(DNITRO)

T15 = FP_fixedI_v4t_BE.a
T15ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -proc arm4t -big -DBIG_ENDIAN $(DNITRO)

T16 = FP_fixedI_v5t_LE.a
T16ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DARM5 -proc arm5t $(DNITRO)

T17 = FP_fixedI_v5t_BE.a
T17ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DARM5 -proc arm5t -big -DBIG_ENDIAN $(DNITRO)

T18 = FP_fullI_xM_LE.a
T18ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DARM_xM -proc arm4xm $(DNITRO)

T19 = FP_fullI_xM_BE.a
T19ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DARM_xM -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)

T20 = FP_fullI_v4t_LE.a
T20ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -proc arm4t $(DNITRO)

T21 = FP_fullI_v4t_BE.a
T21ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -proc arm4t -big -DBIG_ENDIAN $(DNITRO)

T22 = FP_fullI_v5t_LE.a
T22ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DARM5 -proc arm5t $(DNITRO)

T23 = FP_fullI_v5t_BE.a
T23ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DARM5 -proc arm5t -big -DBIG_ENDIAN $(DNITRO)

# Size libs
#
T40 = FP_flush0_xM_LE_sz.a
T40ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DARM_xM -DUS32_DIV_SZ -proc arm4xm $(DNITRO)
T41 = FP_flush0_xM_BE_sz.a
T41ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DARM_xM -DUS32_DIV_SZ -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)
T42 = FP_flush0_v4t_LE_sz.a
T42ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DUS32_DIV_SZ -proc arm4t $(DNITRO)
T43 = FP_flush0_v4t_BE_sz.a
T43ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DUS32_DIV_SZ -proc arm4t -big -DBIG_ENDIAN $(DNITRO)
T44 = FP_flush0_v5t_LE_sz.a
T44ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t $(DNITRO)
T45 = FP_flush0_v5t_BE_sz.a
T45ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_STD -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t -big -DBIG_ENDIAN $(DNITRO)
T46 = FP_fastI_xM_LE_sz.a
T46ASFLAGS = -I. -maxerrors 5 -colons -period -DARM_xM -DUS32_DIV_SZ -proc arm4xm $(DNITRO)
T47 = FP_fastI_xM_BE_sz.a
T47ASFLAGS = -I. -maxerrors 5 -colons -period -DARM_xM -DUS32_DIV_SZ -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)
T48 = FP_fastI_v4t_LE_sz.a
T48ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DUS32_DIV_SZ -proc arm4t $(DNITRO)
T49 = FP_fastI_v4t_BE_sz.a
T49ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DUS32_DIV_SZ -proc arm4t -big -DBIG_ENDIAN $(DNITRO)
T50 = FP_fastI_v5t_LE_sz.a
T50ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t $(DNITRO)
T51 = FP_fastI_v5t_BE_sz.a
T51ASFLAGS = -I. -maxerrors 5 -colons -period -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t -big -DBIG_ENDIAN $(DNITRO)
T52 = FP_fixedI_xM_LE_sz.a
T52ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DARM_xM -DUS32_DIV_SZ -proc arm4xm $(DNITRO)
T53 = FP_fixedI_xM_BE_sz.a
T53ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DARM_xM -DUS32_DIV_SZ -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)
T54 = FP_fixedI_v4t_LE_sz.a
T54ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DUS32_DIV_SZ -proc arm4t $(DNITRO)
T55 = FP_fixedI_v4t_BE_sz.a
T55ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DUS32_DIV_SZ -proc arm4t -big -DBIG_ENDIAN $(DNITRO)
T56 = FP_fixedI_v5t_LE_sz.a
T56ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t $(DNITRO)
T57 = FP_fixedI_v5t_BE_sz.a
T57ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FIXED -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t -big -DBIG_ENDIAN $(DNITRO)
T58 = FP_fullI_xM_LE_sz.a
T58ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DARM_xM -DUS32_DIV_SZ -proc arm4xm $(DNITRO)
T59 = FP_fullI_xM_BE_sz.a
T59ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DARM_xM -DUS32_DIV_SZ -proc arm4xm -big -DBIG_ENDIAN $(DNITRO)
T60 = FP_fullI_v4t_LE_sz.a
T60ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DUS32_DIV_SZ -proc arm4t $(DNITRO)
T61 = FP_fullI_v4t_BE_sz.a
T61ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DUS32_DIV_SZ -proc arm4t -big -DBIG_ENDIAN $(DNITRO)
T62 = FP_fullI_v5t_LE_sz.a
T62ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t $(DNITRO)
T63 = FP_fullI_v5t_BE_sz.a
T63ASFLAGS = -I. -maxerrors 5 -colons -period -DIEEE_FULL -DTHUMB -DARM5 -DUS32_DIV_SZ -proc arm5t -big -DBIG_ENDIAN $(DNITRO)


TARGETS = $(T0) $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7) $(T8) $(T9) \
$(T10) $(T11) $(T12) $(T13) $(T14) $(T15) $(T16) $(T17) $(T18) $(T19) \
$(T20) $(T21) $(T22) $(T23)

TARGETS_SZ = $(T40) $(T41) $(T42) $(T43) $(T44) $(T45) $(T46) $(T47) $(T48) $(T49) \
$(T50) $(T51) $(T52) $(T53) $(T54) $(T55) $(T56) $(T57) $(T58) $(T59) $(T60) \
$(T61) $(T62) $(T63)

ALL_TARGETS = $(TARGETS) $(TARGETS_SZ) 

# Set target specific flags
lib/$(T0) : ASFLAGS = $(T0ASFLAGS)
lib/$(T1) : ASFLAGS = $(T1ASFLAGS)
lib/$(T2) : ASFLAGS = $(T2ASFLAGS)
lib/$(T3) : ASFLAGS = $(T3ASFLAGS)
lib/$(T4) : ASFLAGS = $(T4ASFLAGS)
lib/$(T5) : ASFLAGS = $(T5ASFLAGS)
lib/$(T6) : ASFLAGS = $(T6ASFLAGS)
lib/$(T7) : ASFLAGS = $(T7ASFLAGS)
lib/$(T8) : ASFLAGS = $(T8ASFLAGS)
lib/$(T9) : ASFLAGS = $(T9ASFLAGS)
lib/$(T10) : ASFLAGS = $(T10ASFLAGS)
lib/$(T11) : ASFLAGS = $(T11ASFLAGS)
lib/$(T12) : ASFLAGS = $(T12ASFLAGS)
lib/$(T13) : ASFLAGS = $(T13ASFLAGS)
lib/$(T14) : ASFLAGS = $(T14ASFLAGS)
lib/$(T15) : ASFLAGS = $(T15ASFLAGS)
lib/$(T16) : ASFLAGS = $(T16ASFLAGS)
lib/$(T17) : ASFLAGS = $(T17ASFLAGS)
lib/$(T18) : ASFLAGS = $(T18ASFLAGS)
lib/$(T19) : ASFLAGS = $(T19ASFLAGS)
lib/$(T20) : ASFLAGS = $(T20ASFLAGS)
lib/$(T21) : ASFLAGS = $(T21ASFLAGS)
lib/$(T22) : ASFLAGS = $(T22ASFLAGS)
lib/$(T23) : ASFLAGS = $(T23ASFLAGS)

lib/$(T40) : ASFLAGS = $(T40ASFLAGS)
lib/$(T41) : ASFLAGS = $(T41ASFLAGS)
lib/$(T42) : ASFLAGS = $(T42ASFLAGS)
lib/$(T43) : ASFLAGS = $(T43ASFLAGS)
lib/$(T44) : ASFLAGS = $(T44ASFLAGS)
lib/$(T45) : ASFLAGS = $(T45ASFLAGS)
lib/$(T46) : ASFLAGS = $(T46ASFLAGS)
lib/$(T47) : ASFLAGS = $(T47ASFLAGS)
lib/$(T48) : ASFLAGS = $(T48ASFLAGS)
lib/$(T49) : ASFLAGS = $(T49ASFLAGS)
lib/$(T50) : ASFLAGS = $(T50ASFLAGS)
lib/$(T51) : ASFLAGS = $(T51ASFLAGS)
lib/$(T52) : ASFLAGS = $(T52ASFLAGS)
lib/$(T53) : ASFLAGS = $(T53ASFLAGS)
lib/$(T54) : ASFLAGS = $(T54ASFLAGS)
lib/$(T55) : ASFLAGS = $(T55ASFLAGS)
lib/$(T56) : ASFLAGS = $(T56ASFLAGS)
lib/$(T57) : ASFLAGS = $(T57ASFLAGS)
lib/$(T58) : ASFLAGS = $(T58ASFLAGS)
lib/$(T59) : ASFLAGS = $(T59ASFLAGS)
lib/$(T60) : ASFLAGS = $(T60ASFLAGS)
lib/$(T61) : ASFLAGS = $(T61ASFLAGS)
lib/$(T62) : ASFLAGS = $(T62ASFLAGS)
lib/$(T63) : ASFLAGS = $(T63ASFLAGS)

TARGET_MAK = Makefile
include Makerules
