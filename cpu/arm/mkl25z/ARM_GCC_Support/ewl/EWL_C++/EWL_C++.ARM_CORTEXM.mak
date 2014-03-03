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
OBJDATA_ROOT = EWL_C++.ARM_CmdLineData


# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT = ../../ewl
INCLUDE = -I- -I$(INCLUDE_ROOT)/EWL_C++/include 
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/include 


# Build ARM ABI compliant C libs if make specified with ABI=1
#
ifeq ($(ABI),1)
ABISPEC=-D_AEABI_PORTABILITY_LEVEL=1
else
ABISPEC=
endif

OPTS=4

# Common build flags
#
CXFLAGS	= -warnings on,nocmdline -opt level=$(OPTS),speed -inline level=8,bottomup -ipa file -noconstpool -char unsigned  -Cpp_exceptions on -str reuse $(INCLUDE) -c $(ABISPEC) -g3
LXFLAGS	= -warnings on,nocmdline  -library 

# Target name definition and target specific build flags
#
T01 = libc++_Thumb_LE_v7M 
$(join $(T01),CFLAGS) = -proc v7 -little -thumb -interworking -lavender model=ewl_c++
$(join $(T01),LFLAGS) = -proc v7 -little -thumb -interworking

T02 = libc++_Thumb_BE_v7M
$(join $(T02),CFLAGS) = -proc v7 -big -thumb -interworking -lavender model=ewl_c++
$(join $(T02),LFLAGS) = -proc v7 -big -thumb -interworking

T03 = libc++_Thumb_LE_PIC_PID_v7M
$(join $(T03),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -lavender model=ewl_c++ 
$(join $(T03),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T04 = libc++_Thumb_BE_PIC_PID_v7M
$(join $(T04),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -lavender model=ewl_c++
$(join $(T04),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T05 = libc++_Thumb_LE_VFP_v7M 
$(join $(T05),CFLAGS) = -proc v7 -little -thumb -interworking -lavender model=ewl_c++ -fp=vfpv4
$(join $(T05),LFLAGS) = -proc v7 -little -thumb -interworking

T06 = libc++_Thumb_BE_VFP_v7M
$(join $(T06),CFLAGS) = -proc v7 -big -thumb -interworking -lavender model=ewl_c++ -fp=vfpv4
$(join $(T06),LFLAGS) = -proc v7 -big -thumb -interworking

T07 = libc++_Thumb_LE_PIC_PID_VFP_v7M
$(join $(T07),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -lavender model=ewl_c++ -fp=vfpv4 
$(join $(T07),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T08 = libc++_Thumb_BE_PIC_PID_VFP_v7M
$(join $(T08),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -lavender model=ewl_c++ -fp=vfpv4
$(join $(T08),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T11 = libstdc++_Thumb_LE_v7M 
$(join $(T11),CFLAGS) = -proc v7 -little -thumb -interworking -lavender model=c9x_c++
$(join $(T11),LFLAGS) = -proc v7 -little -thumb -interworking

T12 = libstdc++_Thumb_BE_v7M
$(join $(T12),CFLAGS) = -proc v7 -big -thumb -interworking -lavender model=c9x_c++
$(join $(T12),LFLAGS) = -proc v7 -big -thumb -interworking

T13 = libstdc++_Thumb_LE_PIC_PID_v7M
$(join $(T13),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -lavender model=c9x_c++
$(join $(T13),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T14 = libstdc++_Thumb_BE_PIC_PID_v7M
$(join $(T14),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -lavender model=c9x_c++
$(join $(T14),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

T15 = libstdc++_Thumb_LE_VFP_v7M 
$(join $(T15),CFLAGS) = -proc v7 -little -thumb -interworking -lavender model=c9x_c++ -fp=vfpv4
$(join $(T15),LFLAGS) = -proc v7 -little -thumb -interworking

T16 = libstdc++_Thumb_BE_VFP_v7M
$(join $(T16),CFLAGS) = -proc v7 -big -thumb -interworking -lavender model=c9x_c++ -fp=vfpv4
$(join $(T16),LFLAGS) = -proc v7 -big -thumb -interworking

T17 = libstdc++_Thumb_LE_PIC_PID_VFP_v7M
$(join $(T17),CFLAGS) = -proc v7 -little -thumb -interworking -pic -pid -lavender model=c9x_c++ -fp=vfpv4
$(join $(T17),LFLAGS) = -proc v7 -little -thumb -interworking -pic -pid

T18 = libstdc++_Thumb_BE_PIC_PID_VFP_v7M
$(join $(T18),CFLAGS) = -proc v7 -big -thumb -interworking -pic -pid -lavender model=c9x_c++ -fp=vfpv4
$(join $(T18),LFLAGS) = -proc v7 -big -thumb -interworking -pic -pid

# List of targets
#
TARGETS = $(T01) $(T02) $(T11) $(T12)
TARGETS_PI = $(T03) $(T04) $(T13) $(T14)
TARGETS_VFP = $(T05) $(T06) $(T07) $(T08) $(T15) $(T16) $(T17) $(T18)
ALL_TARGETS = $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP)

# Search paths for files
#
vpath %.cpp ./src

# Determine the library components
#
ALLFILES = $(wildcard ./src/*.cpp)

# Filter ALLFILES for excluded files here
#
ALLFILES := $(filter-out %/float.cpp,$(ALLFILES))

CFILES :=
CPPFILES := $(ALLFILES)
ASMFILES :=

# Create the object file names
#
COBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
CPPOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))

# Define the target makefile name
#
TARGET_MAK = EWL_C++.ARM_CORTEXM.mak

# The common makefile will try to make all targets by recursively calling
# make TARGET=<targetname>. This creates a target consisting of the library
# name with dependencies consisting of all the objects from FILES. A static
# pattern rule defines the .c files for the dependencies of the .o files.
#
ifndef TARGET
all: $(TARGETS) $(TARGETS_PI) $(TARGETS_VFP)


$(TARGETS) $(TARGETS_PI) $(TARGETS_VFP):
	@echo "Making $@"
	make -f $(TARGET_MAK) TARGET=$@

endif

COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
CPPOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
ASMOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))

# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : CFLAGS = $($(TARGET)CFLAGS)
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : LFLAGS = $($(TARGET)LFLAGS)
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $(ASMOBJECTS)
	$(call CREATE_DIR_IF_NOT_EXISTS,$(dir $(LIBOUT)/$(TARGET)))
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CFLAGS) $(CXFLAGS) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT) $(CFILES:.o=.c) $(CPPFILES:.o=.cpp)
	$(LINK) $(LFLAGS) $(LXFLAGS) $(COBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) -o $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS)   : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CFLAGS) $(CXFLAGS) "$<" -o $@ 

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(COMPILE) $(CFLAGS) $(CXFLAGS) "$<" -o $@ 

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)); $(ASSEMBLE) $(AFLAGS) "$<" -o $@ 

precompile: EWLHeaders_Thumb_LE_v7M.mch EWLHeaders_Thumb_BE_v7M.mch

MCHCFLAGS = -proc v7 -thumb -lavender model=ewl_c++ -lang c++ -c

EWLHeaders_Thumb_LE_v7M.mch:
	$(COMPILE) $(MCHCFLAGS) $(INCLUDE) -little -Isrc include/arm/ARM.pch
	
EWLHeaders_Thumb_BE_v7M.mch:
	$(COMPILE) $(MCHCFLAGS) $(INCLUDE) -big -Isrc include/arm/ARM.pch
	
clean: $(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(ALL_TARGETS)))
# Remove generated data files, e.g. EWL_C++.ARM_CmdLineData/* 
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

