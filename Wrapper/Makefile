#Make file to generate the .h file and the shared library
#author N Krishna

HDRS=jni.h
JNI_PATH=$(JAVA_HOME)/include
INCLUDES=-I$(JNI_PATH) -I$(JNI_PATH)/linux -I$(SWIFT_HOME)/include/
LIBS=$(SWIFT_HOME)/lib/libswift.so $(SWIFT_HOME)/lib/libceplex_us.so $(SWIFT_HOME)/lib/libceplang_en.so
CC=g++
OBJS= JNICepstralWrapper


all: $(OBJS)

JNICepstralWrapper: cepstral_JNICepstral.h JNICepstralWrapper.cpp
	$(CC) -shared $(INCLUDES) JNICepstralWrapper.cpp \
        cepstral_JNICepstral.h $(LIBS) -lm -o libJNICepstralWrapper.so

clean:
	rm -f libJNICepstralWrapper.so

