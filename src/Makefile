#
# Kurzanleitung
# =============
#
# make		-- Baut den Kernel.
# make all
#
# make install	-- Baut den Kernel und transferiert ihn auf den Server.
# 		   Das Board holt sich diesen Kernel beim nächsten Reset.
#
# make clean	-- Löscht alle erzeugten Dateien.
#

BINDIR = ../bin
ODIR = obj
LSCRIPT = kernel.lds
ULSCRIPT = user.lds

VPATH = user core cpu drivers sys

_KOBJ =  start_asm.o start.o
_KOBJ += exc.o exc_asm.o
_KOBJ += mc.o
_KOBJ += mem.o
_KOBJ += assert.o
_KOBJ += panic.o
_KOBJ += sched.o
_KOBJ += syscall.o syscall_asm.o
_KOBJ += task.o

_KOBJ += cpu.o cpu_asm.o
_KOBJ += mmu.o mmu_asm.o

_KOBJ += aic.o
_KOBJ += dbgu.o
_KOBJ += st.o

_KOBJ += list.o
_KOBJ += no.o
_KOBJ += ringbuf.o

KOBJ = $(patsubst %,$(ODIR)/%,$(_KOBJ))

_UOBJ = userlib.o userlib_asm.o
_UOBJ += user.o

UOBJ = $(patsubst %,$(ODIR)/%,$(_UOBJ))

OBJ = $(KOBJ) $(UOBJ)

#
# Konfiguration
#
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -Wextra -ffreestanding -std=gnu99 -mcpu=arm920t -O2
CPPFLAGS = -Iinclude -Iuser/include
#LIBGCC := $(shell $(CC) -print-libgcc-file-name)

LIBGCC := $(shell $(CC) \
-print-libgcc-file-name)

DEP = $(OBJ:.o=.d) $(LSCRIPT:.lds=.d)

#
# Regeln
#
.PHONY: all
all: kernel

-include $(DEP)

$(ODIR)/%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

$(ODIR)/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

# https://stackoverflow.com/questions/28837199/can-i-use-preprocessor-directives-in-ld-file
%.lds: %.lds.S
	$(CC) $(CPPFLAGS) -E $< | grep -v '^#' >$@

kernel: $(BINDIR)/kernel
$(BINDIR)/kernel: $(LSCRIPT) $(OBJ)
	$(LD) -T$(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

kernel.bin: $(BINDIR)/kernel.bin
$(BINDIR)/kernel.bin: $(BINDIR)/kernel
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

kernel.img: $(BINDIR)/kernel.img
$(BINDIR)/kernel.img: $(BINDIR)/kernel.bin
	mkimage -A arm -T standalone -C none -a 0x20000000 -d $< $@

.PHONY: install
install: kernel.img
	arm-install-image $<

.PHONY: clean
clean:
	rm -f $(BINDIR)/*
	rm -f $(OBJ)
	rm -f $(DEP)
	rm -f $(LSCRIPT)

.PHONY: kernelonly
kernelonly: $(LSCRIPT) $(KOBJ)
	$(LD) -T$(LSCRIPT) -o kernel $(KOBJ) $(LIBGCC)

.PHONY: useronly
useronly: $(ULSCRIPT) $(UOBJ)
	$(LD) -T$(ULSCRIPT) -o kernel $(UOBJ) $(LIBGCC)
