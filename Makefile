include ../../Makefile.cfg

all:
	mkdir -p cd_root
	$(CC) $(CCFLAGS)-DEXAMPLES_VMODE=VMODE_PAL buttonmash.c -o buttonmash.elf
	elf2exe buttonmash.elf buttonmash.exe
	cp buttonmash.exe cd_root
	systemcnf buttonmash.exe > cd_root/system.cnf
	mkisofs.exe -o buttonmash.hsf -V ButtonMasher -sysid PLAYSTATION cd_root
	mkpsxiso buttonmash.hsf buttonmash.bin $(CDLIC_FILE)