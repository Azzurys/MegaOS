add_rules("mode.debug", "mode.release")


toolchain("cross-toolchain")
    set_kind("standalone")
    set_sdkdir("~/cross-compiler/")
    set_bindir("~/cross-compiler/bin/")
    set_toolset("cxx", "x86_64-elf-g++")
    set_toolset("ld", "x86_64-elf-ld")
    set_toolset("as", "nasm")
toolchain_end()


target("MegaOS")
    set_kind("binary")
    set_targetdir("build/")
    set_toolchains("cross-toolchain")

    add_files("kernel/**.cpp", "kernel/**.asm")
    add_headerfiles("kernel/**.hpp")
    add_includedirs("kernel/", "./")

    add_defines(
        "PRINTF_DISABLE_SUPPORT_FLOAT",
        "PRINTF_DISABLE_SUPPORT_EXPONENTIAL"
    )

    add_cxxflags(
        "-Wall",
        "-Wextra",
        "-Wshadow",
        "-Wcast-align",
        "-Wunused",
        "-Wpedantic",
        "-Wconversion",
        "-pipe",
        "-ffreestanding",
        "-fno-stack-protector",
        "-fno-stack-protector",
        "-fno-pic",
        "-fpie",
        "-fno-exceptions",
        "-fno-rtti",
        "-mno-80387",
        "-mno-mmx",
        "-mno-3dnow",
        "-mno-sse",
        "-mno-sse2",
        "-mno-red-zone",
        {force = true}
    )

    add_ldflags(
        "-Tkernel/linker.ld",
        "-nostdlib",
        "-zmax-page-size=0x1000",
        "-static",
        "-pie",
        "--no-dynamic-linker",
        "-ztext",
        {force = true}
    )

    add_asflags("-f elf64", {force = true})

    if is_mode("debug") then
        add_cxxflags("-ggdb", {force = true})
        add_asflags("-F dwarf -g", {force = true})
    else
        add_defines("USE_SERIAL_LOGGING")
    end

    after_build(
    function (target)
        os.run("rm -rf iso_root")
        os.run("mkdir -p iso_root")
        os.run("cp build/MegaOS limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin iso_root/")
    end
    )

    before_run(
    function (target)
        os.run("xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin --efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o build/disk.iso")
        os.run("limine/limine-install build/disk.iso")
        os.run("rm -rf iso_root")
    end
    )

    on_run(
    function (target)
        import("core.base.process")

        if is_mode("debug") then
            local proc1 = process.open(
                "qemu-system-x86_64 -M q35,smm=off -m 256M -cdrom build/disk.iso -s -S -d int -no-reboot -no-shutdown",
                { stderr="log.txt" }
            )

            os.run("sleep 1")

            local proc2 = process.openv(
                "gdb",
                {"-ex", "target remote localhost:1234", "-ex", "symbol-file build/MegaOS", "-ex", "b _start", "-ex", "c"}
            )

            proc1:wait()
            proc2:wait()
        else
            os.exec("qemu-system-x86_64 -M q35,smm=off -m 256M -cdrom build/disk.iso -no-reboot -no-shutdown -serial stdio")
        end
    end
    )

    after_clean(
    function (target)
        os.rm("build/disk.iso")
    end
    )