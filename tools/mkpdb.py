import dataclasses
import mmap
from pathlib import Path
import struct
import sys
from typing import List

class SectionHeader:
    STRUCT = struct.Struct(">IIIIIIIIII")

    def __init__(self, data):
        (
            self.sh_name,
            self.sh_type,
            self.sh_flags,
            self.sh_addr,
            self.sh_offset,
            self.sh_size,
            self.sh_link,
            self.sh_info,
            self.sh_addralign,
            self.sh_entsize,
        ) = self.STRUCT.unpack(data)


class Symbol:
    STRUCT = struct.Struct(">IIIBBH")

    def __init__(self, data):
        (
            self.st_name,
            self.st_value,
            self.st_size,
            self.st_info,
            self.st_other,
            self.st_shndx,
        ) = self.STRUCT.unpack(data)


class ELF:
    def __init__(self, data: mmap.mmap) -> None:
        self.data = data

        # Header

        self.e_ident = data[0:16]

        assert self.e_ident[0:4] == b"\x7FELF"
        assert self.e_ident[4] == 1
        assert self.e_ident[5] == 2

        (
            self.e_type,
            self.e_machine,
            self.e_version,
            self.e_entry,
            self.e_phoff,
            self.e_shoff,
            self.e_flags,
            self.e_ehsize,
            self.e_phentsize,
            self.e_phnum,
            self.e_shentsize,
            self.e_shnum,
            self.e_shstrndx,
        ) = struct.unpack(">HHIIIIIHHHHHH", data[16:52])

        assert self.e_shentsize == 0x28

        self.shstrtab = self.section_foridx(self.e_shstrndx)

        symtab = None
        strtab = None

        for i in range(self.e_shnum):
            section = self.section_foridx(i)

            if section.sh_type == 2:
                assert symtab is None
                symtab = section
                continue

            name = self.get_string(self.shstrtab, section.sh_name)
            if name == b".strtab":
                assert strtab is None
                strtab = section
                continue

        assert symtab is not None
        assert strtab is not None

        self.symtab = symtab
        self.strtab = strtab

    def get_string(self, str_section: SectionHeader, offset: int) -> memoryview:
        start = str_section.sh_offset + offset
        end = self.data.find(b"\0", start)
        if end == -1:
            raise Exception(f"Did not find any NUL byte starting at {start:#X}")
        return self.data[start:end]

    def section_foridx(self, i):
        off = self.e_shoff + i * 0x28
        return SectionHeader(self.data[off : off + 0x28])


@dataclasses.dataclass
class SymInfo:
    start: int
    end: int
    name_offset: int
    name: memoryview


def main():
    elf_path = Path(sys.argv[1])
    pdb_path = Path(sys.argv[2])

    with elf_path.open("rb") as f_elf:
        elf_data = mmap.mmap(f_elf.fileno(), 0, access=mmap.ACCESS_READ)
        main_wrapped(elf_data, pdb_path)


STRUCT_BE_III = struct.Struct(">III")


def collect_symbols(elf: ELF):
    sym_info: List[SymInfo] = []
    str_data = bytearray()

    start = elf.symtab.sh_offset
    end = start + elf.symtab.sh_size

    while start < end:
        st_info = elf.data[start + 12]
        if (st_info & 0xF) == 2:
            st_name, st_value, st_size = STRUCT_BE_III.unpack(
                elf.data[start : start + 12]
            )
            name = elf.get_string(elf.strtab, st_name)

            sym_info.append(SymInfo(st_value, st_value + st_size, len(str_data), name))

            # max length 60 chars
            str_data += name[:59]
            # NUL-terminate
            str_data += b"\0"

            # pad to 4 byte boundary
            while len(str_data) % 4 != 0:
                str_data += b"\0"

        start += 0x10

    return sym_info, str_data


def assemble_pdb(sym_info: List[SymInfo], str_data: bytearray):
    sym_data = b"".join(
        STRUCT_BE_III.pack(inf.start, inf.end, inf.name_offset) for inf in sym_info
    )

    all_data = b"".join(
        (
            struct.pack(">II", 0xFFFF5A5A, len(sym_info)),
            sym_data,
            str_data,
        )
    )

    final_len = len(all_data)
    next_MiB = (final_len + 0x100000 - 1) % 0x100000

    padded_data = all_data + b"\0" * (next_MiB - final_len)

    return final_len, padded_data


def main_wrapped(elf_data: mmap.mmap, pdb_path: Path):
    elf = ELF(elf_data)

    print("Collecting symbols")
    sym_info, str_data = collect_symbols(elf)

    print("Sorting symbols")
    sym_info.sort(key=lambda inf: inf.start)

    print("Assembling pdb")
    final_len, padded_data = assemble_pdb(sym_info, str_data)

    pdb_path.write_bytes(padded_data)

    print(f"Wrote {len(sym_info)} syms. PDB size (before padding) = 0x{final_len:X}")


if __name__ == "__main__":
    main()
