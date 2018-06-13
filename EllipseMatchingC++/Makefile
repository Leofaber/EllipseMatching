#############################################################################
# Use make variable_name=' options ' to override the variables or make -e to
# override the file variables with the environment variables
#	make CXXFLAGS='-g'
#	make prefix='/usr'
#	make CXX=g++
# External environment variable: CFISIO, ROOTSYS, CTARTA, ICEDIR
# Instructions:
# - modify the section 1)
# - in section 10), modify the following action:
#	* all: and or remove exe and lib prerequisite
#	* lib: and or remove staticlib and dynamiclib prerequisite
#	* clean: add or remove the files and directories that should be cleaned
#	* install: add or remove the files and directories that should be installed
#	* uninstall: add or remove the files and directories that should be uninstalled
#############################################################################

PROJECT= EllipseMatching
SHELL = /bin/sh

####### 1) Project names and system

SYSTEM= $(shell gcc -dumpmachine)
#ice, ctarta, mpi, cfitsio
LINKERENV=

# Applications
AG_EXE = AG_ellipse_matching

# Libraries

VER_FILE_NAME = version.h
#the name of the directory where the conf file are copied (into $(datadir))
CONF_DEST_DIR =
#the name of the icon for the installation
ICON_NAME=

####### 2) Directories for the installation

# Prefix for each installed program. Only ABSOLUTE PATH
prefix=$(AGILE)
exec_prefix=$(prefix)
# The directory to install the binary files in.
bindir=$(exec_prefix)/bin
# The directory to install the local configuration file.
datadir=$(exec_prefix)/share
# The directory to install the libraries in.
libdir=$(exec_prefix)/lib
# The directory to install the info files in.
infodir=$(exec_prefix)/info
# The directory to install the include files in.
includedir=$(exec_prefix)/include
# The directory to install the icon
icondir=$(HOME)/.local/share/applications/

####### 3) Directories for the compiler

OBJECTS_DIR = obj
SOURCE_DIR = src
INCLUDE_DIR = include
DOC_DIR = ref
DOXY_SOURCE_DIR = code_filtered
EXE_DESTDIR  = bin
LIB_DESTDIR = lib
CONF_DIR=conf
ICON_DIR = ui

####### 4) Compiler, tools and options

ifneq (, $(findstring mpi, $(LINKERENV)))
CXX = mpic++
else
CXX = g++
endif

#CXXFLAGS = -g -02 -std=c++11 -pipe -I $(INCLUDE_DIR)

CXXFLAGS = -g -O2 -pipe -I $(INCLUDE_DIR)

ifeq ($(DEBUG),1)
	CXXFLAGS += -DDEBUG
endif


ifneq (, $(findstring root, $(LINKERENV)))
    CXXFLAGS += -W -fPIC -D_REENTRANT $(shell root-config --cflags)
    LIBS += $(shell root-config --glibs) -lMinuit
endif

LINK     = $(CXX)
#for link
LFLAGS = -shared -Wl,-soname,$(TARGET1) -Wl,-rpath,$(DESTDIR)
AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f -r
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### 5) VPATH

VPATH=$(SOURCE_DIR):$(INCLUDE_DIR):
vpath %.o $(OBJECTS_DIR)

####### 6) Files of the project

INCLUDE=$(foreach dir,$(INCLUDE_DIR), $(wildcard $(dir)/*.h))
SOURCE=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.cpp))
SOURCE+=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.c))
#Objects to build
OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE))))


####### 7) Only for library generation



####### 8) Preliminar operations

$(shell  cut $(INCLUDE_DIR)/$(VER_FILE_NAME) -f 3 > version)
#WARNING: use -d ' ' if in the version.h the separator is a space

####### 9) Pattern rules

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJECTS_DIR)/$@

#only for documentation generation
$(DOXY_SOURCE_DIR)/%.h : %.h
	cp  $<  $@

$(DOXY_SOURCE_DIR)/%.cpp : %.cpp
	cp  $<  $@

####### 10) Build rules

#all: compile the entire program.
all: exe

exe: makeobjdir $(OBJECTS)
	test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)
	$(CXX) $(CPPFLAGS) $(ALL_CFLAGS) -o $(EXE_DESTDIR)/$(AG_EXE) $(OBJECTS_DIR)/*.o $(LIBS)

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)

makelibdir:
	test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)

#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	$(DEL_FILE) *~ core *.core
	$(DEL_FILE) $(EXE_DESTDIR)/AG_*
	$(DEL_FILE) version
	$(DEL_FILE) prefix
	test $(OBJECTS_DIR) = . || $(DEL_DIR) $(OBJECTS_DIR)
	test $(EXE_DESTDIR) = . || $(DEL_DIR) $(EXE_DESTDIR)

#Delete all files from the current directory that are created by configuring or building the program.
distclean: clean

#install: compile the program and copy the executables, libraries,
#and so on to the file names where they should reside for actual use.
install: all
	# For exe installation
	test -d $(bindir) || mkdir -p $(bindir)
	$(COPY_FILE) $(EXE_DESTDIR)/* $(bindir)

#uninstall: delete all the installed files--the copies that the `install' target creates.
uninstall:
	# For exe uninstall
	$(DEL_FILE) $(bindir)/$(EXE_NAME)
