#!/usr/bin/env python3

from edid import Edid
import sys

if __name__ == '__main__':
    edid = Edid.from_file(sys.argv[1])

    print("Checksum: {} ".format(edid.checksum), end='')
    calcChecksum = 0
    for byte in edid.data:
        calcChecksum += byte
    calcChecksum &= 0xFF

    if calcChecksum + edid.checksum != 256:
        print("Bad, should be {}".format(256 - calcChecksum))
    else:
        print("Good")

    print("*** Header ***")
    print("Manufacturer ID: ", end='')
    for letter in edid.header.manu_id.letter:
        print(letter.name.upper(), end='')
    print()
    print("Product Code: {:04X}".format(edid.header.product_code))
    print("Serial: {:08X}".format(edid.header.serial))
    print("Week: {}".format(edid.header.week))
    if edid.header.year:
        print("Year: {}".format(edid.header.year))
    if edid.header.model_year:
        print("Model Year: {}".format(edid.header.model_year))
    print("EDID Version: {}.{}".format(edid.header.vermaj, edid.header.vermin))

    print()
    print("*** Basic Parameters ***")
    print("Input Type: {}".format(edid.basic_params.input_type.name))
    if edid.basic_params.input_type == Edid.InputType.analog:
        levelsStr = edid.basic_params.video_levels.name[1:]
        whiteLevel = "+{}.{}".format(levelsStr[0], levelsStr[1:3])
        syncLevel = "{}{}.{}".format('+' if levelsStr[5] != '0' else '-',
            levelsStr[4], levelsStr[5:])
        print("Video Levels, relative to blank: White: ", whiteLevel, " Sync: ", syncLevel)
        if edid.basic_params.black_to_black:
            print("Blank-to-black setup (pedestal) expected")
        if edid.basic_params.separate_sync:
            print("Separate sync supported")
        if edid.basic_params.composite_sync:
            print("Composite sync (on HSync) supported")
        if edid.basic_params.sync_on_green:
            print("Sync on green supported")
        if edid.basic_params.serrated_vsync:
            print("VSync pulse must be serrated when composite or sync-on-green is used.")
    elif edid.basic_params.input_type == Edid.InputType.digital:
        print("Bit Depth: {}".format(edid.basic_params.bit_depth.name[5:]))
        print("Video Interface: {}".format(edid.basic_params.video_interface.name))
    if edid.basic_params.width and edid.basic_params.height:
        print("Width: {} cm".format(edid.basic_params.width))
        print("Height: {} cm".format(edid.basic_params.height))
    print("Aspect: {:.3}:1".format(edid.basic_params.aspect))
