# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

from pkg_resources import parse_version
from kaitaistruct import __version__ as ks_version, KaitaiStruct, KaitaiStream, BytesIO
from enum import Enum


if parse_version(ks_version) < parse_version('0.7'):
    raise Exception("Incompatible Kaitai Struct Python API: 0.7 or later is required, but you have %s" % (ks_version))

class Edid(KaitaiStruct):

    class DigitalSyncType(Enum):
        composite = 0
        separate = 1

    class Cea861DataBlockType(Enum):
        audio = 1
        video = 2
        vendor_defined = 3
        speaker_allocation = 4

    class AnalogDisplayType(Enum):
        black_and_white = 0
        rgb_color = 1
        non_rgb_color = 2
        undefined = 3

    class TimingAspect(Enum):
        aspect1610 = 0
        aspect0403 = 1
        aspect0504 = 2
        aspect1609 = 3

    class AnalogSyncType(Enum):
        composite = 0
        bipolar_composite = 1

    class InputType(Enum):
        analog = 0
        digital = 1

    class TimingInformationTypes(Enum):
        default_gtf = 0
        none = 1
        secondary_gtf = 2
        cvt = 4

    class VideoInterface(Enum):
        undefined = 0
        hdmia = 1
        hdmib = 2
        mddi = 4
        displayport = 5

    class DigitalDisplayType(Enum):
        rgb444 = 0
        rgb444_ycrcb444 = 1
        rgb444_ycrcb422 = 2
        rgb444_ycrcb444_ycrcb422 = 3

    class BitDepth(Enum):
        undefined = 0
        bits_6 = 1
        bits_8 = 2
        bits_10 = 3
        bits_12 = 4
        bits_14 = 5
        bits_16 = 6
        reserved = 7

    class PreferredAspect(Enum):
        aspect0403 = 0
        aspect1609 = 1
        aspect1610 = 2
        aspect0504 = 3
        aspect1509 = 4

    class Cea861VideoFormat(Enum):
        format_006400480ps060 = 1
        format_007200480ps060 = 2
        format_007200480pw060 = 3
        format_012800720pw060 = 4
        format_019201080iw060 = 5
        format_014400480is060 = 6
        format_014400480iw060 = 7
        format_014400240ps060 = 8
        format_014400240pw060 = 9
        format_028800480is060 = 10
        format_028800480iw060 = 11
        format_028800240ps060 = 12
        format_028800240pw060 = 13
        format_014400480ps060 = 14
        format_014400480pw060 = 15
        format_019201080pw060 = 16
        format_007200576ps050 = 17
        format_007200576pw050 = 18
        format_012800720pw050 = 19
        format_019201080iw050 = 20
        format_014400576is050 = 21
        format_014400576iw050 = 22
        format_014400288ps050 = 23
        format_014400288pw050 = 24
        format_028800576is050 = 25
        format_028800576iw050 = 26
        format_028800288ps050 = 27
        format_028800288pw050 = 28
        format_014400576ps050 = 29
        format_014400576pw050 = 30
        format_019201080pw050 = 31
        format_019201080pw024 = 32
        format_019201080pw025 = 33
        format_019201080pw030 = 34
        format_028800480ps060 = 35
        format_028800480pw060 = 36
        format_028800576ps050 = 37
        format_028800576pw050 = 38
        format_019201080iw050b = 39
        format_019201080iw100 = 40
        format_012800720pw100 = 41
        format_007200576ps100 = 42
        format_007200576pw100 = 43
        format_014400576is100 = 44
        format_014400576iw100 = 45
        format_019201080iw120 = 46
        format_012800720pw120 = 47
        format_007200480ps120 = 48
        format_007200480pw120 = 49
        format_014400480is120 = 50
        format_014400480iw120 = 51
        format_007200576ps200 = 52
        format_007200576pw200 = 53
        format_014400576is200 = 54
        format_014400576iw200 = 55
        format_007200480ps240 = 56
        format_007200480pw240 = 57
        format_014400480is240 = 58
        format_014400480iw240 = 59
        format_012800720pw024 = 60
        format_012800720pw025 = 61
        format_012800720pw030 = 62
        format_019201080pw120 = 63
        format_019201080pw100 = 64
        format_012800720pu024 = 65
        format_012800720pu025 = 66
        format_012800720pu030 = 67
        format_012800720pu050 = 68
        format_012800720pu060 = 69
        format_012800720pu100 = 70
        format_012800720pu120 = 71
        format_019201080pu024 = 72
        format_019201080pu025 = 73
        format_019201080pu030 = 74
        format_019201080pu050 = 75
        format_019201080pu060 = 76
        format_019201080pu100 = 77
        format_019201080pu120 = 78
        format_016800720pu024 = 79
        format_016800720pu025 = 80
        format_016800720pu030 = 81
        format_016800720pu050 = 82
        format_016800720pu060 = 83
        format_016800720pu100 = 84
        format_016800720pu120 = 85
        format_025601080pu024 = 86
        format_025601080pu025 = 87
        format_025601080pu030 = 88
        format_025601080pu050 = 89
        format_025601080pu060 = 90
        format_025601080pu100 = 91
        format_025601080pu120 = 92
        format_038402160pw024 = 93
        format_038402160pw025 = 94
        format_038402160pw030 = 95
        format_038402160pw050 = 96
        format_038402160pw060 = 97
        format_040962160pc024 = 98
        format_040962160pc025 = 99
        format_040962160pc030 = 100
        format_040962160pc050 = 101
        format_040962160pc060 = 102
        format_038402160pu024 = 103
        format_038402160pu025 = 104
        format_038402160pu030 = 105
        format_038402160pu050 = 106
        format_038402160pu060 = 107
        format_012800720pw048 = 108
        format_012800720pu048 = 109
        format_016800720pu048 = 110
        format_019201080pw048 = 111
        format_019201080pu048 = 112
        format_025601080pu048 = 113
        format_038402160pw048 = 114
        format_040962160pc048 = 115
        format_038402160pu048 = 116
        format_038402160pw100 = 117
        format_038402160pw120 = 118
        format_038402160pu100 = 119
        format_038402160pu120 = 120
        format_051202160pu024 = 121
        format_051202160pu025 = 122
        format_051202160pu030 = 123
        format_051202160pu048 = 124
        format_051202160pu050 = 125
        format_051202160pu060 = 126
        format_051202160pu100 = 127
        format_051202160pu120 = 193
        format_076804320pw024 = 194
        format_076804320pw025 = 195
        format_076804320pw030 = 196
        format_076804320pw048 = 197
        format_076804320pw050 = 198
        format_076804320pw060 = 199
        format_076804320pw100 = 200
        format_076804320pw120 = 201
        format_076804320pu024 = 202
        format_076804320pu025 = 203
        format_076804320pu030 = 204
        format_076804320pu048 = 205
        format_076804320pu050 = 206
        format_076804320pu060 = 207
        format_076804320pu100 = 208
        format_076804320pu120 = 209
        format_102404320pu024 = 210
        format_102404320pu025 = 211
        format_102404320pu030 = 212
        format_102404320pu048 = 213
        format_102404320pu050 = 214
        format_102404320pu060 = 215
        format_102404320pu100 = 216
        format_102404320pu120 = 217
        format_040962160pc100 = 218
        format_040962160pc120 = 219

    class Fivebitlettermap(Enum):
        a = 1
        b = 2
        c = 3
        d = 4
        e = 5
        f = 6
        g = 7
        h = 8
        i = 9
        j = 10
        k = 11
        l = 12
        m = 13
        n = 14
        o = 15
        p = 16
        q = 17
        r = 18
        s = 19
        t = 20
        u = 21
        v = 22
        w = 23
        x = 24
        y = 25
        z = 26

    class Cea861AudioFormat(Enum):
        lpcm = 1
        ac3 = 2
        mpeg1 = 3
        mp3 = 4
        mpeg2 = 5
        aac = 6
        dts = 7
        atrac = 8
        dsd = 9
        ddplus = 10
        dtshd = 11
        truehd = 12
        dst = 13
        wmapro = 14

    class VideoLevels(Enum):
        v07000300 = 0
        v07140286 = 1
        v10000400 = 2
        v07000000 = 3

    class StereoMode(Enum):
        none = 0
        invalid = 1
        field_sequential = 2
        right_image_even = 3
        opposite_field_sequential = 4
        left_image_even = 5
        four_way_interleaved = 6
        side_by_side = 7

    class ExtensionType(Enum):
        cea_861 = 2
        unknown1 = 112

    class SyncPolarity(Enum):
        negative = 0
        positive = 1

    class PreferredVerticalRate(Enum):
        hz_50 = 0
        hz_60 = 1
        hz_75 = 2
        hz_85 = 3

    class SyncSignalType(Enum):
        analog = 0
        digital = 1

    class DescType(Enum):
        additional_standard_timings = 247
        three_byte_timings = 248
        display_color_management = 249
        additional_two_byte_timings = 250
        additional_white_points = 251
        display_name = 252
        display_range_limits = 253
        unspecified_text = 254
        display_serial = 255
    def __init__(self, _io, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self
        self._read()

    def _read(self):
        self.header = self._root.Header(self._io, self, self._root)
        self.basic_params = self._root.BasicParams(self._io, self, self._root)
        self.chromaticity = self._root.Chromaticity(self._io, self, self._root)
        self.modes = self._root.Modes(self._io, self, self._root)
        self.two_byte_timings = self._root.TwoByteTimings(self._io, self, self._root)
        self.descriptor_blocks = self._root.DescriptorBlocks(self._io, self, self._root)
        self.extensions = self._io.read_u1()
        self.checksum = self._io.read_u1()
        self._raw_extension = [None] * (self.extensions)
        self.extension = [None] * (self.extensions)
        for i in range(self.extensions):
            self._raw_extension[i] = self._io.read_bytes(128)
            io = KaitaiStream(BytesIO(self._raw_extension[i]))
            self.extension[i] = self._root.Extension(io, self, self._root)


    class Modes(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.mode0720040070 = self._io.read_bits_int(1) != 0
            self.mode0720040088 = self._io.read_bits_int(1) != 0
            self.mode0640048060 = self._io.read_bits_int(1) != 0
            self.mode0640048067 = self._io.read_bits_int(1) != 0
            self.mode0640048072 = self._io.read_bits_int(1) != 0
            self.mode0640048075 = self._io.read_bits_int(1) != 0
            self.mode0800060056 = self._io.read_bits_int(1) != 0
            self.mode0800060060 = self._io.read_bits_int(1) != 0
            self.mode0800060072 = self._io.read_bits_int(1) != 0
            self.mode0800060075 = self._io.read_bits_int(1) != 0
            self.mode0832062475 = self._io.read_bits_int(1) != 0
            self.mode1024076887i = self._io.read_bits_int(1) != 0
            self.mode1024060060 = self._io.read_bits_int(1) != 0
            self.mode1024076872 = self._io.read_bits_int(1) != 0
            self.mode1024076875 = self._io.read_bits_int(1) != 0
            self.mode1280102475 = self._io.read_bits_int(1) != 0
            self.mode1152087075 = self._io.read_bits_int(1) != 0
            self.other_modes = self._io.read_bits_int(7)


    class Cea861Dtds(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self._raw_dtds = []
            self.dtds = []
            i = 0
            while True:
                _buf = self._io.read_bytes(18)
                self._raw_dtds.append(_buf)
                io = KaitaiStream(BytesIO(self._raw_dtds[-1]))
                _ = self._root.Cea861Dtd(io, self, self._root)
                self.dtds.append(_)
                if _.is_dtd == 0:
                    break
                i += 1


    class Desc(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.type = self._root.DescType(self._io.read_u1())

        @property
        def display_color_management(self):
            if hasattr(self, '_m_display_color_management'):
                return self._m_display_color_management if hasattr(self, '_m_display_color_management') else None

            if self.type == self._root.DescType.display_color_management:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_display_color_management = self._root.DisplayColorManagement(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_display_color_management if hasattr(self, '_m_display_color_management') else None

        @property
        def display_name(self):
            if hasattr(self, '_m_display_name'):
                return self._m_display_name if hasattr(self, '_m_display_name') else None

            if self.type == self._root.DescType.display_name:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_display_name = self._root.TextDescriptor(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_display_name if hasattr(self, '_m_display_name') else None

        @property
        def additional_two_byte_timings(self):
            if hasattr(self, '_m_additional_two_byte_timings'):
                return self._m_additional_two_byte_timings if hasattr(self, '_m_additional_two_byte_timings') else None

            if self.type == self._root.DescType.additional_two_byte_timings:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_additional_two_byte_timings = self._root.AdditionalTwoByteTimings(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_additional_two_byte_timings if hasattr(self, '_m_additional_two_byte_timings') else None

        @property
        def display_range_limits(self):
            if hasattr(self, '_m_display_range_limits'):
                return self._m_display_range_limits if hasattr(self, '_m_display_range_limits') else None

            if self.type == self._root.DescType.display_range_limits:
                _pos = self._io.pos()
                self._io.seek(4)
                self._m_display_range_limits = self._root.DisplayRangeLimits(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_display_range_limits if hasattr(self, '_m_display_range_limits') else None

        @property
        def display_serial(self):
            if hasattr(self, '_m_display_serial'):
                return self._m_display_serial if hasattr(self, '_m_display_serial') else None

            if self.type == self._root.DescType.display_serial:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_display_serial = self._root.TextDescriptor(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_display_serial if hasattr(self, '_m_display_serial') else None

        @property
        def unknown(self):
            if hasattr(self, '_m_unknown'):
                return self._m_unknown if hasattr(self, '_m_unknown') else None

            self._m_unknown = self._root.UnknownDescriptor(self._io, self, self._root)
            return self._m_unknown if hasattr(self, '_m_unknown') else None

        @property
        def unspecified_text(self):
            if hasattr(self, '_m_unspecified_text'):
                return self._m_unspecified_text if hasattr(self, '_m_unspecified_text') else None

            if self.type == self._root.DescType.unspecified_text:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_unspecified_text = self._root.TextDescriptor(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_unspecified_text if hasattr(self, '_m_unspecified_text') else None

        @property
        def three_byte_timings(self):
            if hasattr(self, '_m_three_byte_timings'):
                return self._m_three_byte_timings if hasattr(self, '_m_three_byte_timings') else None

            if self.type == self._root.DescType.three_byte_timings:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_three_byte_timings = self._root.ThreeByteTimings(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_three_byte_timings if hasattr(self, '_m_three_byte_timings') else None

        @property
        def additional_standard_timings(self):
            if hasattr(self, '_m_additional_standard_timings'):
                return self._m_additional_standard_timings if hasattr(self, '_m_additional_standard_timings') else None

            if self.type == self._root.DescType.additional_standard_timings:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_additional_standard_timings = self._root.AdditionalStandardTimings(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_additional_standard_timings if hasattr(self, '_m_additional_standard_timings') else None

        @property
        def additional_white_points(self):
            if hasattr(self, '_m_additional_white_points'):
                return self._m_additional_white_points if hasattr(self, '_m_additional_white_points') else None

            if self.type == self._root.DescType.additional_white_points:
                _pos = self._io.pos()
                self._io.seek(5)
                self._m_additional_white_points = self._root.AdditionalWhitePoints(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_additional_white_points if hasattr(self, '_m_additional_white_points') else None


    class Cea861VideoDataBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.format = []
            i = 0
            while not self._io.is_eof():
                self.format.append(self._root.Cea861VideoFormat(self._io.read_u1()))
                i += 1



    class DisplayRangeLimits(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.unused = self._io.read_bits_int(4)
            self.hmax_msb = self._io.read_bits_int(1) != 0
            self.hmin_msb = self._io.read_bits_int(1) != 0
            self.vmax_msb = self._io.read_bits_int(1) != 0
            self.vmin_msb = self._io.read_bits_int(1) != 0
            self._io.align_to_byte()
            self.vmin_lsb = self._io.read_u1()
            self.vmax_lsb = self._io.read_u1()
            self.hmin_lsb = self._io.read_u1()
            self.hmax_lsb = self._io.read_u1()
            self.raw_max_pixel_clock = self._io.read_u1()
            self.timing_information_type = self._root.TimingInformationTypes(self._io.read_u1())
            if self.timing_information_type == self._root.TimingInformationTypes.secondary_gtf:
                self.gtf = self._root.Gtf(self._io, self, self._root)

            if self.timing_information_type == self._root.TimingInformationTypes.cvt:
                self.cvt = self._root.Cvt(self._io, self, self._root)


        @property
        def minimum_horizontal_rate(self):
            if hasattr(self, '_m_minimum_horizontal_rate'):
                return self._m_minimum_horizontal_rate if hasattr(self, '_m_minimum_horizontal_rate') else None

            self._m_minimum_horizontal_rate = (((1 if self.hmin_msb else 0) << 8) | self.hmin_lsb)
            return self._m_minimum_horizontal_rate if hasattr(self, '_m_minimum_horizontal_rate') else None

        @property
        def max_pixel_clock(self):
            if hasattr(self, '_m_max_pixel_clock'):
                return self._m_max_pixel_clock if hasattr(self, '_m_max_pixel_clock') else None

            self._m_max_pixel_clock = (self.raw_max_pixel_clock * 10)
            return self._m_max_pixel_clock if hasattr(self, '_m_max_pixel_clock') else None

        @property
        def maximum_horizontal_rate(self):
            if hasattr(self, '_m_maximum_horizontal_rate'):
                return self._m_maximum_horizontal_rate if hasattr(self, '_m_maximum_horizontal_rate') else None

            self._m_maximum_horizontal_rate = (((1 if self.hmax_msb else 0) << 8) | self.hmax_lsb)
            return self._m_maximum_horizontal_rate if hasattr(self, '_m_maximum_horizontal_rate') else None

        @property
        def minimum_vertical_rate(self):
            if hasattr(self, '_m_minimum_vertical_rate'):
                return self._m_minimum_vertical_rate if hasattr(self, '_m_minimum_vertical_rate') else None

            self._m_minimum_vertical_rate = (((1 if self.vmin_msb else 0) << 8) | self.vmin_lsb)
            return self._m_minimum_vertical_rate if hasattr(self, '_m_minimum_vertical_rate') else None

        @property
        def maximum_vertical_rate(self):
            if hasattr(self, '_m_maximum_vertical_rate'):
                return self._m_maximum_vertical_rate if hasattr(self, '_m_maximum_vertical_rate') else None

            self._m_maximum_vertical_rate = (((1 if self.vmax_msb else 0) << 8) | self.vmax_lsb)
            return self._m_maximum_vertical_rate if hasattr(self, '_m_maximum_vertical_rate') else None


    class Dtd(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.raw_pixel_clock = self._io.read_u2le()
            self.hactive_lsb = self._io.read_u1()
            self.hblank_lsb = self._io.read_u1()
            self.hactive_msb = self._io.read_bits_int(4)
            self.hblank_msb = self._io.read_bits_int(4)
            self._io.align_to_byte()
            self.vactive_lsb = self._io.read_u1()
            self.vblank_lsb = self._io.read_u1()
            self.vactive_msb = self._io.read_bits_int(4)
            self.vblank_msb = self._io.read_bits_int(4)
            self._io.align_to_byte()
            self.hfront_porch_lsb = self._io.read_u1()
            self.hsync_pulse_lsb = self._io.read_u1()
            self.vfront_porch_lsb = self._io.read_bits_int(4)
            self.vsync_pulse_lsb = self._io.read_bits_int(4)
            self.hfront_porch_msb = self._io.read_bits_int(2)
            self.hsync_pulse_msb = self._io.read_bits_int(2)
            self.vfront_porch_msb = self._io.read_bits_int(2)
            self.vsync_pulse_msb = self._io.read_bits_int(2)
            self._io.align_to_byte()
            self.width_lsb = self._io.read_u1()
            self.height_lsb = self._io.read_u1()
            self.width_msb = self._io.read_bits_int(4)
            self.height_msb = self._io.read_bits_int(4)
            self._io.align_to_byte()
            self.hborder = self._io.read_u1()
            self.vborder = self._io.read_u1()
            self.interlaced = self._io.read_bits_int(1) != 0
            self.stereo_hi_bits = self._io.read_bits_int(2)
            self.sync_signal_type = self._root.SyncSignalType(self._io.read_bits_int(1))
            if self.sync_signal_type == self._root.SyncSignalType.analog:
                self.analog_sync_type = self._root.AnalogSyncType(self._io.read_bits_int(1))

            if self.sync_signal_type == self._root.SyncSignalType.analog:
                self.analog_vsync_serration = self._io.read_bits_int(1) != 0

            if self.sync_signal_type == self._root.SyncSignalType.analog:
                self.sync_on_all = self._io.read_bits_int(1) != 0

            if self.sync_signal_type == self._root.SyncSignalType.digital:
                self.digital_sync_type = self._root.DigitalSyncType(self._io.read_bits_int(1))

            if  ((self.sync_signal_type == self._root.SyncSignalType.digital) and (self.digital_sync_type == self._root.DigitalSyncType.composite)) :
                self.digital_vsync_polarity = self._root.SyncPolarity(self._io.read_bits_int(1))

            if  ((self.sync_signal_type == self._root.SyncSignalType.digital) and (self.digital_sync_type == self._root.DigitalSyncType.composite)) :
                self.digital_sync_reserved = self._io.read_bits_int(1) != 0

            if  ((self.sync_signal_type == self._root.SyncSignalType.digital) and (self.digital_sync_type == self._root.DigitalSyncType.separate)) :
                self.digital_vsync_serration = self._io.read_bits_int(1) != 0

            if  ((self.sync_signal_type == self._root.SyncSignalType.digital) and (self.digital_sync_type == self._root.DigitalSyncType.separate)) :
                self.digital_hsync_polarity = self._root.SyncPolarity(self._io.read_bits_int(1))

            self.stereo_lo_bit = self._io.read_bits_int(1) != 0

        @property
        def vblank(self):
            if hasattr(self, '_m_vblank'):
                return self._m_vblank if hasattr(self, '_m_vblank') else None

            self._m_vblank = ((self.vblank_msb << 8) | self.vblank_lsb)
            return self._m_vblank if hasattr(self, '_m_vblank') else None

        @property
        def height(self):
            if hasattr(self, '_m_height'):
                return self._m_height if hasattr(self, '_m_height') else None

            self._m_height = ((self.height_msb << 8) | self.height_lsb)
            return self._m_height if hasattr(self, '_m_height') else None

        @property
        def pixel_clock(self):
            if hasattr(self, '_m_pixel_clock'):
                return self._m_pixel_clock if hasattr(self, '_m_pixel_clock') else None

            self._m_pixel_clock = (self.raw_pixel_clock / 100.0)
            return self._m_pixel_clock if hasattr(self, '_m_pixel_clock') else None

        @property
        def vactive(self):
            if hasattr(self, '_m_vactive'):
                return self._m_vactive if hasattr(self, '_m_vactive') else None

            self._m_vactive = ((self.vactive_msb << 8) | self.vactive_lsb)
            return self._m_vactive if hasattr(self, '_m_vactive') else None

        @property
        def hactive(self):
            if hasattr(self, '_m_hactive'):
                return self._m_hactive if hasattr(self, '_m_hactive') else None

            self._m_hactive = ((self.hactive_msb << 8) | self.hactive_lsb)
            return self._m_hactive if hasattr(self, '_m_hactive') else None

        @property
        def hfront_porch(self):
            if hasattr(self, '_m_hfront_porch'):
                return self._m_hfront_porch if hasattr(self, '_m_hfront_porch') else None

            self._m_hfront_porch = ((self.hfront_porch_msb << 8) | self.hfront_porch_lsb)
            return self._m_hfront_porch if hasattr(self, '_m_hfront_porch') else None

        @property
        def hblank(self):
            if hasattr(self, '_m_hblank'):
                return self._m_hblank if hasattr(self, '_m_hblank') else None

            self._m_hblank = ((self.hblank_msb << 8) | self.hblank_lsb)
            return self._m_hblank if hasattr(self, '_m_hblank') else None

        @property
        def stereo_mode(self):
            if hasattr(self, '_m_stereo_mode'):
                return self._m_stereo_mode if hasattr(self, '_m_stereo_mode') else None

            self._m_stereo_mode = self._root.StereoMode(((self.stereo_hi_bits << 1) + (1 if self.stereo_lo_bit else 0)))
            return self._m_stereo_mode if hasattr(self, '_m_stereo_mode') else None

        @property
        def width(self):
            if hasattr(self, '_m_width'):
                return self._m_width if hasattr(self, '_m_width') else None

            self._m_width = ((self.width_msb << 8) | self.width_lsb)
            return self._m_width if hasattr(self, '_m_width') else None

        @property
        def hsync_pulse(self):
            if hasattr(self, '_m_hsync_pulse'):
                return self._m_hsync_pulse if hasattr(self, '_m_hsync_pulse') else None

            self._m_hsync_pulse = ((self.hsync_pulse_msb << 8) | self.hsync_pulse_lsb)
            return self._m_hsync_pulse if hasattr(self, '_m_hsync_pulse') else None

        @property
        def vfront_porch(self):
            if hasattr(self, '_m_vfront_porch'):
                return self._m_vfront_porch if hasattr(self, '_m_vfront_porch') else None

            self._m_vfront_porch = ((self.vfront_porch_msb << 4) | self.vfront_porch_lsb)
            return self._m_vfront_porch if hasattr(self, '_m_vfront_porch') else None

        @property
        def vsync_pulse(self):
            if hasattr(self, '_m_vsync_pulse'):
                return self._m_vsync_pulse if hasattr(self, '_m_vsync_pulse') else None

            self._m_vsync_pulse = ((self.vsync_pulse_msb << 4) | self.vsync_pulse_lsb)
            return self._m_vsync_pulse if hasattr(self, '_m_vsync_pulse') else None


    class ManuId(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.pad = self._io.read_bits_int(1) != 0
            self.letter = [None] * (3)
            for i in range(3):
                self.letter[i] = self._root.Fivebitlettermap(self._io.read_bits_int(5))



    class AdditionalTwoByteTimings(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.two_byte_timings = [None] * (6)
            for i in range(6):
                self.two_byte_timings[i] = self._root.TwoByteTiming(self._io, self, self._root)

            self.unused = self._io.read_u1()


    class Cea861DataBlocks(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data_blocks = []
            i = 0
            while not self._io.is_eof():
                self.data_blocks.append(self._root.Cea861DataBlock(self._io, self, self._root))
                i += 1



    class WhitePoint(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.index = self._io.read_u1()
            self.unused = self._io.read_bits_int(4)
            self.x_lsb = self._io.read_bits_int(2)
            self.y_lsb = self._io.read_bits_int(2)
            self._io.align_to_byte()
            self.x_msb = self._io.read_u1()
            self.y_msb = self._io.read_u1()
            self.raw_gamma = self._io.read_u1()

        @property
        def x(self):
            if hasattr(self, '_m_x'):
                return self._m_x if hasattr(self, '_m_x') else None

            self._m_x = ((self.x_msb << 2) | self.x_lsb)
            return self._m_x if hasattr(self, '_m_x') else None

        @property
        def y(self):
            if hasattr(self, '_m_y'):
                return self._m_y if hasattr(self, '_m_y') else None

            self._m_y = ((self.y_msb << 2) | self.y_lsb)
            return self._m_y if hasattr(self, '_m_y') else None

        @property
        def gamma(self):
            if hasattr(self, '_m_gamma'):
                return self._m_gamma if hasattr(self, '_m_gamma') else None

            self._m_gamma = (self.raw_gamma + 100) // 100
            return self._m_gamma if hasattr(self, '_m_gamma') else None


    class Gtf(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.reserved = self._io.read_u1()
            self.raw_start = self._io.read_u1()
            self.raw_c = self._io.read_u1()
            self.m = self._io.read_u2le()
            self.k = self._io.read_u1()
            self.raw_j = self._io.read_u1()

        @property
        def start(self):
            if hasattr(self, '_m_start'):
                return self._m_start if hasattr(self, '_m_start') else None

            self._m_start = (self.raw_start * 2)
            return self._m_start if hasattr(self, '_m_start') else None

        @property
        def c(self):
            if hasattr(self, '_m_c'):
                return self._m_c if hasattr(self, '_m_c') else None

            self._m_c = (self.raw_c / 2.0)
            return self._m_c if hasattr(self, '_m_c') else None

        @property
        def j(self):
            if hasattr(self, '_m_j'):
                return self._m_j if hasattr(self, '_m_j') else None

            self._m_j = (self.raw_j / 2.0)
            return self._m_j if hasattr(self, '_m_j') else None


    class Cea861VendorDataBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.raw_type1 = self._io.read_u2le()
            self.raw_type2 = self._io.read_u1()

        @property
        def type(self):
            if hasattr(self, '_m_type'):
                return self._m_type if hasattr(self, '_m_type') else None

            self._m_type = ((self.raw_type1 << 16) | self.raw_type2)
            return self._m_type if hasattr(self, '_m_type') else None


    class AdditionalWhitePoints(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.white_points = [None] * (2)
            for i in range(2):
                self.white_points[i] = self._root.WhitePoint(self._io, self, self._root)

            self.unused = self._io.read_bytes(3)


    class ThreeByteTiming(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.addressable_lines_lsb = self._io.read_u1()
            self.addressable_lines_msb = self._io.read_bits_int(4)
            self.preferred_vertical_rate = self._root.PreferredVerticalRate(self._io.read_bits_int(2))
            self.unused = self._io.read_bits_int(3)
            self.aspect_ratio = self._root.TimingAspect(self._io.read_bits_int(2))
            self.hz_50 = self._io.read_bits_int(1) != 0
            self.hz_60 = self._io.read_bits_int(1) != 0
            self.hz_75 = self._io.read_bits_int(1) != 0
            self.hz_85 = self._io.read_bits_int(1) != 0
            self.hz_60r = self._io.read_bits_int(1) != 0

        @property
        def addressable_lines(self):
            if hasattr(self, '_m_addressable_lines'):
                return self._m_addressable_lines if hasattr(self, '_m_addressable_lines') else None

            self._m_addressable_lines = ((self.addressable_lines_msb << 8) | self.addressable_lines_lsb)
            return self._m_addressable_lines if hasattr(self, '_m_addressable_lines') else None


    class Cea861AudioDescriptor(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.reserved1 = self._io.read_bits_int(1) != 0
            self.format = self._root.Cea861AudioFormat(self._io.read_bits_int(4))
            self.raw_channels = self._io.read_bits_int(3)
            self.reserved2 = self._io.read_bits_int(1) != 0
            self.khz_192 = self._io.read_bits_int(1) != 0
            self.khz_176 = self._io.read_bits_int(1) != 0
            self.khz_96 = self._io.read_bits_int(1) != 0
            self.khz_88 = self._io.read_bits_int(1) != 0
            self.khz_48 = self._io.read_bits_int(1) != 0
            self.khz_44 = self._io.read_bits_int(1) != 0
            self.khz_32 = self._io.read_bits_int(1) != 0
            if self.format == self._root.Cea861AudioFormat.lpcm:
                self.reserved3 = self._io.read_bits_int(5)

            if self.format == self._root.Cea861AudioFormat.lpcm:
                self.bits_24 = self._io.read_bits_int(1) != 0

            if self.format == self._root.Cea861AudioFormat.lpcm:
                self.bits_20 = self._io.read_bits_int(1) != 0

            if self.format == self._root.Cea861AudioFormat.lpcm:
                self.bits_16 = self._io.read_bits_int(1) != 0

            self._io.align_to_byte()
            if self.format != self._root.Cea861AudioFormat.lpcm:
                self.max_bitrate = self._io.read_u1()


        @property
        def channels(self):
            if hasattr(self, '_m_channels'):
                return self._m_channels if hasattr(self, '_m_channels') else None

            self._m_channels = (self.raw_channels + 1)
            return self._m_channels if hasattr(self, '_m_channels') else None


    class Chromaticity(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.red_x_lsb = self._io.read_bits_int(2)
            self.red_y_lsb = self._io.read_bits_int(2)
            self.green_x_lsb = self._io.read_bits_int(2)
            self.green_y_lsb = self._io.read_bits_int(2)
            self.blue_x_lsb = self._io.read_bits_int(2)
            self.blue_y_lsb = self._io.read_bits_int(2)
            self.white_x_lsb = self._io.read_bits_int(2)
            self.white_y_lsb = self._io.read_bits_int(2)
            self._io.align_to_byte()
            self.red_x_msb = self._io.read_u1()
            self.red_y_msb = self._io.read_u1()
            self.green_x_msb = self._io.read_u1()
            self.green_y_msb = self._io.read_u1()
            self.blue_x_msb = self._io.read_u1()
            self.blue_y_msb = self._io.read_u1()
            self.white_x_msb = self._io.read_u1()
            self.white_y_msb = self._io.read_u1()

        @property
        def red_y(self):
            if hasattr(self, '_m_red_y'):
                return self._m_red_y if hasattr(self, '_m_red_y') else None

            self._m_red_y = ((self.red_y_lsb << 8) | self.red_y_msb)
            return self._m_red_y if hasattr(self, '_m_red_y') else None

        @property
        def white_y(self):
            if hasattr(self, '_m_white_y'):
                return self._m_white_y if hasattr(self, '_m_white_y') else None

            self._m_white_y = ((self.white_y_lsb << 8) | self.white_y_msb)
            return self._m_white_y if hasattr(self, '_m_white_y') else None

        @property
        def red_x(self):
            if hasattr(self, '_m_red_x'):
                return self._m_red_x if hasattr(self, '_m_red_x') else None

            self._m_red_x = ((self.red_x_lsb << 8) | self.red_x_msb)
            return self._m_red_x if hasattr(self, '_m_red_x') else None

        @property
        def white_x(self):
            if hasattr(self, '_m_white_x'):
                return self._m_white_x if hasattr(self, '_m_white_x') else None

            self._m_white_x = ((self.white_x_lsb << 8) | self.white_x_msb)
            return self._m_white_x if hasattr(self, '_m_white_x') else None

        @property
        def blue_x(self):
            if hasattr(self, '_m_blue_x'):
                return self._m_blue_x if hasattr(self, '_m_blue_x') else None

            self._m_blue_x = ((self.blue_x_lsb << 8) | self.blue_x_msb)
            return self._m_blue_x if hasattr(self, '_m_blue_x') else None

        @property
        def green_x(self):
            if hasattr(self, '_m_green_x'):
                return self._m_green_x if hasattr(self, '_m_green_x') else None

            self._m_green_x = ((self.green_x_lsb << 8) | self.green_x_msb)
            return self._m_green_x if hasattr(self, '_m_green_x') else None

        @property
        def blue_y(self):
            if hasattr(self, '_m_blue_y'):
                return self._m_blue_y if hasattr(self, '_m_blue_y') else None

            self._m_blue_y = ((self.blue_y_lsb << 8) | self.blue_y_msb)
            return self._m_blue_y if hasattr(self, '_m_blue_y') else None

        @property
        def green_y(self):
            if hasattr(self, '_m_green_y'):
                return self._m_green_y if hasattr(self, '_m_green_y') else None

            self._m_green_y = ((self.green_y_lsb << 8) | self.green_y_msb)
            return self._m_green_y if hasattr(self, '_m_green_y') else None


    class DisplayColorManagement(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.version = self._io.read_u1()
            self.red_a3 = self._io.read_u2le()
            self.red_a2 = self._io.read_u2le()
            self.green_a3 = self._io.read_u2le()
            self.green_a2 = self._io.read_u2le()
            self.blue_a3 = self._io.read_u2le()
            self.blue_a2 = self._io.read_u2le()


    class DescriptorBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.is_dtd = self._io.read_u2le()

        @property
        def dtd(self):
            if hasattr(self, '_m_dtd'):
                return self._m_dtd if hasattr(self, '_m_dtd') else None

            if self.is_dtd != 0:
                _pos = self._io.pos()
                self._io.seek(0)
                self._m_dtd = self._root.Dtd(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_dtd if hasattr(self, '_m_dtd') else None

        @property
        def desc(self):
            if hasattr(self, '_m_desc'):
                return self._m_desc if hasattr(self, '_m_desc') else None

            if self.is_dtd == 0:
                _pos = self._io.pos()
                self._io.seek(3)
                self._m_desc = self._root.Desc(self._io, self, self._root)
                self._io.seek(_pos)

            return self._m_desc if hasattr(self, '_m_desc') else None


    class DescriptorBlocks(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self._raw_descriptor_block = [None] * (4)
            self.descriptor_block = [None] * (4)
            for i in range(4):
                self._raw_descriptor_block[i] = self._io.read_bytes(18)
                io = KaitaiStream(BytesIO(self._raw_descriptor_block[i]))
                self.descriptor_block[i] = self._root.DescriptorBlock(io, self, self._root)



    class TwoByteTimings(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.two_byte_timing = [None] * (8)
            for i in range(8):
                self.two_byte_timing[i] = self._root.TwoByteTiming(self._io, self, self._root)



    class AdditionalStandardTimings(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.version = self._io.read_u1()
            self.mode0640035085 = self._io.read_bits_int(1) != 0
            self.mode0640040085 = self._io.read_bits_int(1) != 0
            self.mode0720040085 = self._io.read_bits_int(1) != 0
            self.mode0640048085 = self._io.read_bits_int(1) != 0
            self.mode0848048060 = self._io.read_bits_int(1) != 0
            self.mode0800060085 = self._io.read_bits_int(1) != 0
            self.mode1024076885 = self._io.read_bits_int(1) != 0
            self.mode1152086485 = self._io.read_bits_int(1) != 0
            self.mode1280076860r = self._io.read_bits_int(1) != 0
            self.mode1280076860 = self._io.read_bits_int(1) != 0
            self.mode1280076875 = self._io.read_bits_int(1) != 0
            self.mode1280076885 = self._io.read_bits_int(1) != 0
            self.mode1280096060 = self._io.read_bits_int(1) != 0
            self.mode1280096085 = self._io.read_bits_int(1) != 0
            self.mode1280102460 = self._io.read_bits_int(1) != 0
            self.mode1280102485 = self._io.read_bits_int(1) != 0
            self.mode1366076860r = self._io.read_bits_int(1) != 0
            self.mode1366076860 = self._io.read_bits_int(1) != 0
            self.mode1440090060r = self._io.read_bits_int(1) != 0
            self.mode1440090075 = self._io.read_bits_int(1) != 0
            self.mode1440090085 = self._io.read_bits_int(1) != 0
            self.mode1440105060r = self._io.read_bits_int(1) != 0
            self.mode1440105060 = self._io.read_bits_int(1) != 0
            self.mode1440105075 = self._io.read_bits_int(1) != 0
            self.mode1440105085 = self._io.read_bits_int(1) != 0
            self.mode1680105060r = self._io.read_bits_int(1) != 0
            self.mode1680105060 = self._io.read_bits_int(1) != 0
            self.mode1680105075 = self._io.read_bits_int(1) != 0
            self.mode1680105085 = self._io.read_bits_int(1) != 0
            self.mode1600120060 = self._io.read_bits_int(1) != 0
            self.mode1600120065 = self._io.read_bits_int(1) != 0
            self.mode1600120070 = self._io.read_bits_int(1) != 0
            self.mode1600120075 = self._io.read_bits_int(1) != 0
            self.mode1600120085 = self._io.read_bits_int(1) != 0
            self.mode1792134460 = self._io.read_bits_int(1) != 0
            self.mode1792134475 = self._io.read_bits_int(1) != 0
            self.mode1856139260 = self._io.read_bits_int(1) != 0
            self.mode1856139275 = self._io.read_bits_int(1) != 0
            self.mode1920120060r = self._io.read_bits_int(1) != 0
            self.mode1920120060 = self._io.read_bits_int(1) != 0
            self.mode1920120075 = self._io.read_bits_int(1) != 0
            self.mode1920120085 = self._io.read_bits_int(1) != 0
            self.mode1920144060 = self._io.read_bits_int(1) != 0
            self.mode1920144075 = self._io.read_bits_int(1) != 0
            self._io.align_to_byte()
            self.unused = self._io.read_bytes(6)


    class Cea861SpeakerDataBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.reserved1 = self._io.read_bits_int(1) != 0
            self.rear_center_lr = self._io.read_bits_int(1) != 0
            self.front_center_lr = self._io.read_bits_int(1) != 0
            self.rear_center = self._io.read_bits_int(1) != 0
            self.rear_lr = self._io.read_bits_int(1) != 0
            self.front_center = self._io.read_bits_int(1) != 0
            self.lfe = self._io.read_bits_int(1) != 0
            self.front_lr = self._io.read_bits_int(1) != 0
            self._io.align_to_byte()
            self.reserved2 = self._io.read_u1()
            self.reserved3 = self._io.read_u1()


    class Cea861Dtd(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.is_dtd = self._io.read_u2le()

        @property
        def dtd(self):
            if hasattr(self, '_m_dtd'):
                return self._m_dtd if hasattr(self, '_m_dtd') else None

            if self.is_dtd != 0:
                _pos = self._io.pos()
                self._io.seek(0)
                self._raw__m_dtd = self._io.read_bytes(18)
                io = KaitaiStream(BytesIO(self._raw__m_dtd))
                self._m_dtd = self._root.Dtd(io, self, self._root)
                self._io.seek(_pos)

            return self._m_dtd if hasattr(self, '_m_dtd') else None


    class Cea861AudioDataBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.descriptors = []
            i = 0
            while not self._io.is_eof():
                self.descriptors.append(self._root.Cea861AudioDescriptor(self._io, self, self._root))
                i += 1



    class BasicParams(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.input_type = self._root.InputType(self._io.read_bits_int(1))
            if self.input_type == self._root.InputType.analog:
                self.video_levels = self._root.VideoLevels(self._io.read_bits_int(2))

            if self.input_type == self._root.InputType.analog:
                self.black_to_black = self._io.read_bits_int(1) != 0

            if self.input_type == self._root.InputType.analog:
                self.separate_sync = self._io.read_bits_int(1) != 0

            if self.input_type == self._root.InputType.analog:
                self.composite_sync = self._io.read_bits_int(1) != 0

            if self.input_type == self._root.InputType.analog:
                self.sync_on_green = self._io.read_bits_int(1) != 0

            if self.input_type == self._root.InputType.analog:
                self.serrated_vsync = self._io.read_bits_int(1) != 0

            if self.input_type == self._root.InputType.digital:
                self.bit_depth = self._root.BitDepth(self._io.read_bits_int(3))

            if self.input_type == self._root.InputType.digital:
                self.video_interface = self._root.VideoInterface(self._io.read_bits_int(4))

            self._io.align_to_byte()
            self.raw_width = self._io.read_u1()
            self.raw_height = self._io.read_u1()
            self.raw_gamma = self._io.read_u1()
            self.dpms_standby = self._io.read_bits_int(1) != 0
            self.dpms_suspend = self._io.read_bits_int(1) != 0
            self.dpms_active_off = self._io.read_bits_int(1) != 0
            if self.input_type == self._root.InputType.digital:
                self.digital_display_type = self._root.DigitalDisplayType(self._io.read_bits_int(2))

            if self.input_type == self._root.InputType.analog:
                self.analog_display_type = self._root.AnalogDisplayType(self._io.read_bits_int(2))


        @property
        def width(self):
            if hasattr(self, '_m_width'):
                return self._m_width if hasattr(self, '_m_width') else None

            if  ((self.raw_height != 0) and (self.raw_width != 0)) :
                self._m_width = self.raw_width

            return self._m_width if hasattr(self, '_m_width') else None

        @property
        def height(self):
            if hasattr(self, '_m_height'):
                return self._m_height if hasattr(self, '_m_height') else None

            if  ((self.raw_height != 0) and (self.raw_width != 0)) :
                self._m_height = self.raw_height

            return self._m_height if hasattr(self, '_m_height') else None

        @property
        def aspect(self):
            if hasattr(self, '_m_aspect'):
                return self._m_aspect if hasattr(self, '_m_aspect') else None

            self._m_aspect = ((0.0 if self.raw_width == 0 else ((self.raw_width + 100.0) / 100.0)) if self.raw_height == 0 else ((100.0 / (self.raw_width + 100.0)) if self.raw_width == 0.0 else ((self.raw_width + 0.0) / (self.raw_height + 0.0))))
            return self._m_aspect if hasattr(self, '_m_aspect') else None

        @property
        def gamma(self):
            if hasattr(self, '_m_gamma'):
                return self._m_gamma if hasattr(self, '_m_gamma') else None

            self._m_gamma = ((self.raw_gamma + 100.0) / 100.0)
            return self._m_gamma if hasattr(self, '_m_gamma') else None


    class Cea861DataBlock(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.type = self._root.Cea861DataBlockType(self._io.read_bits_int(3))
            self.size = self._io.read_bits_int(5)
            self._io.align_to_byte()
            if self.type == self._root.Cea861DataBlockType.audio:
                self._raw_audio_block = self._io.read_bytes(self.size)
                io = KaitaiStream(BytesIO(self._raw_audio_block))
                self.audio_block = self._root.Cea861AudioDataBlock(io, self, self._root)

            if self.type == self._root.Cea861DataBlockType.video:
                self._raw_video_block = self._io.read_bytes(self.size)
                io = KaitaiStream(BytesIO(self._raw_video_block))
                self.video_block = self._root.Cea861VideoDataBlock(io, self, self._root)

            if self.type == self._root.Cea861DataBlockType.vendor_specific:
                self._raw_vendor_block = self._io.read_bytes(self.size)
                io = KaitaiStream(BytesIO(self._raw_vendor_block))
                self.vendor_block = self._root.Cea861VendorDataBlock(io, self, self._root)

            if self.type == self._root.Cea861DataBlockType.speaker_allocation:
                self._raw_speaker_block = self._io.read_bytes(self.size)
                io = KaitaiStream(BytesIO(self._raw_speaker_block))
                self.speaker_block = self._root.Cea861SpeakerDataBlock(io, self, self._root)



    class Header(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.magic = self._io.ensure_fixed_contents(b"\x00\xFF\xFF\xFF\xFF\xFF\xFF\x00")
            self.manu_id = self._root.ManuId(self._io, self, self._root)
            self.product_code = self._io.read_u2le()
            self.serial = self._io.read_u4le()
            self.week = self._io.read_u1()
            self.raw_year = self._io.read_u1()
            self.vermaj = self._io.read_u1()
            self.vermin = self._io.read_u1()

        @property
        def year(self):
            if hasattr(self, '_m_year'):
                return self._m_year if hasattr(self, '_m_year') else None

            if self.week != 255:
                self._m_year = (self.raw_year + 1990)

            return self._m_year if hasattr(self, '_m_year') else None

        @property
        def model_year(self):
            if hasattr(self, '_m_model_year'):
                return self._m_model_year if hasattr(self, '_m_model_year') else None

            if self.week == 255:
                self._m_model_year = (self.raw_year + 1990)

            return self._m_model_year if hasattr(self, '_m_model_year') else None


    class Cvt(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.vermaj = self._io.read_bits_int(4)
            self.vermin = self._io.read_bits_int(4)
            self.raw_additional_clock_precision = self._io.read_bits_int(6)
            self.max_width_msb = self._io.read_bits_int(2)
            self._io.align_to_byte()
            self.max_width_lsb = self._io.read_u1()
            self.aspect0403 = self._io.read_bits_int(1) != 0
            self.aspect1609 = self._io.read_bits_int(1) != 0
            self.aspect1610 = self._io.read_bits_int(1) != 0
            self.aspect0504 = self._io.read_bits_int(1) != 0
            self.aspect1509 = self._io.read_bits_int(1) != 0
            self.reserved1 = self._io.read_bits_int(3)
            self.preferred_aspect = self._root.PreferredAspect(self._io.read_bits_int(3))
            self.reduced_blanking = self._io.read_bits_int(1) != 0
            self.standard_blanking = self._io.read_bits_int(1) != 0
            self.reserved2 = self._io.read_bits_int(3)
            self.horizontal_shrink = self._io.read_bits_int(1) != 0
            self.horizontal_stretch = self._io.read_bits_int(1) != 0
            self.vertical_shrink = self._io.read_bits_int(1) != 0
            self.vertical_stretch = self._io.read_bits_int(1) != 0
            self.reserved3 = self._io.read_bits_int(4)
            self._io.align_to_byte()
            self.preferred_vertical_refresh = self._io.read_u1()

        @property
        def additional_clock_precision(self):
            if hasattr(self, '_m_additional_clock_precision'):
                return self._m_additional_clock_precision if hasattr(self, '_m_additional_clock_precision') else None

            self._m_additional_clock_precision = (self.raw_additional_clock_precision * 0.25)
            return self._m_additional_clock_precision if hasattr(self, '_m_additional_clock_precision') else None

        @property
        def max_width(self):
            if hasattr(self, '_m_max_width'):
                return self._m_max_width if hasattr(self, '_m_max_width') else None

            self._m_max_width = ((self.max_width_msb << 8) | self.max_width_lsb)
            return self._m_max_width if hasattr(self, '_m_max_width') else None


    class TwoByteTiming(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.xres = self._io.read_u1()
            self.aspect = self._root.TimingAspect(self._io.read_bits_int(2))
            self.raw_rate = self._io.read_bits_int(6)

        @property
        def invalid(self):
            if hasattr(self, '_m_invalid'):
                return self._m_invalid if hasattr(self, '_m_invalid') else None

            self._m_invalid =  ((self.xres == 1) and (self.aspect == self._root.TimingAspect.aspect1610) and (self.raw_rate == 1)) 
            return self._m_invalid if hasattr(self, '_m_invalid') else None

        @property
        def width(self):
            if hasattr(self, '_m_width'):
                return self._m_width if hasattr(self, '_m_width') else None

            self._m_width = ((self.xres + 31) * 8)
            return self._m_width if hasattr(self, '_m_width') else None

        @property
        def rate(self):
            if hasattr(self, '_m_rate'):
                return self._m_rate if hasattr(self, '_m_rate') else None

            self._m_rate = (self.raw_rate + 60)
            return self._m_rate if hasattr(self, '_m_rate') else None


    class Cea861(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.type = self._root.ExtensionType(self._io.read_u1())
            self.version = self._io.read_u1()
            self.dtd_pos = self._io.read_u1()
            self.underscan = self._io.read_bits_int(1) != 0
            self.basic_audio = self._io.read_bits_int(1) != 0
            self.ycbcr_444 = self._io.read_bits_int(1) != 0
            self.ycbcr_422 = self._io.read_bits_int(1) != 0
            self.native_dtds = self._io.read_bits_int(4)
            self._io.align_to_byte()
            if self.dtd_pos > 4:
                self._raw_data_block_collection = self._io.read_bytes((self.dtd_pos - 4))
                io = KaitaiStream(BytesIO(self._raw_data_block_collection))
                self.data_block_collection = self._root.Cea861DataBlocks(io, self, self._root)


        @property
        def dtd_collection(self):
            if hasattr(self, '_m_dtd_collection'):
                return self._m_dtd_collection if hasattr(self, '_m_dtd_collection') else None

            if self.dtd_pos >= 4:
                _pos = self._io.pos()
                self._io.seek(self.dtd_pos)
                self._raw__m_dtd_collection = self._io.read_bytes_full()
                io = KaitaiStream(BytesIO(self._raw__m_dtd_collection))
                self._m_dtd_collection = self._root.Cea861Dtds(io, self, self._root)
                self._io.seek(_pos)

            return self._m_dtd_collection if hasattr(self, '_m_dtd_collection') else None


    class UnknownDescriptor(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_bytes(16)


    class ThreeByteTimings(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.version = self._io.read_u1()
            self.three_byte_timings = [None] * (4)
            for i in range(4):
                self.three_byte_timings[i] = self._root.ThreeByteTiming(self._io, self, self._root)



    class TextDescriptor(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.text = self._io.read_bytes(13)


    class Extension(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.extension_type = self._root.ExtensionType(self._io.read_u1())

        @property
        def cea_861(self):
            if hasattr(self, '_m_cea_861'):
                return self._m_cea_861 if hasattr(self, '_m_cea_861') else None

            if self.extension_type == self._root.ExtensionType.cea_861:
                _pos = self._io.pos()
                self._io.seek(0)
                self._raw__m_cea_861 = self._io.read_bytes(127)
                io = KaitaiStream(BytesIO(self._raw__m_cea_861))
                self._m_cea_861 = self._root.Cea861(io, self, self._root)
                self._io.seek(_pos)

            return self._m_cea_861 if hasattr(self, '_m_cea_861') else None

        @property
        def checksum(self):
            if hasattr(self, '_m_checksum'):
                return self._m_checksum if hasattr(self, '_m_checksum') else None

            _pos = self._io.pos()
            self._io.seek(127)
            self._m_checksum = self._io.read_u1()
            self._io.seek(_pos)
            return self._m_checksum if hasattr(self, '_m_checksum') else None

        @property
        def data(self):
            if hasattr(self, '_m_data'):
                return self._m_data if hasattr(self, '_m_data') else None

            _pos = self._io.pos()
            self._io.seek(0)
            self._m_data = self._io.read_bytes(127)
            self._io.seek(_pos)
            return self._m_data if hasattr(self, '_m_data') else None


    @property
    def data(self):
        if hasattr(self, '_m_data'):
            return self._m_data if hasattr(self, '_m_data') else None

        _pos = self._io.pos()
        self._io.seek(0)
        self._m_data = self._io.read_bytes(127)
        self._io.seek(_pos)
        return self._m_data if hasattr(self, '_m_data') else None


