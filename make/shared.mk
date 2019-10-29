#
# Общие правила для сборки разделяемых библиотек в BARDY
# Файл shared.mk должен быть включен в Makefile собираемой
# библиотеки. В Makefile библиотеки необходимо определить
# следующие переменные:
#
# OBJ_FILES - список объектных файлов для линковки библиотеки
# LIBNAME := libbrd.so - имя собираемой библиотеки
# SONAME   := $(LIBNAME).0 - имя символьной ссылки на библиотеку
# LIBDIR - директория для установки полученной библиотеки
# GIPCYDIR - директория с библиотекой gipcy.
# Пока не найду более красивого решения
#

LIBDIR  := $(BARDYDIR)/bin

LIBS += -ldl -lpthread -lc -lrt -lgipcy
LDOPTIONS := -Wl,-rpath $(GIPCYLIB) -L"$(GIPCYLIB)" $(LIBS)
#LDOPTIONS := -Wl,-rpath $(GIPCYLIB), -Wl,-rpath $(BARDYLIB) -L"$(GIPCYLIB)" $(LIBS)

# OS dependent flags
RDYNAMIC := -rdynamic
SHARED := -shared
DEBUG := -g
LDFLAGS := $(DEBUG) $(SHARED) $(RDYNAMIC)

$(LIBNAME): $(OBJ_FILES)
	$(LD) $(LDFLAGS) -Wl,-soname,$(SONAME) \
	-o $(LIBNAME) $(notdir $(OBJ_FILES)) $(LDOPTIONS)
	chmod 666 $(LIBNAME)
	ln -sf $(LIBNAME) $(SONAME)
	cp -d $(LIBNAME) $(LIBDIR)
	cp -d $(SONAME) $(LIBDIR)
