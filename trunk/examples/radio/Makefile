MAINDIR := ../..

DEPENDS=$(MAINDIR)/lib/radio.o

sender:
	$(MAKE) TARGET=sender DEPENDS=$(DEPENDS) all

receiver: receiver.hex

include $(MAINDIR)/Makefile.common
