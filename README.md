# NOS
Simple Operating System for x86


## Compiling

NOS has been developed in DOS (DOSBox 0.74). The Borland tool chains
has been used to compile the source code including (tcc, tlink, tasm and tdstrip).

To build all binaries:
- set_path.bat
- build.bat

Note: You will find the binaries in build folder.

## Creating floppy image
As version 0.0.1 dosn't have file system yet to read kernel file from disk. I reserved
32 sector for kernel in hidden area of FAT. So to create floppy image that comply with
our assumption, type the following command (note: this is NIX command):
```
mkfs.msdos -h 32 -R 32 ./floppya.img
```

You have to copy boot.bin to floppya.img at offset 0. Then you have to copy kernel.bin
at offset 0x200 (512)

Note: you can use Hex editors to achive this task. (I will provide a utility in the future)

## Run using Bochs emulator

To run the floppy image, type the folowing command:
```
bochs -q -f emulator\bochsrc.bxrc
```
Note: you can use BXSHARE to set BIOS/VGA path...
