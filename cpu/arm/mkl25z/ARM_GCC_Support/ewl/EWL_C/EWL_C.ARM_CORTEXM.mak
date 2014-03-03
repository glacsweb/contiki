# Tools definition 
#

# Reset environment vars you explicitly want to override
#
MWCIncludes =
export MWCIncludes

# CWINSTALL
#
ifndef CWINSTALL
errornotdefined1:
	@echo "CWINSTALL not defined"
endif

-include ./../make.os

ifneq (,$(findstring CYGWIN,$(SYS_OS)))
	CWFOLDER = '$(shell cygpath -m "$(CWINSTALL)")'
else
	CWFOLDER = '$(CWINSTALL)'
endif

TOOLS = $(CWFOLDER)/ARM_Tools/Command_Line_Tools
CC = $(TOOLS)/mwccarm
LD = $(TOOLS)/mwldarm
LINT = $(LNTDIR)/lint-nt

#
#
# End tools definition


# Local variable definitions
#
# Compile and Link command variables
COMPILE = $(CC)
LINK = $(LD) 

# Library file suffix
#
LIBTYPE = .a

# Library file output path
#
LIBOUT = ../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
OBJOUT = obj

# Root directory for all generated command line output
#
OBJDATA_ROOT = EWL_C.ARM_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT = ../../ewl
INCLUDE = -I- -I$(INCLUDE_ROOT)/EWL_C/include 
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/src
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/include/sun_math 

# Build ARM ABI compliant C libs if make specified with ABI=1
#
ABISPEC=-D_AEABI_PORTABILITY_LEVEL=1

OPTS=4

# Common build flags
#
CXFLAGS	= -warnings on,nocmdline -opt level=$(OPTS),speed -inline off -ipa off -noconstpool -char unsigned  -Cpp_exceptions off -str reuse $(INCLUDE) -c -g3
CABIXFLAGS	= $(CXFLAGS) $(ABISPEC)
#CXFLAGS	= -warnings on,nocmdline -opt level=4,speed -inline smart,bottomup -ipa file -noconstpool -char unsigned  -Cpp_exceptions off -str reuse $(INCLUDE) -c $(ABISPEC)
LXFLAGS	= -warnings on,nocmdline  -library 

LNTFLAGS = -b -t4 -u -i./include -i./src -i./include/sun_math -i./include/lnt $(LNTINCL) au-misra2.lnt co-cwARM.lnt
#	-b:		suppresses banner
#	-t4:	tab size 4
#	-u:		suppresses inter-module messages

# Target name definition and target specific build flags
#
T01 = libc_Thumb_LE_v7M 
$(join $(T01),CFLAGS) = -proc v7 -little -thumb -interworking -prefix lib_ewl.prefix
$(join $(T01),LFLAGS) = -proc v7 -little -thumb -interworking

T02 = libc_Thumb_BE_v7M
$(join $(T02),CFLAGS) = -proc v7 -big -thumb -interworking -prefix lib_ewl.prefix
$(join $(T02),LFLAGS) = -proc v7 -big -thumb -interworking

