ISO_IMAGE = disk.iso
GDB = gdb

.PHONY: all run clean distclean

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	qemu-system-x86_64 -M q35,smm=off -m 2G -cdrom $(ISO_IMAGE) -d int -no-reboot -no-shutdown 2> log.txt

debug: $(ISO_IMAGE) kernel/kernel.elf
	qemu-system-x86_64 -M q35,smm=off -m 2G -cdrom $(ISO_IMAGE) -s -S -d int -no-reboot -no-shutdown 2> log.txt & sleep 1
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel/kernel.elf"

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1
	make -C limine

kernel/kernel.elf:
	$(MAKE) -C kernel

$(ISO_IMAGE): limine kernel/kernel.elf
	rm -rf iso_root
	mkdir -p iso_root
	cp kernel/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(ISO_IMAGE)
	limine/limine-install $(ISO_IMAGE)
	rm -rf iso_root

clean:
	rm -f $(ISO_IMAGE)
	$(MAKE) -C kernel clean

distclean: clean
	rm -rf limine kernel/stivale2.h
