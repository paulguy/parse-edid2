#!/usr/bin/env python3

from edid import Edid
from pkg_resources import parse_version
import sys

def aspectFromTimingAspect(timingAspect, vermaj, vermin):
    aspect = 1.0
    if not (parse_version("{}.{}".format(vermaj, vermin)) < parse_version('1.3') and timingAspect == Edid.TimingAspect.aspect1610):
        aspect = int(timingAspect.name[6:8]) / int(timingAspect.name[8:])
    return aspect

def printTwoByteTiming(timing, vermaj, vermin):
    if not timing.invalid:
        aspect = aspectFromTimingAspect(timing.aspect, vermaj, vermin)
        print("{}x{}@{}hz".format(timing.width, int(timing.width / aspect), timing.rate))

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
    print("Input Type:", edid.basic_params.input_type.name)
    if edid.basic_params.input_type == Edid.InputType.analog:
        print("Display type:", edid.basic_params.analog_display_type.name.replace('_', ' '))
        levelsStr = edid.basic_params.video_levels.name[1:]
        whiteLevel = "+{}.{}".format(levelsStr[0], levelsStr[1:3])
        syncLevel = "{}{}.{}".format('' if levelsStr[5] != '0' else '-',
            levelsStr[4], levelsStr[5:])
        print("Video Levels, relative to blank: White: {}, Sync: {}".format(whiteLevel, syncLevel))
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
        print("Display type:", edid.basic_params.digital_display_type.name.replace('_', ', '))
        print("Bit Depth:", edid.basic_params.bit_depth.name[5:])
        print("Video Interface:", edid.basic_params.video_interface.name)
    if edid.basic_params.width and edid.basic_params.height:
        print("Width: {} cm".format(edid.basic_params.width))
        print("Height: {} cm".format(edid.basic_params.height))
    print("Aspect: {:.3}:1".format(edid.basic_params.aspect))
    print("Gamma: {:.2}".format(edid.basic_params.gamma))
    if edid.basic_params.dpms_standby:
        print("DPMS Standby")
    if edid.basic_params.dpms_suspend:
        print("DPMS Suspend")
    if edid.basic_params.dpms_active_off:
        print("DPMS Active Off")

    print()
    print("*** Chromaticity ***")
    print("Red Point: ({}, {})".format(edid.chromaticity.red_x, edid.chromaticity.red_y))
    print("Green Point: ({}, {})".format(edid.chromaticity.green_x, edid.chromaticity.green_y))
    print("Blue Point: ({}, {})".format(edid.chromaticity.blue_x, edid.chromaticity.blue_y))
    print("White Point: ({}, {})".format(edid.chromaticity.white_x, edid.chromaticity.white_y))

    print()
    print("*** Established Modes ***")
    for mode in edid.modes.__dict__.keys():
        if mode[:4] == 'mode':
            if edid.modes.__dict__[mode] == True:
                print("{}x{}@{}Hz".format(int(mode[4:8]), int(mode[8:12]), int(mode[12:])))

    print()
    print("*** Two-Byte Timings ***")
    for timing in edid.two_byte_timings.two_byte_timing:
        printTwoByteTiming(timing, edid.header.vermaj, edid.header.vermin)

    print()
    print("*** Descriptor Blocks ***")
    for desc in enumerate(edid.descriptor_blocks.descriptor_block):
        if desc[1].is_dtd:
            print("** {}: Detailed Timing Descriptor **".format(desc[0]))
            dtd = desc[1].dtd
            print("Pixel Clock: {} Mhz".format(dtd.pixel_clock))
            print("Horizontal Active Pixels: {}".format(dtd.hactive))
            print("Horizontal Blanking Pixels: {}".format(dtd.hblank))
            print("Vertical Active Lines: {}".format(dtd.vactive))
            print("Vertical Blanking Lines: {}".format(dtd.vblank))
            print("Horizontal Front Porch Pixels: {}".format(dtd.hfront_porch))
            print("Horizontal Sync Pulse Pixels: {}".format(dtd.hsync_pulse))
            print("Vertical Front Porch Lines: {}".format(dtd.vfront_porch))
            print("Vertical Sync Pulse Lines: {}".format(dtd.vsync_pulse))
            print("Width: {} mm".format(dtd.width))
            print("Height: {} mm".format(dtd.height))
            print("Horizontal Border Pixels: {}".format(dtd.hborder))
            print("Vertical Border Lines: {}".format(dtd.vborder))
            if dtd.interlaced:
                print("Interlaced")
            print("Stereo Mode:", dtd.stereo_mode.name)
            print("Sync Signal Type:", dtd.sync_signal_type.name)
            if dtd.sync_signal_type == Edid.SyncSignalType.analog:
                print("Analog Sync Type:", dtd.analog_sync_type.name)
                if dtd.analog_vsync_serration:
                    print("VSync serration (HSync during VSync)")
                if dtd.sync_on_all:
                    print("Sync on all 3 RGB lines (else green only)")
            else:
                print("Digital Sync Type:", dtd.digital_sync_type.name)
                if dtd.digital_sync_type == Edid.DigitalSyncType.composite:
                    print("Vertical Sync Polarity:", dtd.digital_vsync_polarity.name)
                else:
                    if dtd.digital_vsync_serration:
                        print("VSync serration (HSync during VSync)")
                    print("Horizontal Sync Polarity:", dtd.digital_hsync_polarity.name)
            print()
        else: # not a DTD
            num = desc[0]
            if desc[1].desc.type == Edid.DescType.additional_standard_timings:
                desc = desc[1].desc.additional_standard_timings
                print("** {}: Additional Standard Timings **".format(num))
                for mode in desc.__dict__.keys():
                    if mode[:4] == 'mode':
                        if desc.__dict__[mode] == True:
                            print("{}x{}@{}Hz".format(int(mode[4:8]), int(mode[8:12]), int(mode[12:])))
                print()
            elif desc[1].desc.type == Edid.DescType.three_byte_timings:
                desc = desc[1].desc.three_byte_timings
                print("** {}: CVT Three-Byte Timings **".format(num))
                print("Version: {}".format(desc.version))
                for timing in enumerate(desc.three_byte_timings):
                    aspect = 1.0
                    if not (parse_version("{}.{}".format(edid.header.vermaj, edid.header.vermin)) < parse_version('1.3') and aspect == aspect1610):
                        aspect = int(timing.aspect.name[6:8]) / int(timing.aspect.name[8:])
                    print("{}: {}x{} @{}Hz".format(timing[0], int(timing[1].addressable_lines * aspect), timing[1].addressable_lines, timing[1].preferred_vertical_rate.name[3:]), end='')
                    if timing[1].hz_50 or timing[1].hz_60 or timing[1].hz_75 or timing[1].hz_85 or timing[1].hz_60r:
                        print(" Also:", end='')
                        for hz in timing[1].__dict__.keys():
                            if hz[:3] == 'hz_':
                                if timing[1].__dict__[hz] == True:
                                    print(" {}Hz".format(int(mode[3:])))
                print()
            elif desc[1].desc.type == Edid.DescType.display_color_management:
                desc = desc[1].desc.display_color_management
                print("** {}: Display Color Management **".format(num))
                print("Version: {}".format(desc.version))
                print("Red A2: {}".format(desc.red_a2))
                print("Red A3: {}".format(desc.red_a3))
                print("Green A2: {}".format(desc.green_a2))
                print("Green A3: {}".format(desc.green_a3))
                print("Blue A2: {}".format(desc.blue_a2))
                print("Blue A3: {}".format(desc.blue_a3))
                print()
            elif desc[1].desc.type == Edid.DescType.additional_two_byte_timings:
                desc = desc[1].desc.additional_two_byte_timings
                print("** {}: Additional Two-Byte Timings **".format(num))
                for timing in desc.two_byte_timings:
                    parseTwoByteTiming(timing, edid.header.vermaj, edid.header.vermin)
            elif desc[1].desc.type == Edid.DescType.additional_white_points:
                desc = desc[1].desc.additional_white_points
                print("** {}: Additional White Points **".format(num))
                for whitePoint in enumerate(desc.white_points):
                    print("{}: Index: {}, Coords: ({}, {}), Gamma: {}".format(whitePoint[0], whitePoint[1].index, whitePoint[1].x, whitePoint[1].y, whitePoint[1].gamma))
            elif desc[1].desc.type == Edid.DescType.display_name:
                desc = desc[1].desc.display_name
                print("** {}: Display Name **".format(num))
                print("Name:", desc.text.decode('cp437'))
            elif desc[1].desc.type == Edid.DescType.display_range_limits:
                desc = desc[1].desc.display_range_limits
                print("** {}: Display Range Limits **".format(num))
                print("Minimum Vertical Rate: {} Hz".format(desc.minimum_vertical_rate))
                print("Maximum Vertical Rate: {} Hz".format(desc.maximum_vertical_rate))
                print("Minimum Horizontal Rate: {} kHz".format(desc.minimum_horizontal_rate))
                print("Maximum Horizontal Rate: {} kHz".format(desc.maximum_horizontal_rate))
                print("Maximum Pixel Clock: {} Mhz".format(desc.max_pixel_clock))
                print("Timing Type:", desc.timing_information_type.name.replace('_', ' '))
                if desc.timing_information_type == Edid.TimingInformationTypes.secondary_gtf:
                    gtf = desc.gtf
                    print("Start freq: {}".format(gtf.start))
                    print("C: {}".format(gtf.c))
                    print("M: {}".format(gtf.m))
                    print("K: {}".format(gtf.k))
                    print("J: {}".format(gtf.j))
                elif desc.timing_information_type == Edid.TimingInformationTypes.cvt:
                    cvt = desc.cvt
                    print("Version: {}.{}".format(cvt.vermaj, cvt.vermin))
                    print("Additional Clock Precision: Maximum Pixel Clock - {} Mhz".format(cvt.additional_clock_precision))
                    print("Max Pixel Width: {}".format(cvt.max_width))
                    print("Aspect Ratios:", end='')
                    if not (cvt.aspect0403 or cvt.aspect1609 or cvt.aspect1610 or cvt.aspect0504 or cvt.aspect1509):
                        print(" none")
                    else:
                        for aspect in cvt.__dict__.keys():
                            if aspect[:6] == 'aspect':
                                if cvt.__dict__[aspect] == True:
                                    print("{}:{}".format(int(mode[6:8]), int(mode[8:])))
                    print("Preferred Aspect: {}:{}".format(int(cvt.preferred_aspect.name[6:8]), int(cvt.preferred_aspect.name[8:])))
                    if cvt.reduced_blanking:
                        print("Reduced Blanking Preferred")
                    if cvt.standard_blanking:
                        print("Standard Blanking Supported")
                    if cvt.horizontal_shrink:
                        print("Horizontal Shrink Supported")
                    if cvt.horizontal_stretch:
                        print("Horizontal Stretch Supported")
                    if cvt.vertical_shrink:
                        print("Vertical Shrink Supported")
                    if cvt.vertical_stretch:
                        print("Vertical Stretch Supported")
                    print("Preferred Vertical Refresh: {} Hz".format(cvt.preferred_vertical_refresh))
                print()
            elif desc[1].desc.type == Edid.DescType.unspecified_text:
                desc = desc[1].desc.unspecified_text
                print("** {}: Unspecified Text **".format(num))
                print("Text:", desc.text.decode('cp437'))
            elif desc[1].desc.type == Edid.DescType.display_serial:
                desc = desc[1].desc.display_serial
                print("** {}: Display Serial **".format(num))
                print("Serial:", desc.text.decode('cp437'))

