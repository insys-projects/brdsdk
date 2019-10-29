
PHONY = clean
TARGET_NAME = host_template

include $(BRDSDKDIR)/make/macros.mk

DIRS := $(GIPCYDIR)/include $(BRDSDKDIR)/BRDINC $(BRDSDKDIR)/BRDINC/ctrladmpro

INC := $(addprefix  -I, $(DIRS))

CFLAGS += $(INC) -DIS_STEP_MAIN_THREAD

SRC := $(wildcard *.cpp)
SOURCE += $(SRC)
OBJ_FILES := $(SOURCE:.cpp=.o)

all: $(TARGET_NAME)

include $(BRDSDKDIR)/make/rules.mk
include $(BRDSDKDIR)/make/clean.mk
include $(BRDSDKDIR)/make/app.mk