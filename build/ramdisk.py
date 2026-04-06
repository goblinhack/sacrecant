#!/usr/bin/env python3
#
# Why a ramdisk ? Oryx tiles request that the images are hidden reasonably,
# so this is my attempt; tarring the images during build and in the release
# adding the images into the binary itself, so we never ship the raw images.
#

import collections
import pathlib
import re
import os
import sys
import subprocess


def is_old_clang_version():

    if os.name == 'nt':
        return False

    check_clang = "clang++ --version | head -1 | sed -e 's/ (.*//g' -e 's/.* //g' -e 's/\\..*//g'"
    try:
        result = subprocess.check_output(check_clang,
                                         shell=True,
                                         executable="/bin/bash",
                                         stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as cpe:
        result = cpe.output
    finally:
        for line in result.splitlines():
            version = int(line.decode())
            if (version < 21):
                return True
            else:
                return False


number_of_files_to_add_to_ramdisk = 0
number_of_ramdisk_files = 10

root = pathlib.Path(".")

if sys.platform == 'win32':
    target = "sacrecant.exe"
else:
    target = "sacrecant"

#
# If the make target exists, see if any graphics are newer than the
# target. If so we need to rebuild the ramdisk.
#
try:
    with open(target) as unused:
        target_ctime = os.path.getctime(target)
        newer = False
        for filepath in root.rglob(r"data/gfx.tgz"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/sounds.tgz"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/fonts/*.tga"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/gfx/*.tga"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/gfx/*/*.tga"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/gfx/*/*/*.tga"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/sounds/*/*/.wav"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break

        for filepath in root.rglob(r"data/music/*.ogg"):
            if target_ctime < os.path.getctime(filepath):
                print("{} is newer".format(filepath))
                newer = True
                break
        #
        # If nothing is newer than the target, the ramdisk is up to date
        #
        if not newer:
            print("Ramdisk is up to date")
            sys.exit(0)
except IOError:
    pass

files = collections.defaultdict(list)

#
# For each .tga convert it to assembly to be included in the build
#
for filepath in root.rglob(r"data/*/*.tga"):
    files[filepath.parent].append(filepath.name)
    number_of_files_to_add_to_ramdisk += 1

for filepath in root.rglob(r"data/*/*/*.tga"):
    files[filepath.parent].append(filepath.name)
    number_of_files_to_add_to_ramdisk += 1

for filepath in root.rglob(r"data/*/*/*/*.tga"):
    files[filepath.parent].append(filepath.name)
    number_of_files_to_add_to_ramdisk += 1

for filepath in root.rglob(r"data/sounds/*/*.wav"):
    files[filepath.parent].append(filepath.name)
    number_of_files_to_add_to_ramdisk += 1

# for filepath in root.rglob(r"data/music/*.ogg"):
#     files[filepath.parent].append(filepath.name)
#     number_of_files_to_add_to_ramdisk += 1

if is_old_clang_version():
    for ram_file in range(number_of_ramdisk_files):
        with open("src/ramdisk_data_{}.S".format(ram_file), "w") as myfile:
            myfile.write("// Ramdisk data for files that due to licensing cannot be exposed\n\n")

    asm_count = 0
    for record_number, (folder, filenames) in enumerate(sorted(files.items())):
        for filename in enumerate(filenames):
            ram_file = asm_count % number_of_ramdisk_files
            asm_count += 1

            rec, orig_filename = filename
            c_filename = re.sub(r'[\-\.]', "_", orig_filename)
            rel_path_filename = os.path.join(folder, orig_filename)

            print("Write src/ramdisk_data_{}.S".format(ram_file))
            with open("src/ramdisk_data_{}.S".format(ram_file), "a") as myfile:
                myfile.write(".align 4\n")
                myfile.write(".globl data_{}_start_\n".format(c_filename))
                myfile.write("data_{}_start_:\n".format(c_filename))
                myfile.write('.incbin "../{}"\n'.format(rel_path_filename))
                myfile.write(".align 4\n")
                myfile.write(".globl data_{}_end_\n".format(c_filename))
                myfile.write("data_{}_end_:\n".format(c_filename))
                myfile.write("\n")

    #
    # This weird thing appears harmless but is a warning. Not sure how to check for it.
    #
    if sys.platform == "linux":
        for ram_file in range(number_of_ramdisk_files):
            with open("src/ramdisk_data_{}.S".format(ram_file), "a") as myfile:
                myfile.write("\n.section .note.GNU-stack,\"\",%progbits\n")

#
# This file references the assembly above to allow for easy loading of
# the files
#
with open("src/ramdisk_data.cpp", "w") as myfile:
    print("Write src/ramdisk_data.cpp")
    myfile.write('#include "my_ramdisk.hpp"')
    myfile.write("\n")
    myfile.write("\n")
    myfile.write("void ramdisk_init (void)\n")
    myfile.write("{\n")

    cpp_count = 0
    for record_number, (folder, filenames) in enumerate(sorted(files.items())):
        for filename in enumerate(filenames):
            ram_file = cpp_count % number_of_ramdisk_files
            cpp_count += 1

            rec, orig_filename = filename
            c_filename = re.sub(r'[\-\.]', "_", orig_filename)
            rel_path_filename = os.path.join(folder, orig_filename)

            myfile.write("    {\n")
            myfile.write("#ifdef __has_embed\n")
            myfile.write("        static const uint8_t start [] = {{\n#embed \"../{}\"\n}};\n".format(rel_path_filename))
            myfile.write("        ramdisk_t r;\n")
            myfile.write("        r.data = start;\n")
            myfile.write("        r.len = sizeof(start);\n")
            myfile.write("#else\n")
            myfile.write('        extern uint8_t *data_{}_start_\n           asm("data_{}_start_");\n'.format(c_filename, c_filename))
            myfile.write('        extern uint8_t *data_{}_end_\n           asm("data_{}_end_");\n'.format(c_filename, c_filename))
            myfile.write("        static const uint8_t *const start =\n           (const uint8_t *const) (char*)&data_{}_start_;\n".format(c_filename))
            myfile.write("        static const uint8_t *const end   =\n           (const uint8_t *const) (char*)&data_{}_end_;\n".format(c_filename))
            myfile.write("        ramdisk_t r;\n")
            myfile.write("        r.data = start;\n")
            myfile.write("        r.len = end - start;\n")
            myfile.write("#endif\n")
            myfile.write('        ramdisk_data["{}"] = r;\n'.format(rel_path_filename))
            myfile.write("    }\n")
            myfile.write("\n")

    myfile.write("}\n")
