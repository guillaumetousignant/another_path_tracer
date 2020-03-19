#----------------------------------------------------+
# C++ makefile with recursive directory file search
# by Matthieu Parenteau
# PI4 2018
#----------------------------------------------------+

# Operating System
SHELL = /bin/sh
OS := $(shell uname)

# Text Variables
comma := ,
empty :=
space := $(empty) $(empty)
sharp := "\#"
quote := \"
newline := ""

#-------------------------------------------------------------------------------------------------------------------+
# Directory Variables

ifdef VERIFY
DeactivatedFiles = main.cpp /winlibs/*
else
DeactivatedFiles := $(shell grep -v '^//' .makeignore | grep -v '^$$')
endif

# Names of all root folders (these contains source code in any tree structure of visible folders)
SourceDirs := $(shell tree -dfi --noreport)

# Determine the path for the source code folders in the .debug folder (will mimic the visible root tree structure)
DebugDirs := $(subst . ,,$(subst ./,.debug/,$(SourceDirs)))

# Determine the path for the source code folders in the .release folder (will mimic the visible root tree structure)
ReleaseDirs := $(subst . ,,$(subst ./,.release/,$(SourceDirs)))

# Determine the path for the source code folders in the .debug folder (will mimic the visible root tree structure)
MPIDebugDirs := $(subst . ,,$(subst ./,.mpidebug/,$(SourceDirs)))

# Determine the path for the source code folders in the .release folder (will mimic the visible root tree structure)
MPIReleaseDirs := $(subst . ,,$(subst ./,.mpirelease/,$(SourceDirs)))

# Determine the path for the source code folders in the .debug folder (will mimic the visible root tree structure)
WINDebugDirs := $(subst . ,,$(subst ./,.windebug/,$(SourceDirs)))

# Determine the path for the source code folders in the .release folder (will mimic the visible root tree structure)
WINReleaseDirs := $(subst . ,,$(subst ./,.winrelease/,$(SourceDirs)))

# Determine the path for the source code folders in the .debug folder (will mimic the visible root tree structure)
ANDROIDDebugDirs := $(subst . ,,$(subst ./,.androiddebug/,$(SourceDirs)))

# Determine the path for the source code folders in the .release folder (will mimic the visible root tree structure)
ANDROIDReleaseDirs := $(subst . ,,$(subst ./,.androidrelease/,$(SourceDirs)))

# Concatenate all of the previous 3 folder name variables
AllDirs := $(SourceDirs) $(ReleaseDirs) $(DebugDirs) $(MPIReleaseDirs) $(MPIDebugDirs) $(WINReleaseDirs) $(WINDebugDirs) $(ANDROIDReleaseDirs) $(ANDROIDDebugDirs)

#-------------------------------------------------------------------------------------------------------------------+
#-----------------------------------------------------------------------------------------------------+
# File Variables

# Shared Library
current_dir := $(shell basename $(CURDIR))

Executable = $(current_dir)
WINExecutable = $(current_dir).exe

ifdef VERIFY
ExecutableSourceFile = main_test.cpp
else
ExecutableSourceFile = main.cpp
endif

# Executable debug object file
ExecutableDebugObjectFile = $(subst .cpp,.o,$(addprefix .debug/,$(ExecutableSourceFile)))

# Executable release object file
ExecutableReleaseObjectFile = $(subst .cpp,.o,$(addprefix .release/,$(ExecutableSourceFile)))

# Executable debug object file
ExecutableMPIDebugObjectFile = $(subst .cpp,.o,$(addprefix .mpidebug/,$(ExecutableSourceFile)))

# Executable release object file
ExecutableMPIReleaseObjectFile = $(subst .cpp,.o,$(addprefix .mpirelease/,$(ExecutableSourceFile)))

# Executable debug object file
ExecutableWINDebugObjectFile = $(subst .cpp,.o,$(addprefix .windebug/,$(ExecutableSourceFile)))

# Executable release object file
ExecutableWINReleaseObjectFile = $(subst .cpp,.o,$(addprefix .winrelease/,$(ExecutableSourceFile)))

# Executable debug object file
ExecutableANDROIDDebugObjectFile = $(subst .cpp,.o,$(addprefix .androiddebug/,$(ExecutableSourceFile)))

# Executable release object file
ExecutableANDROIDReleaseObjectFile = $(subst .cpp,.o,$(addprefix .androidrelease/,$(ExecutableSourceFile)))

# Source files (.cpp)
SourceFiles := $(filter-out $(ExecutableSourceFile),$(subst ./,,$(subst .//,,$(shell find ./ -regex .*.cpp))))

# Filtered Source files (.cpp with deactivated files)
ActiveSourceFiles = $(filter-out $(DeactivatedFiles),$(SourceFiles))

# Debug object files (names of the object files that are produced by make debug)
DebugObjectFiles := $(addprefix .debug/,$(subst .cpp,.o,$(ActiveSourceFiles))) 

# Release object files (names of the object files that are produced by make release)
ReleaseObjectFiles := $(addprefix .release/,$(subst .cpp,.o,$(ActiveSourceFiles))) 

# Debug object files (names of the object files that are produced by make debug)
MPIDebugObjectFiles := $(addprefix .mpidebug/,$(subst .cpp,.o,$(ActiveSourceFiles))) 

# Release object files (names of the object files that are produced by make release)
MPIReleaseObjectFiles := $(addprefix .mpirelease/,$(subst .cpp,.o,$(ActiveSourceFiles)))

# Debug object files (names of the object files that are produced by make debug)
WINDebugObjectFiles := $(addprefix .windebug/,$(subst .cpp,.o,$(ActiveSourceFiles))) 

# Release object files (names of the object files that are produced by make release)
WINReleaseObjectFiles := $(addprefix .winrelease/,$(subst .cpp,.o,$(ActiveSourceFiles)))

# Debug object files (names of the object files that are produced by make debug)
ANDROIDDebugObjectFiles := $(addprefix .androiddebug/,$(subst .cpp,.o,$(ActiveSourceFiles))) 

# Release object files (names of the object files that are produced by make release)
ANDROIDReleaseObjectFiles := $(addprefix .androidrelease/,$(subst .cpp,.o,$(ActiveSourceFiles)))

# Concatenate all object files
AnyObjectFiles := $(notdir $(shell find ./ -regex .*.o))

#-----------------------------------------------------------------------------------------------------+
#--------------------------------------------------------------------------------------------------------------------------------------+
# Compilation Variables

# Default file search locations (All directories)
VPATH := $(AllDirs)

# Default compilation configuration
CXX = g++
MPICXX = mpic++
WINCXX = x86_64-w64-mingw32-gcc-win32
ANDROIDCXX = clang++
CXXFLAGS += -std=c++11 -Wall -Wno-unused-function -Wno-strict-overflow

DEBUGFLAGS += -Og -g -pg
RELEASEFLAGS += -O3 -fopenmp

# Include search paths

# Included Libraries
IMAGELIBS += -lpng -ljpeg -ltiff
DISPLAYLIBS += -lglut -lGL
WINDISPLAYLIBS += -lfreeglut -lglu32 -lopengl32
WINLIBS += -lstdc++ -lgdi32
ANDROIDLIBS += -lm
DEBUGLIBS += -lpthread

#--------------------------------------------------------------------------------------------------------------------------------------+
#---------------------------------------------------------------------------------------------------+
# Targets

all : release $(ReleaseObjectFiles)

debug : .debug  begun $(DebugObjectFiles) $(ExecutableDebugObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(DebugObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking Debug...'
	@$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(ExecutableDebugObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(DISPLAYLIBS) $(DEBUGLIBS)
	@printf 'Done'
	@printf '\n'

release : .release begun $(ReleaseObjectFiles) $(ExecutableReleaseObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(ReleaseObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking Release...'
	@$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(ExecutableReleaseObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(DISPLAYLIBS) -D cimg_use_openmp=1
	@printf 'Done'
	@printf '\n'

mpidebug : .mpidebug  begun $(MPIDebugObjectFiles) $(ExecutableMPIDebugObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(MPIDebugObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking MpiDebug...'
	@$(MPICXX) $(CXXFLAGS) $(DEBUGFLAGS) $(ExecutableMPIDebugObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(DISPLAYLIBS) $(DEBUGLIBS)
	@printf 'Done'
	@printf '\n'

mpirelease : .mpirelease begun $(MPIReleaseObjectFiles) $(ExecutableMPIReleaseObjectFile)	
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(MPIReleaseObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking MpiRelease...'
	@$(MPICXX) $(CXXFLAGS) $(RELEASEFLAGS) $(ExecutableMPIReleaseObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(DISPLAYLIBS) -D cimg_use_openmp=1
	@printf 'Done'
	@printf '\n'

windebug : .windebug  begun $(WINDebugObjectFiles) $(ExecutableWINDebugObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(WINDebugObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking WINDebug...'
	@$(WINCXX) $(CXXFLAGS) $(DEBUGFLAGS) $(ExecutableWINDebugObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(WINExecutable)) $(IMAGELIBS) $(WINLIBS) $(WINDISPLAYLIBS)
	@printf 'Done'
	@printf '\n'

winrelease : .winrelease begun $(WINReleaseObjectFiles) $(ExecutableWINReleaseObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(WINReleaseObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking WINRelease...'
	@$(WINCXX) $(CXXFLAGS) $(RELEASEFLAGS) $(ExecutableWINReleaseObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(WINExecutable)) $(IMAGELIBS) $(WINLIBS) $(WINDISPLAYLIBS) -D cimg_use_openmp=1
	@printf 'Done'
	@printf '\n'

androiddebug : .androiddebug  begun $(ANDROIDDebugObjectFiles) $(ExecutableANDROIDDebugObjectFile)
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(ANDROIDDebugObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking ANDROIDDebug...'
	@$(ANDROIDCXX) $(CXXFLAGS) $(DEBUGFLAGS) $(ExecutableANDROIDDebugObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(ANDROIDLIBS) $(DISPLAYLIBS)
	@printf 'Done'
	@printf '\n'

androidrelease : .androidrelease begun $(ANDROIDReleaseObjectFiles) $(ExecutableANDROIDReleaseObjectFile)	
	@printf '   Building Library...'
	@ar rcs $(addprefix lib/lib,$(current_dir)).a $(ANDROIDReleaseObjectFiles)
	@printf 'Done'
	@printf '\n'
	@printf '   Linking ANDROIDRelease...'
	@$(ANDROIDCXX) $(CXXFLAGS) $(RELEASEFLAGS) $(ExecutableANDROIDReleaseObjectFile) -L./lib -l$(current_dir) -o $(addprefix bin/,$(Executable)) $(IMAGELIBS) $(ANDROIDLIBS) $(DISPLAYLIBS) -D cimg_use_openmp=1
	@printf 'Done'
	@printf '\n'

reset : clean 
	@$(shell reset)

verify : release $(ReleaseObjectFiles)

#---------------------------------------------------------------------------------------------------+
#---------------------------------------------------------------------------------------+
# Pattern Rules

.debug/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $(DEBUGFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(DEBUGFLAGS) ' | ' $<' ... Done'

.release/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $(RELEASEFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(RELEASEFLAGS) ' | ' $<' ... Done '

.mpidebug/%.o : %.cpp
	@$(MPICXX) -c $(CXXFLAGS) $(DEBUGFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(DEBUGFLAGS) ' | ' $<' ... Done'

.mpirelease/%.o : %.cpp
	@$(MPICXX) -c $(CXXFLAGS) $(RELEASEFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(RELEASEFLAGS) ' | ' $<' ... Done '

.windebug/%.o : %.cpp
	@$(WINCXX) -c $(CXXFLAGS) $(DEBUGFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(DEBUGFLAGS) ' | ' $<' ... Done'

.winrelease/%.o : %.cpp
	@$(WINCXX) -c $(CXXFLAGS) $(RELEASEFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(RELEASEFLAGS) ' | ' $<' ... Done '

.androiddebug/%.o : %.cpp
	@$(ANDROIDCXX) -c $(CXXFLAGS) $(DEBUGFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(DEBUGFLAGS) ' | ' $<' ... Done'

.androidrelease/%.o : %.cpp
	@$(ANDROIDCXX) -c $(CXXFLAGS) $(RELEASEFLAGS)  -I$(subst $(space), -I,$(AllDirs)) $< -o $@ 
	@echo '   Pattern Rule | Compiling | '$(CXXFLAGS) $(RELEASEFLAGS) ' | ' $<' ... Done '

#---------------------------------------------------------------------------------------+
#--------------------------------------------------------------------------------+
# Phony Targets

.PHONY : clean cleandebug cleanrelease cleanmpidebug cleanmpirelease cleanwindebug cleanwinrelease cleanandroiddebug cleanandroidrelease begin end

begun :

clean : cleandebug cleanrelease cleanmpidebug cleanmpirelease cleanwindebug cleanwinrelease cleanandroiddebug cleanandroidrelease
	@-rm -rf $(AnyObjectFiles)
	@-rm -f bin/$(current_dir)
	@-rm -f $(addprefix lib/lib,$(current_dir)).a
	
cleandebug :
	@-rm -rf .debug .debugtimestamp
	@-rm -f $(addprefix .debug/,$(Executable))

cleanrelease :
	@-rm -rf .release .releasetimestamp
	@-rm -f $(addprefix .release/,$(Executable))

cleanmpidebug :
	@-rm -rf .mpidebug .mpidebugtimestamp
	@-rm -f $(addprefix .mpidebug/,$(Executable))

cleanmpirelease :
	@-rm -rf .mpirelease .mpireleasetimestamp
	@-rm -f $(addprefix .mpirelease/,$(Executable))

cleanwindebug :
	@-rm -rf .windebug .windebugtimestamp
	@-rm -f $(addprefix .windebug/,$(WINExecutable))

cleanwinrelease :
	@-rm -rf .winrelease .winreleasetimestamp
	@-rm -f $(addprefix .winrelease/,$(WINExecutable))

cleanandroiddebug :
	@-rm -rf .androiddebug .androiddebugtimestamp
	@-rm -f $(addprefix .androiddebug/,$(Executable))

cleanandroidrelease :
	@-rm -rf .androidrelease .androidreleasetimestamp
	@-rm -f $(addprefix .androidrelease/,$(Executable))

.debug : .debugtimestamp

.mpidebug : .mpidebugtimestamp

.windebug : .windebugtimestamp

.androiddebug : .androiddebugtimestamp

.debugtimestamp :
	@mkdir -p .debug $(DebugDirs)
	@mkdir -p bin
	@mkdir -p lib
#	@touch .debugtimestamp

.mpidebugtimestamp :
	@mkdir -p .mpidebug $(MPIDebugDirs)
	@mkdir -p bin
	@mkdir -p lib
#	@touch .mpidebugtimestamp

.windebugtimestamp :
	@mkdir -p .windebug $(WINDebugDirs)
	@mkdir -p bin
	@mkdir -p lib
#	@touch .windebugtimestamp

.androiddebugtimestamp :
	@mkdir -p .androiddebug $(ANDROIDDebugDirs)
	@mkdir -p bin
	@mkdir -p lib
#	@touch .androiddebugtimestamp

.release : .releasetimestamp

.mpirelease : .mpireleasetimestamp

.winrelease : .winreleasetimestamp

.androidrelease : .androidreleasetimestamp

.releasetimestamp :
	@mkdir -p .release $(ReleaseDirs)
	@mkdir -p bin
	@mkdir -p lib

.mpireleasetimestamp :
	@mkdir -p .mpirelease $(MPIReleaseDirs)
	@mkdir -p bin
	@mkdir -p lib

.winreleasetimestamp :
	@mkdir -p .winrelease $(WINReleaseDirs)
	@mkdir -p bin
	@mkdir -p lib

.androidreleasetimestamp :
	@mkdir -p .androidrelease $(ANDROIDReleaseDirs)
	@mkdir -p bin
	@mkdir -p lib

#--------------------------------------------------------------------------------+
#--------------------------------------+
# Information

sourcedirs :
	@printf '%s\n' $(SourceDirs)

sourcefiles :
	@printf '%s\n' $(SourceFiles)

activesourcefiles :
	@printf '%s\n' $(ActiveSourceFiles)

deactivatedfiles :
	@printf '%s\n' $(DeactivatedFiles)

debugobjectfiles :
	@printf '%s\n' $(DebugObjectFiles)

releaseobjectfiles :
	@printf '%s\n' $(ReleaseObjectFiles)

mpidebugobjectfiles :
	@printf '%s\n' $(MPIDebugObjectFiles)

mpireleaseobjectfiles :
	@printf '%s\n' $(MPIReleaseObjectFiles)

windebugobjectfiles :
	@printf '%s\n' $(WINDebugObjectFiles)

winreleaseobjectfiles :
	@printf '%s\n' $(WINReleaseObjectFiles)

androiddebugobjectfiles :
	@printf '%s\n' $(ANDROIDDebugObjectFiles)

androidreleaseobjectfiles :
	@printf '%s\n' $(ANDROIDReleaseObjectFiles)

os :
	@echo $(OS)

executable:
	@echo $(current_dir)
#--------------------------------------+