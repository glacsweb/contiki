all:
	$(MAKE) -f EWL_Runtime.ARM_CORTEXM.mak -C Runtime_ARM/Projects 

rtlib:
	$(MAKE) -f EWL_Runtime.ARM_CORTEXM.mak -C Runtime_ARM/Projects RtLibs
	
prebuild:	
#  empty target for now	

cleanobj:
	$(MAKE) -f EWL_Runtime.ARM_CORTEXM.mak -C Runtime_ARM/Projects cleanobj

clean:
	$(MAKE) -f EWL_Runtime.ARM_CORTEXM.mak -C Runtime_ARM/Projects clean
