MAINDIR := ../..

DEPENDS=$(MAINDIR)/lib/serial.o $(MAINDIR)/lib/radio.o
TARGET=main

main:
	@$(MAKE) TARGET=$(TARGET) DEPENDS="$(DEPENDS)" all

include $(MAINDIR)/Makefile.common
