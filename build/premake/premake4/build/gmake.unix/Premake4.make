# GNU Make project makefile autogenerated by Premake
ifndef config
  config=release
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),release)
  OBJDIR     = obj/Release
  TARGETDIR  = ../../bin/release
  TARGET     = $(TARGETDIR)/premake4
  DEFINES   += -DNDEBUG -DLUA_USE_POSIX -DLUA_USE_DLOPEN
  INCLUDES  += -I../../src/host/lua-5.1.4/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -Os
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -rdynamic
  LIBS      += -lm -ldl
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug)
  OBJDIR     = obj/Debug
  TARGETDIR  = ../../bin/debug
  TARGET     = $(TARGETDIR)/premake4
  DEFINES   += -D_DEBUG -DLUA_USE_POSIX -DLUA_USE_DLOPEN
  INCLUDES  += -I../../src/host/lua-5.1.4/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -g
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -rdynamic
  LIBS      += -lm -ldl
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/os_getcwd.o \
	$(OBJDIR)/path_isabsolute.o \
	$(OBJDIR)/os_isdir.o \
	$(OBJDIR)/os_rmdir.o \
	$(OBJDIR)/os_chdir.o \
	$(OBJDIR)/os_uuid.o \
	$(OBJDIR)/os_isfile.o \
	$(OBJDIR)/os_match.o \
	$(OBJDIR)/os_pathsearch.o \
	$(OBJDIR)/os_copyfile.o \
	$(OBJDIR)/os_mkdir.o \
	$(OBJDIR)/premake.o \
	$(OBJDIR)/string_endswith.o \
	$(OBJDIR)/scripts.o \
	$(OBJDIR)/liolib.o \
	$(OBJDIR)/ltm.o \
	$(OBJDIR)/linit.o \
	$(OBJDIR)/loadlib.o \
	$(OBJDIR)/lgc.o \
	$(OBJDIR)/lstate.o \
	$(OBJDIR)/llex.o \
	$(OBJDIR)/lstrlib.o \
	$(OBJDIR)/lmem.o \
	$(OBJDIR)/lopcodes.o \
	$(OBJDIR)/ldump.o \
	$(OBJDIR)/lmathlib.o \
	$(OBJDIR)/ldo.o \
	$(OBJDIR)/ltablib.o \
	$(OBJDIR)/lparser.o \
	$(OBJDIR)/lfunc.o \
	$(OBJDIR)/lobject.o \
	$(OBJDIR)/ldebug.o \
	$(OBJDIR)/ldblib.o \
	$(OBJDIR)/lauxlib.o \
	$(OBJDIR)/lzio.o \
	$(OBJDIR)/loslib.o \
	$(OBJDIR)/lcode.o \
	$(OBJDIR)/lapi.o \
	$(OBJDIR)/ltable.o \
	$(OBJDIR)/lbaselib.o \
	$(OBJDIR)/lvm.o \
	$(OBJDIR)/lstring.o \
	$(OBJDIR)/lundump.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Premake4
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Premake4
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/os_getcwd.o: ../../src/host/os_getcwd.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/path_isabsolute.o: ../../src/host/path_isabsolute.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_isdir.o: ../../src/host/os_isdir.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_rmdir.o: ../../src/host/os_rmdir.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_chdir.o: ../../src/host/os_chdir.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_uuid.o: ../../src/host/os_uuid.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_isfile.o: ../../src/host/os_isfile.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_match.o: ../../src/host/os_match.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_pathsearch.o: ../../src/host/os_pathsearch.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_copyfile.o: ../../src/host/os_copyfile.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/os_mkdir.o: ../../src/host/os_mkdir.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/premake.o: ../../src/host/premake.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/string_endswith.o: ../../src/host/string_endswith.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/scripts.o: ../../src/host/scripts.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/liolib.o: ../../src/host/lua-5.1.4/src/liolib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltm.o: ../../src/host/lua-5.1.4/src/ltm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/linit.o: ../../src/host/lua-5.1.4/src/linit.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/loadlib.o: ../../src/host/lua-5.1.4/src/loadlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lgc.o: ../../src/host/lua-5.1.4/src/lgc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstate.o: ../../src/host/lua-5.1.4/src/lstate.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/llex.o: ../../src/host/lua-5.1.4/src/llex.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstrlib.o: ../../src/host/lua-5.1.4/src/lstrlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lmem.o: ../../src/host/lua-5.1.4/src/lmem.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lopcodes.o: ../../src/host/lua-5.1.4/src/lopcodes.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldump.o: ../../src/host/lua-5.1.4/src/ldump.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lmathlib.o: ../../src/host/lua-5.1.4/src/lmathlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldo.o: ../../src/host/lua-5.1.4/src/ldo.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltablib.o: ../../src/host/lua-5.1.4/src/ltablib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lparser.o: ../../src/host/lua-5.1.4/src/lparser.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lfunc.o: ../../src/host/lua-5.1.4/src/lfunc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lobject.o: ../../src/host/lua-5.1.4/src/lobject.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldebug.o: ../../src/host/lua-5.1.4/src/ldebug.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldblib.o: ../../src/host/lua-5.1.4/src/ldblib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lauxlib.o: ../../src/host/lua-5.1.4/src/lauxlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lzio.o: ../../src/host/lua-5.1.4/src/lzio.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/loslib.o: ../../src/host/lua-5.1.4/src/loslib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lcode.o: ../../src/host/lua-5.1.4/src/lcode.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lapi.o: ../../src/host/lua-5.1.4/src/lapi.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltable.o: ../../src/host/lua-5.1.4/src/ltable.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lbaselib.o: ../../src/host/lua-5.1.4/src/lbaselib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lvm.o: ../../src/host/lua-5.1.4/src/lvm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstring.o: ../../src/host/lua-5.1.4/src/lstring.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lundump.o: ../../src/host/lua-5.1.4/src/lundump.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
