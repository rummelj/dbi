#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/bufferManager/BufferFrame.o \
	${OBJECTDIR}/bufferManager/BufferManager.o \
	${OBJECTDIR}/bufferManager/PageFileManager.o \
	${OBJECTDIR}/extSort/ExternalSorter.o \
	${OBJECTDIR}/extSort/RunDescriptor.o \
	${OBJECTDIR}/extSort/SortQueue.o \
	${OBJECTDIR}/extSort/file_descriptor.o \
	${OBJECTDIR}/extSort/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sort

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sort: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sort ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/bufferManager/BufferFrame.o: bufferManager/BufferFrame.cpp 
	${MKDIR} -p ${OBJECTDIR}/bufferManager
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bufferManager/BufferFrame.o bufferManager/BufferFrame.cpp

${OBJECTDIR}/bufferManager/BufferManager.o: bufferManager/BufferManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/bufferManager
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bufferManager/BufferManager.o bufferManager/BufferManager.cpp

${OBJECTDIR}/bufferManager/PageFileManager.o: bufferManager/PageFileManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/bufferManager
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bufferManager/PageFileManager.o bufferManager/PageFileManager.cpp

${OBJECTDIR}/extSort/ExternalSorter.o: extSort/ExternalSorter.cpp 
	${MKDIR} -p ${OBJECTDIR}/extSort
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/extSort/ExternalSorter.o extSort/ExternalSorter.cpp

${OBJECTDIR}/extSort/RunDescriptor.o: extSort/RunDescriptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/extSort
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/extSort/RunDescriptor.o extSort/RunDescriptor.cpp

${OBJECTDIR}/extSort/SortQueue.o: extSort/SortQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}/extSort
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/extSort/SortQueue.o extSort/SortQueue.cpp

${OBJECTDIR}/extSort/file_descriptor.o: extSort/file_descriptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/extSort
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/extSort/file_descriptor.o extSort/file_descriptor.cpp

${OBJECTDIR}/extSort/main.o: extSort/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/extSort
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/extSort/main.o extSort/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sort

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
