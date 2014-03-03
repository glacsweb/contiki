
#################################################################
################ WINDOWS HOST ###################################
#################################################################
ifneq (,$(findstring Windows,$(OS)))

#################################################################
################# WINDOWS HOST - command prompt / eclipse #######
#################################################################
ifeq ($(OSTYPE), console)
define SYS_OS
endef
define CREATE_DIR_IF_NOT_EXISTS
  if not exist "$(subst \$(space),$(space),$1)" (md "$(subst \$(space),$(space),$1)")
endef
define REMOVE_FILE
  if exist $(subst /,\,$1) del /f /q /s $(subst /,\,$1)
endef
define REMOVE_DIR
  if exist $(subst /,\,$1) rd /s /q $(subst /,\,$1)
endef
define MOVE_FILE
  -move /y $(subst /,\,$1) $(subst /,\,$2)
endef
define COPY_FILE
  -copy /y $(subst /,\,$1) $(subst /,\,$2)
endef
define RENAME_FILE
  if exist $(subst /,\,$2)  del /f /q /s $(subst /,\,$2)  
  ren $(subst /,\,$1) $(notdir $2)
endef

#################################################################
################ WINDOWS HOST - cygwin console ##################
#################################################################
else
define SYS_OS
  $(shell uname -s)
endef
define CREATE_DIR_IF_NOT_EXISTS
  [ -e $1 ] || mkdir -p $1  
endef
define REMOVE_FILE
  -rm -f $1
endef
define REMOVE_DIR
  -rm -Rf $1
endef
define MOVE_FILE
  -mv -f $1 $2
endef
define RENAME_FILE
  -mv -f $1 $2
endef
define COPY_FILE
  -cp -f $1 $2
endef

endif

#################################################################
################# LINUX HOST ####################################
#################################################################
else
define SYS_OS
  $(shell uname -s)
endef
define CREATE_DIR_IF_NOT_EXISTS
  [ -e $1 ] || mkdir -p $1  
endef
define REMOVE_FILE
  -rm -f $1
endef
define REMOVE_DIR
  -rm -Rf $1
endef
define MOVE_FILE
  -mv -f $1 $2
endef
define RENAME_FILE
  -mv -f $1 $2
endef
define COPY_FILE
  -cp -f $1 $2
endef

endif
