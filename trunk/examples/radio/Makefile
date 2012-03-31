MAINDIR := ../..

DEPENDS=$(MAINDIR)/lib/radio.o

sender:
	@$(MAKE) TARGET=sender DEPENDS=$(DEPENDS) all

receiver:
	@$(MAKE) TARGET=receiver DEPENDS=$(DEPENDS) all

include $(MAINDIR)/Makefile.common