T03 = libc_Thumb_LE_PIC_PID_v7M
$(join $(T03),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix lib_ewl.prefix
$(join $(T03),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T04 = libc_Thumb_BE_PIC_PID_v7M
$(join $(T04),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix lib_ewl.prefix 
$(join $(T04),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T05 = libc_Thumb_LE_VFP_v7M
$(join $(T05),CFLAGS) = -proc v7 -little -thumb -interworking -prefix lib_ewl.prefix -fp=vfpv4
$(join $(T05),LFLAGS) = -proc v7 -little -thumb -interworking

T06 = libc_Thumb_BE_VFP_v7M
$(join $(T06),CFLAGS) = -proc v7 -big -thumb -interworking -prefix lib_ewl.prefix -fp=vfpv4
$(join $(T06),LFLAGS) = -proc v7 -big -thumb -interworking

T07 = libc_Thumb_LE_PIC_PID_VFP_v7M
$(join $(T07),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix lib_ewl.prefix -fp=vfpv4
$(join $(T07),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T08 = libc_Thumb_BE_PIC_PID_VFP_v7M
$(join $(T08),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix lib_ewl.prefix -fp=vfpv4
$(join $(T08),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T11 = libc99_Thumb_LE_v7M 
$(join $(T11),CFLAGS) = -proc v7 -little -thumb -interworking -prefix lib_c99.prefix
$(join $(T11),LFLAGS) = -proc v7 -little -thumb -interworking

T12 = libc99_Thumb_BE_v7M
$(join $(T12),CFLAGS) = -proc v7 -big -thumb -interworking -prefix lib_c99.prefix
$(join $(T12),LFLAGS) = -proc v7 -big -thumb -interworking

T13 = libc99_Thumb_LE_PIC_PID_v7M
$(join $(T13),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix lib_c99.prefix
$(join $(T13),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T14 = libc99_Thumb_BE_PIC_PID_v7M
$(join $(T14),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix lib_c99.prefix
$(join $(T14),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T15 = libc99_Thumb_LE_VFP_v7M 
$(join $(T15),CFLAGS) = -proc v7 -little -thumb -interworking -prefix lib_c99.prefix -fp=vfpv4
$(join $(T15),LFLAGS) = -proc v7 -little -thumb -interworking

T16 = libc99_Thumb_BE_VFP_v7M
$(join $(T16),CFLAGS) = -proc v7 -big -thumb -interworking -prefix lib_c99.prefix -fp=vfpv4
$(join $(T16),LFLAGS) = -proc v7 -big -thumb -interworking

T17 = libc99_Thumb_LE_PIC_PID_VFP_v7M
$(join $(T17),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix lib_c99.prefix -fp=vfpv4
$(join $(T17),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T18 = libc99_Thumb_BE_PIC_PID_VFP_v7M
$(join $(T18),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix lib_c99.prefix -fp=vfpv4
$(join $(T18),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T21 = libm_Thumb_LE_v7M 
$(join $(T21),CFLAGS) = -proc v7 -little -thumb -interworking -prefix libm.prefix
$(join $(T21),LFLAGS) = -proc v7 -little -thumb -interworking

T22 = libm_Thumb_BE_v7M
$(join $(T22),CFLAGS) = -proc v7 -big -thumb -interworking -prefix libm.prefix
$(join $(T22),LFLAGS) = -proc v7 -big -thumb -interworking

T23 = libm_Thumb_LE_PIC_PID_v7M
$(join $(T23),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix libm.prefix
$(join $(T23),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T24 = libm_Thumb_BE_PIC_PID_v7M
$(join $(T24),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix libm.prefix
$(join $(T24),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T25 = libm_Thumb_LE_VFP_v7M 
$(join $(T25),CFLAGS) = -proc v7 -little -thumb -interworking -prefix libm.prefix -fp=vfpv4
$(join $(T25),LFLAGS) = -proc v7 -little -thumb -interworking

T26 = libm_Thumb_BE_VFP_v7M
$(join $(T26),CFLAGS) = -proc v7 -big -thumb -interworking -prefix libm.prefix -fp=vfpv4
$(join $(T26),LFLAGS) = -proc v7 -big -thumb -interworking

T27 = libm_Thumb_LE_PIC_PID_VFP_v7M
$(join $(T27),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -prefix libm.prefix -fp=vfpv4
$(join $(T27),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T28 = libm_Thumb_BE_PIC_PID_VFP_v7M
$(join $(T28),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -prefix libm.prefix -fp=vfpv4
$(join $(T28),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

# List of targets
#
TARGETS = $(T01) $(T02) $(T11) $(T12) $(T21) $(T22) 
TARGETS_PI = $(T03) $(T04) $(T13) $(T14) $(T23) $(T24) 
TARGETS_VFP = $(T05) $(T06) $(T07) $(T08) $(T15) $(T16) $(T17) $(T18) $(T25) $(T26) $(T27) $(T28)
ALL_TARGETS = $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP)

# Search paths for files
#
vpath % ./src ./src/sys ./src/sun_math/Double_precision ./src/extras

# Determine the library components
#
ALLFILES = $(wildcard ./src/*.c)
ALLFILES += $(wildcard ./src/arm/*.c)
ALLMATHFILES = $(wildcard ./src/sun_math/*.c)
ALLMATHFILES += $(wildcard ./src/sun_math/Double_precision/*.c)

# Exclude these files from the library
#
CFILES := $(ALLFILES)
CFILES := $(filter-out %/critical_regions_NITRO.c,$(CFILES))
CFILES := $(filter-out %/pool_alloc.NITRO.c,$(CFILES))
CFILES := $(filter-out %/critical_regions_pthreads.c,$(CFILES))
CFILES := $(filter-out %/thread_local_data_pthreads.c,$(CFILES))
CFILES := $(filter-out %/uart_console_io.c,$(CFILES))
CFILES := $(filter-out %/complex_rt.c,$(CFILES))
CFILES := $(filter-out %/math.c,$(CFILES))
CFILES := $(filter-out %/math_double.c,$(CFILES))
CFILES := $(filter-out %/math_float.c,$(CFILES))
CFILES := $(filter-out %/math_longdouble.c,$(CFILES))
CFILES := $(filter-out %/math_fma.c,$(CFILES))
CFILES := $(filter-out %/math_api.c,$(CFILES))
CFILES := $(filter-out %/float.c,$(CFILES))
CFILES := $(filter-out %/pool_alloc.c,$(CFILES))
CFILES := $(filter-out %/fenv_arm_ms.c,$(CFILES))

CABIFILES := $(filter %/errno_aeabi.c,$(CFILES))
CABIFILES += $(filter %/osnone_aeabi.c,$(CFILES))
CABIFILES += $(filter %/locale_aeabi.c,$(CFILES))
CABIFILES += $(filter %/signal_aeabi.c,$(CFILES))
CABIFILES += $(filter %/arith_aeabi.c,$(CFILES))
CABIFILES += $(filter %/localel_aeabi.c,$(CFILES))
CABIFILES += $(filter %/assert_aeabi.c,$(CFILES))
CABIFILES += $(filter %/ctype_aeabi.c,$(CFILES))
CABIFILES += $(filter %/file_io_aeabi.c,$(CFILES))
CABIFILES += $(filter %/math_aeabi.c,$(CFILES))

CFILES := $(filter-out %/errno_aeabi.c,$(CFILES))
CFILES := $(filter-out %/osnone_aeabi.c,$(CFILES))
CFILES := $(filter-out %/locale_aeabi.c,$(CFILES))
CFILES := $(filter-out %/signal_aeabi.c,$(CFILES))
CFILES := $(filter-out %/arith_aeabi.c,$(CFILES))
CFILES := $(filter-out %/localel_aeabi.c,$(CFILES))
CFILES := $(filter-out %/assert_aeabi.c,$(CFILES))
CFILES := $(filter-out %/ctype_aeabi.c,$(CFILES))
CFILES := $(filter-out %/file_io_aeabi.c,$(CFILES))
CFILES := $(filter-out %/math_aeabi.c,$(CFILES))

C99FILES := $(filter %/printformat.c,$(CFILES))
C99FILES += $(filter %/scanformat.c,$(CFILES))
C99FILES += $(filter %/wtime.c,$(CFILES))
C99FILES += $(filter %/wcstold.c,$(CFILES))
C99FILES += $(filter %/wcstoul.c,$(CFILES))
C99FILES += $(filter %/wfile_io.c,$(CFILES))
C99FILES += $(filter %/wchar_io.c,$(CFILES))
C99FILES += $(filter %/wctrans.c,$(CFILES))
C99FILES += $(filter %/wctype.c,$(CFILES))
C99FILES += $(filter %/wmem.c,$(CFILES))
C99FILES += $(filter %/wprintformat.c,$(CFILES))
C99FILES += $(filter %/wprintf.c,$(CFILES))
C99FILES += $(filter %/wscanf.c,$(CFILES))
C99FILES += $(filter %/wbstring.c,$(CFILES))
C99FILES += $(filter %/wstring.c,$(CFILES))

CFMTFILES := $(filter %/printformat_.c,$(CFILES))
CFMTFILES += $(filter %/printformat_ll.c,$(CFILES))
CFMTFILES += $(filter %/printformat_fp.c,$(CFILES))
CFMTFILES += $(filter %/printformat_fp_ll.c,$(CFILES))
CFMTFILES += $(filter %/printformat_s.c,$(CFILES))
CFMTFILES += $(filter %/printformat_s_ll.c,$(CFILES))
CFMTFILES += $(filter %/printformat_s_fp.c,$(CFILES))
CFMTFILES += $(filter %/printformat_s_fp_ll.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_ll.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_fp.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_fp_ll.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_s.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_s_ll.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_s_fp.c,$(CFILES))
CFMTFILES += $(filter %/scanformat_s_fp_ll.c,$(CFILES))

CFILES := $(filter-out %/printformat.c,$(CFILES))
CFILES := $(filter-out %/scanformat.c,$(CFILES))
CFILES := $(filter-out %/wtime.c,$(CFILES))
CFILES := $(filter-out %/wcstold.c,$(CFILES))
CFILES := $(filter-out %/wcstoul.c,$(CFILES))
CFILES := $(filter-out %/wfile_io.c,$(CFILES))
CFILES := $(filter-out %/wchar_io.c,$(CFILES))
CFILES := $(filter-out %/wctrans.c,$(CFILES))
CFILES := $(filter-out %/wctype.c,$(CFILES))
CFILES := $(filter-out %/wmem.c,$(CFILES))
CFILES := $(filter-out %/wprintf.c,$(CFILES))
CFILES := $(filter-out %/wprintformat.c,$(CFILES))
CFILES := $(filter-out %/wscanf.c,$(CFILES))
CFILES := $(filter-out %/wbstring.c,$(CFILES))
CFILES := $(filter-out %/wstring.c,$(CFILES))

CFILES := $(filter-out %/printformat_.c,$(CFILES))
CFILES := $(filter-out %/printformat_ll.c,$(CFILES))
CFILES := $(filter-out %/printformat_fp.c,$(CFILES))
CFILES := $(filter-out %/printformat_fp_ll.c,$(CFILES))
CFILES := $(filter-out %/printformat_s.c,$(CFILES))
CFILES := $(filter-out %/printformat_s_ll.c,$(CFILES))
CFILES := $(filter-out %/printformat_s_fp.c,$(CFILES))
CFILES := $(filter-out %/printformat_s_fp_ll.c,$(CFILES))
CFILES := $(filter-out %/scanformat_.c,$(CFILES))
CFILES := $(filter-out %/scanformat_ll.c,$(CFILES))
CFILES := $(filter-out %/scanformat_fp.c,$(CFILES))
CFILES := $(filter-out %/scanformat_fp_ll.c,$(CFILES))
CFILES := $(filter-out %/scanformat_s.c,$(CFILES))
CFILES := $(filter-out %/scanformat_s_ll.c,$(CFILES))
CFILES := $(filter-out %/scanformat_s_fp.c,$(CFILES))
CFILES := $(filter-out %/scanformat_s_fp_ll.c,$(CFILES))


# These are included by math.c, no need to re-compile.
MATHFILES := $(ALLMATHFILES)
MATHFILES += $(filter %/complex_rt.c,$(ALLFILES))
MATHFILES += $(filter %/float.c,$(ALLFILES))
MATHFILES += $(filter %/math_api.c,$(ALLFILES))
MATHFILES += $(filter %/math.c,$(ALLFILES))
MATHFILES += $(filter %/math_fma.c,$(ALLFILES))
MATHFILES += $(filter %/math_double.c,$(ALLFILES))
MATHFILES += $(filter %/math_float.c,$(ALLFILES))
MATHFILES += $(filter %/math_longdouble.c,$(ALLFILES))

CPPFILES  := 
UARTFILES :=

# Create the object file names
#
CABIOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CABIFILES))))
C99OBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(C99FILES))))
CFMTOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFMTFILES))))
COBJS    = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
MATHOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(MATHFILES))))
CPPOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
ASMOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))
UARTOBJ  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(UARTFILES))))

# Define the target makefile name
#
TARGET_MAK = EWL_C.ARM_CORTEXM.mak

# The common makefile will try to make all targets by recursively calling
# make TARGET=<targetname>. This creates a target consisting of the library
# name with dependencies consisting of all the objects from FILES. A static
# pattern rule defines the .c files for the dependencies of the .o files.
#

# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP) 


$(TARGETS) $(TARGETS_PI) $(TARGETS_VFP):
	make -f $(TARGET_MAK) TARGET=$@
endif

CABIOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CABIOBJS))
C99OBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(C99OBJS))
CFMTOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CFMTOBJS))
COBJECTS    = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
MATHOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(MATHOBJS))
CPPOBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
UARTOBJECT  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(UARTOBJ))
FPLOBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(FPLOBJS))

# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)$(ABIPATH)/, $(addsuffix $(LIBTYPE),$(TARGET))) : CFLAGS = $($(TARGET)CFLAGS) 
$(addprefix $(LIBOUT)$(ABIPATH)/, $(addsuffix $(LIBTYPE),$(TARGET))) : LFLAGS = $($(TARGET)LFLAGS)
$(addprefix $(LIBOUT)$(ABIPATH)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $(ASMOBJECTS) 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(dir $(LIBOUT)$(ABIPATH)/$(TARGET)))
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
ifneq (,$(findstring libc99,$(TARGET)))
	$(COMPILE) $(CABIXFLAGS) $(CFLAGS) $(CABIFILES:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CXFLAGS) $(CFLAGS) $(CFILES:.o=.c) $(C99FILES:.o=.c) $(UARTFILE:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(LINK) $(LXFLAGS) $(LFLAGS) $(COBJECTS) $(C99OBJECTS) $(CPPOBJECTS) $(UARTOBJECT) $(CABIOBJECTS) -o $@
else
ifneq (,$(findstring libm,$(TARGET)))
	$(COMPILE)  $(CXFLAGS) $(CFLAGS) $(MATHFILES:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(LINK) $(LXFLAGS) $(LFLAGS) $(MATHOBJECTS) -o $@
else
	$(COMPILE) $(CABIXFLAGS) $(CFLAGS) $(CABIFILES:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE)  $(CXFLAGS) $(CFLAGS) $(CFILES:.o=.c) $(CFMTFILES:.o=.c) $(CPPFILES:.o=.cpp) $(UARTFILE:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(LINK) $(LXFLAGS) $(LFLAGS) $(COBJECTS) $(CPPOBJECTS) $(CFMTOBJECTS) $(UARTOBJECT) $(CABIOBJECTS) -o $@
endif
endif

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(CFMTOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(CABIOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CABIXFLAGS) $(CFLAGS) "$<" -o $@ 

$(C99OBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(UARTOBJECT) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(FPLOBJECTS): $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(MATHOBJECTS): $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@ 

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CXFLAGS) $(CFLAGS) "$<" -o $@

#
#	Use something like this to check Misra compliance:
#
#	make -C ewl/EWL_C -f -f EWL_C.ARM_CORTEXM.mak misra PLATFORM=ARM_CORTEXM \
#		LNTDIR="/cygdrive/d/Lint" \
#		LNTINCL="-iD:/Lint/lnt" \
#		misra
#
misra:
	-@(for f in $(CFILES:.o=.c) $(C99FILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(lib_c99.prefix)" -d__thumb=1 -d__BIG_ENDIAN=1 $$f ; \
	done ;)
	-@(for f in $(MATHFILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(libm.prefix)" -d__thumb=1 -d__BIG_ENDIAN=1 $$f ; \
	done ;)
	-@(for f in $(CABIFILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(lib_c99.prefix)" -d__thumb=1 -d__BIG_ENDIAN=1 $(ABISPEC) $$f ; \
	done ;)
#	-@(for f in $(CFILES:.o=.c) $(C99FILES:.o=.c) ; \
#	do \
#	$(LINT) $(LNTFLAGS) "-header(lib_c99.prefix)" -d__thumb=1 $$f ; \
#	done ;)
#	-@(for f in $(MATHFILES:.o=.c) ; \
#	do \
#	$(LINT) $(LNTFLAGS) "-header(libm.prefix)" -d__thumb=1 $$f ; \
#	done ;)
#	-@(for f in $(CABIFILES:.o=.c) ; \
#	do \
#	$(LINT) $(LNTFLAGS) "-header(lib_c99.prefix)" -d__thumb=1 $(ABISPEC) $$f ; \
#	done ;)

#misra:
#	$(LINT) $(LNTFLAGS) "-limit(16)" "-header(lib_c99.prefix)" abort_exit.c

clean: $(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(ALL_TARGETS)))
# Remove generated data files, e.g. EWL_C.ARM_CmdLineData/* 
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))

# Remove library files, e.g. ../Lib/*
#
$(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(ALL_TARGETS))):
	$(call REMOVE_FILE,$(subst clean_,,$@)) 

cleanobj:
# Leave the library files intact,  clean up the object files
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
