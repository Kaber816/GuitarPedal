# Project Name
TARGET = GuitarPedal 

# Sources: main.cpp plus everything in hardware/, effect/, utils/
CPP_SOURCES = main.cpp \
              $(wildcard hardware/*.cpp) \
              $(wildcard effect/*.cpp) \
              $(wildcard utils/*.cpp)

# Library Locations (relative to this Makefile, which lives in src/)
LIBDAISY_DIR = ../libDaisy
DAISYSP_DIR  = ../DaisySP

# Extra include paths:
C_INCLUDES += -I. -Ihardware -Ieffect -Iutils -I../Terrarium

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
