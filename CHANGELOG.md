## [0.0.9] - 2021-1-30
### Added
- kernel(Fat12): initializeFATDataAddress
- kernel(Fat12): isFileNamesEqual
- kernel(Fat12): getFileInformation
- kernel(Fat12): showFile
- libc(string): convertCharacterToLowerCase
### Modified
- kernel(Fat12): printFileName

## [0.0.8] - 2021-1-29
### Added
- kernel(Fat12): readRootEntriesTable
### Modified
- build.bat: move the imgwrt into boot, kernel

## [0.0.7] - 2021-1-28
### Added
- libc(conio): inPortByte
- libc(conio): inPortWord
- libc(conio): outPortByte
- libc(conio): outPortWord
- kernel(Fat12): basic implementation
- Adding kernel logger
### Fixed
- libc(string): movedata [data segment]
### Modified
- libc(conio): printFormat [output stream]
- libc(conio): printString [output stream]
- libc(conio): printCharacter [output stream]

## [0.0.6] - 2021-1-27
### Added
- Tool: Disk image writer

## [0.0.5] - 2021-1-5
### Added
- Fix boot drive (boot sector)
- BIOS disk I/O with LBA support

## [0.0.4] - 2021-1-2
### Added
- Kernel interrupt handler
- libc(vector): setInterruptVector
- libc(string): getInterruptVector

## [0.0.3] - 2020-12-31
### Added
- Kernel conventinal memory manager: kmalloc, kfree
- Turbo C helper functions
- libc(string): memset
- libc(string): movedata

## [0.0.2] - 2020-12-25
### Added
- libc(conio): setTextcolor
- libc(conio): setActivePage
- libc(conio): setCursorPosition
- libc(conio): getCursorPosition
- libc(conio): clearScreen
- libc(conio): printCharacter
- libc(conio): printString
- libc(conio): convertIntegerToString
- libc(conio): printFormat
- libc(conio): readCharacter
- libc(conio): readString

## [0.0.1] - 2020-10-02
### Added
- Support 16 bit Borland compiler/assembler tool chains
- Adding Turbo C helper functions
- Stage zero boot loader
- Makefile infrastructure
- Build batch
- Kernel entry
- Adding Bochs emulator configuration files
- Update README.md to reflect the tool-chains version
