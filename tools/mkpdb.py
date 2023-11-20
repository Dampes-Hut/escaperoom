
import struct, sys

class SectionHeader:
    def __init__(self, data):
        self.sh_name, self.sh_type, self.sh_flags, self.sh_addr, \
            self.sh_offset, self.sh_size, self.sh_link, self.sh_info, \
                self.sh_addralign, self.sh_entsize = struct.unpack(">IIIIIIIIII", data)

class Symbol:
    def __init__(self, data):
        self.st_name, self.st_value, self.st_size, self.st_info, \
            self.st_other, self.st_shndx = struct.unpack(">IIIBBH", data)

class ELF:
    def __init__(self, data) -> None:
        self.data : memoryview = data

        # Header

        self.e_ident = data[0:16]

        assert self.e_ident[0:4] == b"\x7FELF"
        assert self.e_ident[4] == 1
        assert self.e_ident[5] == 2

        self.e_type, self.e_machine, self.e_version, self.e_entry, \
            self.e_phoff, self.e_shoff, self.e_flags, self.e_ehsize, \
                self.e_phentsize, self.e_phnum, self.e_shentsize, self.e_shnum, \
                    self.e_shstrndx = struct.unpack(">HHIIIIIHHHHHH", data[16:52])

        assert self.e_shentsize == 0x28

        self.shstrtab = self.section_foridx(self.e_shstrndx)

        self.symtab = None
        self.strtab = None

        for i in range(self.e_shnum):
            section = self.section_foridx(i)

            if section.sh_type == 2:
                assert self.symtab is None
                self.symtab = section
                continue

            name = self.get_string(self.shstrtab, section.sh_name)
            if name == b".strtab":
                assert self.strtab is None
                self.strtab = section
                continue

        assert self.symtab is not None
        assert self.strtab is not None

    def get_string(self, str_section : SectionHeader, offset : int):
        start = str_section.sh_offset + offset
        end = start
        while self.data[end] != 0:
            end += 1
        return bytes(self.data[start:end])

    def section_foridx(self, i):
        return SectionHeader(self.data[self.e_shoff + i * 0x28:][:0x28])

class SymInfo:
    def __init__(self, start, end, name_offset, name):
        self.start, self.end, self.name_offset = start, end, name_offset
        self.name = name

if __name__ == '__main__':
    ELF_PATH = sys.argv[1]
    PDB_PATH = sys.argv[2]

    with open(ELF_PATH, "rb") as infile:
        elf = ELF(memoryview(infile.read()))

    print("Collecting symbols")

    sym_info = []
    str_data = b""

    start = elf.symtab.sh_offset
    end = start + elf.symtab.sh_size

    nsyms = 0
    while start < end:
        st_info = elf.data[start+12]
        if (st_info & 0xF) == 2:
            st_name, st_value, st_size = struct.unpack(">III", elf.data[start:start+12])
            name = elf.get_string(elf.strtab, st_name)

            sym_info.append(SymInfo(st_value, st_value + st_size, len(str_data), name))

            str_data += name[:59] + b"\0" # ensure null-terminated and max length 60 chars
            while len(str_data) % 4 != 0: # pad to 4 byte boundary
                str_data += b"\0"

            nsyms += 1
        start += 0x10

    print("Sorting symbols")

    sym_info.sort(key=lambda inf : inf.start)

    # for inf in sym_info:
    #     print(f"0x{inf.start:08X} 0x{inf.end:08X} {inf.name.decode("latin1")}")

    print("Assembling pdb")

    sym_data = b"".join(struct.pack(">III", inf.start, inf.end, inf.name_offset) for inf in sym_info)

    all_data = struct.pack(">II", 0xFFFF5A5A, nsyms) + sym_data + str_data
    
    final_len = len(all_data)
    next_mb = (final_len + 0x100000-1) & ~(0x100000-1)

    padded_data = all_data + b"\0" * (next_mb - final_len)

    with open(PDB_PATH, "wb") as outfile:
        outfile.write(padded_data)

    print(f"Wrote {nsyms} syms. PDB size (before padding) = 0x{final_len:X}")
