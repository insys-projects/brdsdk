
ifeq "$(findstring QNX, $(shell uname -a))" "QNX"
    OS := DZY_QNX
    DEF_TYPES := __QNX__
else
    OS := DZY_LINUX
    DEF_TYPES := __LINUX__
endif

ifeq "$(findstring c6x-uclinux-gcc, $(CC))" "c6x-uclinux-gcc"
    OS := DZYTOOLS_C_6_X
    TARGET := -march=c674x -D_c6x_
    DEF_TYPES := __LINUX__
endif

ifneq   "$(findstring 2.4.,$(shell uname -a))" ""
    OS := DZY_LINUX
    DZY_VER := -DDZYTOOLS_2_4_X
endif

#IPC := __IPC_QNX__
IPC := __IPC_LINUX__

#IPC_TYPE := _SYSTEMV_IPC_
#IPC_TYPE := _POSIX_IPC_
IPC_TYPE := _INSYS_IPC_
