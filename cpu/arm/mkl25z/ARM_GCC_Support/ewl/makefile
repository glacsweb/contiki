all:
ifndef PLATFORM
	@echo "need to define a PLATFORM"
	@exit 1
endif

	$(MAKE) -f EWL_Runtime.$(PLATFORM).mak -C EWL_Runtime prebuild
	
	$(MAKE) -f EWL_C.$(PLATFORM).mak -C EWL_C
	$(MAKE) -f EWL_C++.$(PLATFORM).mak -C EWL_C++
	$(MAKE) -f EWL_Runtime.$(PLATFORM).mak -C EWL_Runtime

cleanobj:
	$(MAKE) -f EWL_C.$(PLATFORM).mak -C EWL_C cleanobj 
	$(MAKE) -f EWL_C++.$(PLATFORM).mak -C EWL_C++ cleanobj
	$(MAKE) -f EWL_Runtime.$(PLATFORM).mak  -C EWL_Runtime cleanobj

clean:
	$(MAKE) -f EWL_C.$(PLATFORM).mak -C EWL_C clean 
	$(MAKE) -f EWL_C++.$(PLATFORM).mak -C EWL_C++ clean
	$(MAKE) -f EWL_Runtime.$(PLATFORM).mak  -C EWL_Runtime clean
