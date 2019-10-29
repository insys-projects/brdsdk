#
# Общие правила для сборки приложений в BARDY
# Файл app.mk должен быть включен в Makefile собираемого
# приложения. В Makefile приложения необходимо определить
# следующие переменные:
#
# OBJ_FILES - список объектных файлов для линковки приложения
# TARGET_NAME - имя собираемого приложения
# GIPCYLIB - директория с библиотекой libgipcy.so
# BARDYLIB - директория с библиотекой libbrd.so
#

LIBS += -lbrd -lgipcy  -lstdc++ -ldl -lpthread -lc -lrt -lm
LDOPTIONS := -Wl,-rpath,$(GIPCYLIB) -Wl,-rpath,$(BRDSDKLIB) -L"$(GIPCYLIB)" -L"$(BRDSDKLIB)" $(LIBS)

$(TARGET_NAME): $(OBJ_FILES)
	$(LD) -o $(TARGET_NAME) $(notdir $(OBJ_FILES)) $(LDOPTIONS)
