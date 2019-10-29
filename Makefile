
TOP := $(dir $(lastword $(MAKEFILE_LIST)))

JLEVEL := 4
EARLY_DIRS := EXAM TESTS

BRDSDKDIR ?= $(shell pwd)
BRDSDKLIB ?= $(BRDSDKDIR)/lib

include ./make/checkenv.mk
include ./make/recurse.mk
