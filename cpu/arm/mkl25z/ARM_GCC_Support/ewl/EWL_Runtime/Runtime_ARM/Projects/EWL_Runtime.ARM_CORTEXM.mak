#
# Usage: make -f EWL_Runtime.ARM_CORTEXM.mak [<target>]
#
# Build runtime library defined by <target>. If <target> is omitted, build
# all runtime library targets.
#
# TBD: add target descriptions here
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

-include ./../../../make.os

#Controls building of MATHLIB based on presence of sources
FoundMathLib := 1
ifneq ($(wildcard ../Mathlib),../Mathlib)
	FoundMathLib := 0
endif

ifneq (,$(findstring CYGWIN,$(SYS_OS)))
	CWFOLDER = '$(shell cygpath -m "$(CWINSTALL)")'
else
	CWFOLDER = '$(CWINSTALL)'
endif

OPTS    = 4

TOOLS = $(CWFOLDER)/ARM_Tools/Command_Line_Tools
CC = $(TOOLS)/mwccarm
LD = $(TOOLS)/mwldarm
AS = $(TOOLS)/mwasmarm
ASSEMBLE = $(AS)

ABI	= 1

# Local variable definitions
#
COMPILE = $(CC)
LINK = $(LD) 
ASSEMBLER = $(AS)

# Library file suffix
#
export LIBTYPE = .a

# Library file output path
#
export LIBOUT = ../../../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
export OBJOUT = obj

# Root directory for all generated command line output
#
export OBJDATA_ROOT = Thumb2_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT_RUNTIME = ../..
INCLUDE_ROOT_EWL = ../../../../ewl
INCLUDE_ROOT_MATH = ../../../../Mathlib/lib


INCLUDE = -I- -I$(INCLUDE_ROOT_EWL)/EWL_C/include
INCLUDE += -I$(INCLUDE_ROOT_EWL)/EWL_C/include/extras
INCLUDE += -I$(INCLUDE_ROOT_EWL)/EWL_C++/include
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(Runtime_Common)'
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(Runtime_Common)'/MWRuntime 
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(Runtime_Common)'/cxxabi 
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(RISC_Common)' 
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/Runtime_ARM/Source


# Build ARM ABI compliant C libs if make specified with ABI=1
#
ifeq ($(ABI),1)
#ABISPEC=-D_AEABI_PORTABILITY_LEVEL=1 -D_CPPABI -D__TARGET_ARCH_7_M=1 -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -DUNWIND_ACTIVITY_DIAGNOSTICS=1 -DPR_DIAGNOSTICS=1 -DDEBUG=1
ABISPEC=-D_AEABI_PORTABILITY_LEVEL=1 -D_CPPABI -D__TARGET_ARCH_7_M=1 -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1

else
ABISPEC=
endif


# Common build flags
#
CPPFLAGS =  -Cpp_exceptions on
CCFLAGS  =  -Cpp_exceptions off

CXFLAGS	= -g3 -warnings on,nocmdline -opt level=$(OPTS),speed -inline smart,bottomup -ipa file -noconstpool -char unsigned -str reuse $(INCLUDE) -c $(ABISPEC)
LXFLAGS	= -warnings on,nocmdline  -library 
AXFLAGS = -nocolons

# Target name definition and target specific build flags
#

T01 = librt_Thumb_LE_v7M 
$(join $(T01),CFLAGS) = -proc v7 -little -thumb -interworking 
$(join $(T01),LFLAGS) = -proc v7 -little -thumb -interworking
$(join $(T01),AFLAGS)= -proc v7 -little -thumb

T02 = librt_Thumb_BE_v7M 
$(join $(T02),CFLAGS) = -proc v7 -big -thumb -interworking 
$(join $(T02),LFLAGS) = -proc v7 -big -thumb -interworking
$(join $(T02),AFLAGS) = -proc v7 -little -thumb


