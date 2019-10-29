
ifeq ($(GIPCYDIR), )
$(error "ERROR: Environment variable GIPCYDIR not defined! Run command: export GIPCYDIR=<path to gipcy>")
endif

ifeq ($(BRDSDKDIR), )
$(error "ERROR: Environment variable BRDSDKDIR not defined! Run command: source ./sdkenv.sh")
endif




