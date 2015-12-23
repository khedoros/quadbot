DIRS:=input robot servo
files := $(subst .cpp,.o,$(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp)))
MACHINE:=$(shell uname -m)
CXXFLAGS:=-std=c++11 -D$(MACHINE)
ifeq "$(MACHINE)" "armv6l"
    CXXLD+= -lwiringPi
endif
control: $(files) main.o
	@echo DIRS: $(files)
	g++ *.o $(files) -o control $(CXXLD)

.PHONY:clean
clean:
	rm -f $(files) *.o