T03 = librt_Thumb_LE_PIC_PID_v7M 
$(join $(T03),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid 
$(join $(T03),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid
$(join $(T03),AFLAGS) = -proc v7 -little -thumb -D__APCS_ROPI -D__APCS_RWPI


T04 = librt_Thumb_BE_PIC_PID_v7M 
$(join $(T04),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid 
$(join $(T04),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid
$(join $(T04),AFLAGS) = -proc v7 -big -thumb -D__APCS_ROPI -D__APCS_RWPI

T05 = librt_Thumb_LE_hosted_v7M 
$(join $(T05),CFLAGS) = -proc v7 -little -thumb -interworking -semihosting 
$(join $(T05),LFLAGS) = -proc v7 -little -thumb -interworking
$(join $(T05),AFLAGS)= -proc v7 -little -thumb

T06 = librt_Thumb_BE_hosted_v7M 
$(join $(T06),CFLAGS) = -proc v7 -big -thumb -interworking -semihosting 
$(join $(T06),LFLAGS) = -proc v7 -big -thumb -interworking
$(join $(T06),AFLAGS) = -proc v7 -little -thumb


T07 = librt_Thumb_LE_PIC_PID_hosted_v7M 
$(join $(T07),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -semihosting 
$(join $(T07),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid
$(join $(T07),AFLAGS) = -proc v7 -little -thumb -D__APCS_ROPI -D__APCS_RWPI


T08 = librt_Thumb_BE_PIC_PID_hosted_v7M 
$(join $(T08),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -semihosting 
$(join $(T08),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid
$(join $(T08),AFLAGS) = -proc v7 -big -thumb -D__APCS_ROPI -D__APCS_RWPI

T09 = librt_Thumb_LE_VFP_v7M 
$(join $(T09),CFLAGS) = -proc v7 -little -thumb -interworking -fp=vfpv4
$(join $(T09),LFLAGS) = -proc v7 -little -thumb -interworking
$(join $(T09),AFLAGS)= -proc v7 -little -thumb

T10 = librt_Thumb_BE_VFP_v7M 
$(join $(T10),CFLAGS) = -proc v7 -big -thumb -interworking -fp=vfpv4
$(join $(T10),LFLAGS) = -proc v7 -big -thumb -interworking
$(join $(T10),AFLAGS) = -proc v7 -little -thumb


T11 = librt_Thumb_LE_PIC_PID_VFP_v7M 
$(join $(T11),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -fp=vfpv4
$(join $(T11),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid
$(join $(T11),AFLAGS) = -proc v7 -little -thumb -D__APCS_ROPI -D__APCS_RWPI


T12 = librt_Thumb_BE_PIC_PID_VFP_v7M 
$(join $(T12),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -fp=vfpv4
$(join $(T12),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid
$(join $(T12),AFLAGS) = -proc v7 -big -thumb -D__APCS_ROPI -D__APCS_RWPI

T13 = librt_Thumb_LE_hosted_VFP_v7M 
$(join $(T13),CFLAGS) = -proc v7 -little -thumb -interworking -semihosting -fp=vfpv4
$(join $(T13),LFLAGS) = -proc v7 -little -thumb -interworking
$(join $(T13),AFLAGS)= -proc v7 -little -thumb

T14 = librt_Thumb_BE_hosted_VFP_v7M 
$(join $(T14),CFLAGS) = -proc v7 -big -thumb -interworking -semihosting -fp=vfpv4
$(join $(T14),LFLAGS) = -proc v7 -big -thumb -interworking
$(join $(T14),AFLAGS) = -proc v7 -little -thumb


T15 = librt_Thumb_LE_PIC_PID_hosted_VFP_v7M 
$(join $(T15),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -semihosting -fp=vfpv4
$(join $(T15),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid
$(join $(T15),AFLAGS) = -proc v7 -little -thumb -D__APCS_ROPI -D__APCS_RWPI


T16 = librt_Thumb_BE_PIC_PID_hosted_VFP_v7M 
$(join $(T16),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -semihosting -fp=vfpv4
$(join $(T16),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid
$(join $(T16),AFLAGS) = -proc v7 -big -thumb -D__APCS_ROPI -D__APCS_RWPI


V7T = $(T01) $(T02) $(T05) $(T06)
V7T_PI = $(T03) $(T04) $(T07) $(T08) 
V7T_VFP = $(T09) $(T10) $(T11) $(T12) $(T13) $(T14) $(T15) $(T16) 


# List of targets
#
TARGETS = $(V7T)
TARGETS_PI = $(V7T_PI)
TARGETS_VFP = $(V7T_VFP)
ALL_TARGETS = $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP)

# Search paths for files
#
vpath % ../../(RISC_Common) ../../(Runtime_Common) ../../(Runtime_Common)/MWRuntime ../../(Runtime_Common)/cxxabi ../Source

# Determine the library components
#
ALLCPPFILES  = $(wildcard ../../(RISC_Common)/*.cpp)
ALLCPPFILES += $(wildcard ../../(Runtime_Common)/*.cpp)
ALLCPPFILES += $(wildcard ../../(Runtime_Common)/cxxabi/*.cpp)
ALLCPPFILES += $(wildcard ../Source/*.cpp)

ALLCFILES  = $(wildcard ../../(RISC_Common)/*.c)
ALLCFILES += $(wildcard ../../(Runtime_Common)/*.c)
ALLCFILES += $(wildcard ../../(Runtime_Common)/cxxabi/*.c)
ALLCFILES += $(wildcard ../Source/*.c)

ALLCPFILES = $(wildcard ../Source/*.cp)

ALLASMFILES = $(wildcard ../Source/*.s)

# Exclude these files from the library
#
AFILES := $(ALLASMFILES)
CFILES := $(ALLCFILES)
CFILES := $(filter-out %/__NITRO_eabi_init.c,$(CFILES))
CFILES := $(filter-out %/nitro_console_io.c,$(CFILES))
CFILES := $(filter-out %/abort_exit.c,$(CFILES))

# Filter out Thumb_interworking.c for ARM targets only
#
ifneq (Thumb,$(findstring Thumb,$(TARGET)))
 CFILES := $(filter-out %/Thumb_interworking.c,$(CFILES))
endif

# Workaround for aeabi memclr,memcpy,etc. that don't support Thumb yet.
# Remove from Thumb targets
#
ifeq (Thumb,$(findstring Thumb,$(TARGET)))
 AFILES := $(filter-out %/memclr.s,$(AFILES))
 AFILES := $(filter-out %/memcpy.s,$(AFILES))
 AFILES := $(filter-out %/memmove.s,$(AFILES))
 AFILES := $(filter-out %/memset.s,$(AFILES))
endif

CPPFILES := $(ALLCPPFILES)
ASMFILES := $(AFILES)
CPFILES  := $(ALLCPFILES)

ifeq ($(ABI),1)
CPPFILES := $(filter-out %/ExceptionHandler.cpp,$(CPPFILES))
CPPFILES := $(filter-out %/StaticInitializers.cpp,$(CPPFILES))
CPFILES  := $(filter-out %/ExceptionHandlerTS.cp,$(CPFILES))
endif

# Create the object file names
#
COBJS   = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
CPPOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))
CPOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPFILES))))

# Define the target makefile name
#
TARGET_MAK = EWL_Runtime.ARM_CORTEXM.mak

# The common makefile will try to make all targets by recursively calling
# make TARGET=<targetname>. This creates a target consisting of the library
# name with dependencies consisting of all the objects from FILES. A static
# pattern rule defines the .c files for the dependencies of the .o files.
#
ifndef TARGET
all: RtLibs MATHLIB

RtLibs: $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP)

$(TARGETS) $(TARGETS_PI) $(TARGETS_VFP):
	@echo "Making $@"
	make -f $(TARGET_MAK) TARGET=$@
endif


COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
CPPOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
ASMOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))
CPOBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPOBJS))


# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $(COBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) $(CPOBJECTS)
	$(call CREATE_DIR_IF_NOT_EXISTS,$(LIBOUT))
	$(LINK) $(LXFLAGS) $($(TARGET)LFLAGS) $(COBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) $(CPOBJECTS) -o $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CXFLAGS) $($(TARGET)CFLAGS) $(CCFLAGS) '$<' -o $@ 

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CXFLAGS) $($(TARGET)CFLAGS) $(CPPFLAGS) '$<' -o $@ 

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(ASSEMBLE) $(AXFLAGS) $($(TARGET)AFLAGS) '$<' -o $@ 

$(CPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cp 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CXFLAGS) $($(TARGET)CFLAGS) $(CPPFLAGS) '$<' -o $@ 

MATHLIB:
	$(MAKE) -f Makefile.ARMv7M -C ../Mathlib

clean: $(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(ALL_TARGETS)))
# Remove generated data files, e.g. Thumb2_CmdLineData/* 
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
	$(MAKE) -f Makefile.ARMv7M -C ../Mathlib clean
	
# Remove library files, e.g. ../Lib/*
#
$(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(ALL_TARGETS))):
	$(call REMOVE_FILE,$(subst clean_,,$@)) 

cleanobj:
# Leave the library files intact,  clean up the object files
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
	$(MAKE) -f Makefile.ARMv7M -C ../Mathlib cleanobj
