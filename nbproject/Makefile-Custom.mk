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
CC=avr-gcc
CCC=avr-g++
CXX=avr-g++
FC=gfortran
AS=avr-as

# Macros
CND_PLATFORM=AVR-Linux
CND_DLIB_EXT=so
CND_CONF=Custom
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include /home/dode/dev/avrrfm/Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/48b9ad18/avrrfm.o \
	${OBJECTDIR}/_ext/48b9ad18/bitmaps.o \
	${OBJECTDIR}/_ext/48b9ad18/colorspace.o \
	${OBJECTDIR}/_ext/48b9ad18/dejavu.o \
	${OBJECTDIR}/_ext/48b9ad18/display.o \
	${OBJECTDIR}/_ext/48b9ad18/font.o \
	${OBJECTDIR}/_ext/48b9ad18/i2c.o \
	${OBJECTDIR}/_ext/48b9ad18/mcp9808.o \
	${OBJECTDIR}/_ext/48b9ad18/rfm.o \
	${OBJECTDIR}/_ext/48b9ad18/sdc.o \
	${OBJECTDIR}/_ext/48b9ad18/spi.o \
	${OBJECTDIR}/_ext/48b9ad18/tft.o \
	${OBJECTDIR}/_ext/48b9ad18/unifont.o \
	${OBJECTDIR}/_ext/48b9ad18/usart.o \
	${OBJECTDIR}/librfm69.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avrrfm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avrrfm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avrrfm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/48b9ad18/avrrfm.o: /home/dode/dev/avrrfm/avrrfm.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -std=c99 -o ${OBJECTDIR}/_ext/48b9ad18/avrrfm.o /home/dode/dev/avrrfm/avrrfm.c

${OBJECTDIR}/_ext/48b9ad18/bitmaps.o: /home/dode/dev/avrrfm/bitmaps.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/bitmaps.o /home/dode/dev/avrrfm/bitmaps.c

${OBJECTDIR}/_ext/48b9ad18/colorspace.o: /home/dode/dev/avrrfm/colorspace.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/colorspace.o /home/dode/dev/avrrfm/colorspace.c

${OBJECTDIR}/_ext/48b9ad18/dejavu.o: /home/dode/dev/avrrfm/dejavu.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/dejavu.o /home/dode/dev/avrrfm/dejavu.c

${OBJECTDIR}/_ext/48b9ad18/display.o: /home/dode/dev/avrrfm/display.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/display.o /home/dode/dev/avrrfm/display.c

${OBJECTDIR}/_ext/48b9ad18/font.o: /home/dode/dev/avrrfm/font.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/font.o /home/dode/dev/avrrfm/font.c

${OBJECTDIR}/_ext/48b9ad18/i2c.o: /home/dode/dev/avrrfm/i2c.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/i2c.o /home/dode/dev/avrrfm/i2c.c

${OBJECTDIR}/_ext/48b9ad18/mcp9808.o: /home/dode/dev/avrrfm/mcp9808.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/mcp9808.o /home/dode/dev/avrrfm/mcp9808.c

${OBJECTDIR}/_ext/48b9ad18/rfm.o: /home/dode/dev/avrrfm/rfm.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/rfm.o /home/dode/dev/avrrfm/rfm.c

${OBJECTDIR}/_ext/48b9ad18/sdc.o: /home/dode/dev/avrrfm/sdc.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/sdc.o /home/dode/dev/avrrfm/sdc.c

${OBJECTDIR}/_ext/48b9ad18/spi.o: /home/dode/dev/avrrfm/spi.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -std=c99 -o ${OBJECTDIR}/_ext/48b9ad18/spi.o /home/dode/dev/avrrfm/spi.c

${OBJECTDIR}/_ext/48b9ad18/tft.o: /home/dode/dev/avrrfm/tft.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/tft.o /home/dode/dev/avrrfm/tft.c

${OBJECTDIR}/_ext/48b9ad18/unifont.o: /home/dode/dev/avrrfm/unifont.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/unifont.o /home/dode/dev/avrrfm/unifont.c

${OBJECTDIR}/_ext/48b9ad18/usart.o: /home/dode/dev/avrrfm/usart.c
	${MKDIR} -p ${OBJECTDIR}/_ext/48b9ad18
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/_ext/48b9ad18/usart.o /home/dode/dev/avrrfm/usart.c

${OBJECTDIR}/librfm69.o: librfm69.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -g -DBAUD=38400 -DDRIVER=1 -DF_CPU=16000000UL -D__AVR_ATmega328P__ -D__flash=volatile -I. -o ${OBJECTDIR}/librfm69.o librfm69.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
