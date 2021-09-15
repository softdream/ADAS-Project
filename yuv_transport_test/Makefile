CURDIR	:= $(shell pwd)

CC=arm-ca9-linux-gnueabihf-g++
FUNCTION=0
CLIENT=9
PROJECT=1

DEFS += -DFUNCTION_ID=$(FUNCTION) 
DEFS += -DCLIENT_ID=$(CLIENT) 
DEFS += -DPROJECT_ID=$(PROJECT)
DEFS += -DLINUX
CFLAGS += -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard -ftree-vectorize -fno-builtin -fno-common -Wformat=1 -D_BSP_NA51068_  -D ARM_ASM -O3 -Wall -std=c++11 -fpermissive -finline-functions -fPIC -D__LINUX
CFLAGS += $(DEFS)

TARGETDIR := $(CURDIR)
TARGET_LIB_DIR := $(TARGETDIR)
TARGET_BIN_DIR := $(TARGETDIR)
TARGET_OBJ_DIR := $(TARGETDIR) 
TARGET := $(TARGET_BIN_DIR)/testcase

INCLUDES += -I$(CURDIR) -I$(CURDIR)/inc
#包含需要的头文件路径
CFLAGS += $(INCLUDES)

CFLAGS += -L$(TARGET_LIB_DIR)
CFLAGS += -L$(TARGET_LIB_DIR)/lib

LDFLAGS += -lm -lpthread -ldl -lstdc++ -std=c++11
LDFLAGS += -lObjectEventDetect
LDFLAGS += -lhdal  -lvos -lvendor_ai2 -lvendor_ai2_pub -lprebuilt_ai -lvendor_media 
LDFLAGS += -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -lopencv_core

.PHONY:all
all: $(TARGET)

MKDIR:
	@if test ! -f $(TARGETDIR);then\
		mkdir -p $(TARGETDIR);\
	fi
	@if test ! -f $(TARGET_LIB_DIR);then\
		mkdir -p $(TARGET_LIB_DIR);\
	fi
	@if test ! -f $(TARGET_BIN_DIR);then\
		mkdir -p $(TARGET_BIN_DIR);\
	fi
	@if test ! -f $(TARGET_OBJ_DIR);then\
		mkdir -p $(TARGET_OBJ_DIR);\
	fi

$(TARGET):$(CURDIR)/testcase.cpp $(CURDIR)/transport_udp.cpp
	$(CC) -O3 -Os -o $@ $^  $(LDFLAGS) $(CFLAGS)
	@echo "------------make complete-------------"

.PHONY:clean
clean:
	rm -rf $(TARGET)
