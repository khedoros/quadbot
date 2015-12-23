BINARIES:=main calibtest jstest
INCS:=. ../servo
INC:=$(foreach dir,$(INCS),-I$(dir))
LIB:=
SERVO_DRV_SRC:=../servo/Adafruit_PWM_Servo_Driver.cpp
SERVO_DRV_OBJ:=$(foreach src,$(SERVO_DRV_SRC),$(subst .cpp,.o,$(src)))
SERVO_DRV_INC:=../servo/Adafruit_PWM_Servo_Driver.h
SERVO_DRV_REQ:=$(SERVO_DRV_OBJ) $(SERVO_DRV_INC)

MACHINE:=$(shell uname -m)

CXXFLAGS:=-std=c++11 -D$(MACHINE)

ifeq "$(MACHINE)" "armv6l"
    LIB+= -lwiringPi
endif

.PHONY: binaries
binaries: $(BINARIES)

main: main.o $(SERVO_DRV_REQ)
	g++ $(INC) $@.o $(SERVO_DRV_OBJ) $(LIB) -o $@

calibtest: calibtest.o $(SERVO_DRV_REQ)
	g++ $(INC) $@.o $(SERVO_DRV_OBJ) $(LIB) -o $@

jstest: jstest.o $(SERVO_DRV_REQ)
	g++ $(INC) $@.o $(SERVO_DRV_OBJ) $(LIB) -o $@

%.o: %.cpp
	echo $(CXXFLAGS)
	echo $(CXXLD)
	g++ -c $(CXXFLAGS) $(INC) $^ -o $@

.PHONY:test
test:
	echo $(foreach src,$(SERVO_DRV_SRC),$(subst .cpp,.o,$(src)))

.PHONY:clean
clean:
	rm $(BINARIES) $(SERVO_DRV_OBJ) $(foreach binobjs,$(BINARIES),$(subst ,.o,$(binobjs)))
