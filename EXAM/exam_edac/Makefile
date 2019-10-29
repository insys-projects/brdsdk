
PHONY = clean
TARGET_NAME = exam_edac

include $(BRDSDKDIR)/make/macros.mk

DIRS := $(GIPCYDIR)/include $(BRDSDKDIR)/BRDINC $(BRDSDKDIR)/BRDINC/ctrladmpro $(BRDSDKDIR)/EXAM/common

INC := $(addprefix  -I, $(DIRS))

CFLAGS += $(INC)

SRC := $(wildcard *.cpp)
SRC += $(wildcard ../common/*.cpp)
SOURCE += $(SRC)
OBJ_FILES := $(SOURCE:.cpp=.o)

all: $(TARGET_NAME)
	cp $(TARGET_NAME) $(BRDSDKDIR)/bin

include $(BRDSDKDIR)/make/rules.mk
include $(BRDSDKDIR)/make/clean.mk
include $(BRDSDKDIR)/make/app.mk
