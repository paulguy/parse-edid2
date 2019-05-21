meta:
  id: edid
  endian: le
seq:
  - id: header
    type: header
  - id: basic_params
    type: basic_params
  - id: chromaticity
    type: chromaticity
  - id: modes
    type: modes
  - id: two_byte_timings
    type: two_byte_timings
  - id: descriptor_blocks
    type: descriptor_blocks
  - id: extensions
    type: u1
  - id: checksum
    type: u1
  - id: extension
    type: extension
    size: 128
    repeat: expr
    repeat-expr: extensions
instances:
  data:
    pos: 0
    size: 127
types:
  manu_id:
    seq:
      - id: pad
        type: b1
      - id: letter
        type: b5
        enum: fivebitlettermap
        repeat: expr
        repeat-expr: 3
  header:
    seq:
      - id: magic
        contents: [0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00]
      - id: manu_id
        type: manu_id
      - id: product_code
        type: u2
      - id: serial
        type: u4
      - id: week
        type: u1
      - id: raw_year
        type: u1
      - id: vermaj
        type: u1
      - id: vermin
        type: u1
    instances:
      year:
        value: raw_year + 1990
        if: week != 255
      model_year:
        value: raw_year + 1990
        if: week == 255
  basic_params:
    seq:
      - id: input_type
        type: b1
        enum: input_type
      - id: video_levels
        type: b2
        enum: video_levels
        if: input_type == input_type::analog
        doc: Video white and sync levels, relative to blank
      - id: black_to_black
        type: b1
        if: input_type == input_type::analog
        doc: Blank-to-black setup (pedestal) expected
      - id: separate_sync
        type: b1
        if: input_type == input_type::analog
        doc: Separate sync supported
      - id: composite_sync
        type: b1
        if: input_type == input_type::analog
        doc: Composite sync (on HSync) supported
      - id: sync_on_green
        type: b1
        if: input_type == input_type::analog
        doc: Sync on green supported
      - id: serrated_vsync
        type: b1
        if: input_type == input_type::analog
        doc: VSync pulse must be serrated when composite or sync-on-green is used.
      - id: bit_depth
        type: b3
        enum: bit_depth
        if: input_type == input_type::digital
      - id: video_interface
        type: b4
        enum: video_interface
        if: input_type == input_type::digital
      - id: raw_width
        type: u1
      - id: raw_height
        type: u1
      - id: raw_gamma
        type: u1
      - id: dpms_standby
        type: b1
      - id: dpms_suspend
        type: b1
      - id: dpms_active_off
        type: b1
      - id: digital_display_type
        type: b2
        enum: digital_display_type
        if: input_type == input_type::digital
      - id: analog_display_type
        type: b2
        enum: analog_display_type
        if: input_type == input_type::analog
    instances:
      width:
        value: raw_width
        if: raw_height != 0 and raw_width != 0
      height:
        value: raw_height
        if: raw_height != 0 and raw_width != 0
      aspect:
        value: |
          raw_height == 0 ? 
            (raw_width == 0 ?
              0.0
            :
              (raw_width + 100.0) / 100.0)
          :
            (raw_width == 0.0 ?
              100.0 / (raw_width + 100.0)
            :
              (raw_width + 0.0) / (raw_height + 0.0))
      gamma:
        value: (raw_gamma + 100.0) / 100.0
  chromaticity:
    seq:
      - id: red_x_lsb
        type: b2
      - id: red_y_lsb
        type: b2
      - id: green_x_lsb
        type: b2
      - id: green_y_lsb
        type: b2
      - id: blue_x_lsb
        type: b2
      - id: blue_y_lsb
        type: b2
      - id: white_x_lsb
        type: b2
      - id: white_y_lsb
        type: b2
      - id: red_x_msb
        type: u1
      - id: red_y_msb
        type: u1
      - id: green_x_msb
        type: u1
      - id: green_y_msb
        type: u1
      - id: blue_x_msb
        type: u1
      - id: blue_y_msb
        type: u1
      - id: white_x_msb
        type: u1
      - id: white_y_msb
        type: u1
    instances:
      red_x:
        value: red_x_lsb << 8 | red_x_msb
      red_y:
        value: red_y_lsb << 8 | red_y_msb
      green_x:
        value: green_x_lsb << 8 | green_x_msb
      green_y:
        value: green_y_lsb << 8 | green_y_msb
      blue_x:
        value: blue_x_lsb << 8 | blue_x_msb
      blue_y:
        value: blue_y_lsb << 8 | blue_y_msb
      white_x:
        value: white_x_lsb << 8 | white_x_msb
      white_y:
        value: white_y_lsb << 8 | white_y_msb
  modes:
    seq:
      - id: mode0720040070
        type: b1
      - id: mode0720040088
        type: b1
      - id: mode0640048060
        type: b1
      - id: mode0640048067
        type: b1
      - id: mode0640048072
        type: b1
      - id: mode0640048075
        type: b1
      - id: mode0800060056
        type: b1
      - id: mode0800060060
        type: b1

      - id: mode0800060072
        type: b1
      - id: mode0800060075
        type: b1
      - id: mode0832062475
        type: b1
      - id: mode1024076887i
        type: b1
      - id: mode1024060060
        type: b1
      - id: mode1024076870
        type: b1
      - id: mode1024076875
        type: b1
      - id: mode1280102475
        type: b1

      - id: mode1152087075
        type: b1
      - id: other_modes
        type: b7
  two_byte_timing:
    seq:
      - id: xres
        type: u1
      - id: aspect
        type: b2
        enum: timing_aspect
      - id: raw_rate
        type: b6
    instances:
      invalid:
        value: (xres == 1 and aspect == timing_aspect::aspect1610 and raw_rate == 1)
      width:
        value: (xres + 31) * 8
      rate:
        value: raw_rate + 60
  two_byte_timings:
    seq:
      - id: two_byte_timing
        type: two_byte_timing
        repeat: expr
        repeat-expr: 8
  dtd:
    seq:
      - id: raw_pixel_clock
        type: u2
      - id: hactive_lsb
        type: u1
      - id: hblank_lsb
        type: u1
      - id: hactive_msb
        type: b4
      - id: hblank_msb
        type: b4
      - id: vactive_lsb
        type: u1
      - id: vblank_lsb
        type: u1
      - id: vactive_msb
        type: b4
      - id: vblank_msb
        type: b4
      - id: hfront_porch_lsb
        type: u1
      - id: hsync_pulse_lsb
        type: u1
      - id: vfront_porch_lsb
        type: b4
      - id: vsync_pulse_lsb
        type: b4
      - id: hfront_porch_msb
        type: b2
      - id: hsync_pulse_msb
        type: b2
      - id: vfront_porch_msb
        type: b2
      - id: vsync_pulse_msb
        type: b2
      - id: width_lsb
        type: u1
      - id: height_lsb
        type: u1
      - id: width_msb
        type: b4
      - id: height_msb
        type: b4
      - id: hborder
        type: u1
      - id: vborder
        type: u1
      - id: interlaced
        type: b1
      - id: stereo_hi_bits
        type: b2
      - id: sync_signal_type
        type: b1
        enum: sync_signal_type
      - id: analog_sync_type
        type: b1
        enum: analog_sync_type
        if: sync_signal_type == sync_signal_type::analog
      - id: analog_vsync_serration
        type: b1
        if: sync_signal_type == sync_signal_type::analog
        doc: HSync during VSync
      - id: sync_on_all
        type: b1
        if: sync_signal_type == sync_signal_type::analog
        doc: otherwise sync only on green
      - id: digital_sync_type
        type: b1
        enum: digital_sync_type
        if: sync_signal_type == sync_signal_type::digital
      - id: digital_vsync_polarity
        type: b1
        enum: sync_polarity
        if: sync_signal_type == sync_signal_type::digital and digital_sync_type == digital_sync_type::composite
      - id: digital_sync_reserved
        type: b1
        if: sync_signal_type == sync_signal_type::digital and digital_sync_type == digital_sync_type::composite
      - id: digital_vsync_serration
        type: b1
        if: sync_signal_type == sync_signal_type::digital and digital_sync_type == digital_sync_type::separate
        doc: HSync during VSync
      - id: digital_hsync_polarity
        type: b1
        enum: sync_polarity
        if: sync_signal_type == sync_signal_type::digital and digital_sync_type == digital_sync_type::separate
      - id: stereo_lo_bit
        type: b1
    instances:
      pixel_clock:
        value: raw_pixel_clock / 100.0
      hactive:
        value: hactive_msb << 8 | hactive_lsb
      hblank:
        value: hblank_msb << 8 | hblank_lsb
      vactive:
        value: vactive_msb << 8 | vactive_lsb
      vblank:
        value: vblank_msb << 8 | vblank_lsb
      hfront_porch:
        value: hfront_porch_msb << 8 | hfront_porch_lsb
      hsync_pulse:
        value: hsync_pulse_msb << 8 | hsync_pulse_lsb
      vfront_porch:
        value: vfront_porch_msb << 4 | vfront_porch_lsb
      vsync_pulse:
        value: vsync_pulse_msb << 4 | vsync_pulse_lsb
      width:
        value: width_msb << 8 | width_lsb
      height:
        value: height_msb << 8 | height_lsb
      stereo_mode:
        # this is dumb
        value: "(stereo_hi_bits << 1) + (stereo_lo_bit ? 1 : 0)"
        enum: stereo_mode
  additional_standard_timings:
    seq:
      - id: version
        type: u1
      - id: mode0640035085
        type: b1
      - id: mode0640040085
        type: b1
      - id: mode0720040085
        type: b1
      - id: mode0640048085
        type: b1
      - id: mode0848048060
        type: b1
      - id: mode0800060085
        type: b1
      - id: mode1024076885
        type: b1
      - id: mode1152086485
        type: b1
  
      - id: mode1280076860r
        type: b1
      - id: mode1280076860
        type: b1
      - id: mode1280076875
        type: b1
      - id: mode1280076885
        type: b1
      - id: mode1280096060
        type: b1
      - id: mode1280096085
        type: b1
      - id: mode1280102460
        type: b1
      - id: mode1280102485
        type: b1

      - id: mode1366076860r
        type: b1
      - id: mode1366076860
        type: b1
      - id: mode1440090060r
        type: b1
      - id: mode1440090075
        type: b1
      - id: mode1440090085
        type: b1
      - id: mode1440105060r
        type: b1
      - id: mode1440105060
        type: b1
      - id: mode1440105075
        type: b1

      - id: mode1440105085
        type: b1
      - id: mode1680105060r
        type: b1
      - id: mode1680105060
        type: b1
      - id: mode1680105075
        type: b1
      - id: mode1680105085
        type: b1
      - id: mode1600120060
        type: b1
      - id: mode1600120065
        type: b1
      - id: mode1600120070
        type: b1

      - id: mode1600120075
        type: b1
      - id: mode1600120085
        type: b1
      - id: mode1792134460
        type: b1
      - id: mode1792134475
        type: b1
      - id: mode1856139260
        type: b1
      - id: mode1856139275
        type: b1
      - id: mode1920120060r
        type: b1
      - id: mode1920120060
        type: b1

      - id: mode1920120075
        type: b1
      - id: mode1920120085
        type: b1
      - id: mode1920144060
        type: b1
      - id: mode1920144075
        type: b1

      - id: unused
        size: 6
  three_byte_timing:
    seq:
      - id: addressable_lines_lsb
        type: u1
      - id: addressable_lines_msb
        type: b4
      - id: preferred_vertical_rate
        type: b2
        enum: preferred_vertical_rate
      - id: unused
        type: b3
      - id: aspect_ratio
        type: b2
        enum: timing_aspect
      - id: hz_50
        type: b1
      - id: hz_60
        type: b1
      - id: hz_75
        type: b1
      - id: hz_85
        type: b1
      - id: hz_60r
        type: b1
    instances:
      addressable_lines:
        value: addressable_lines_msb << 8 | addressable_lines_lsb
  three_byte_timings:
    seq:
      - id: version
        type: u1
      - id: three_byte_timings
        type: three_byte_timing
        repeat: expr
        repeat-expr: 4
  display_color_management:
    seq:
      - id: version
        type: u1
      - id: red_a3
        type: u2
      - id: red_a2
        type: u2
      - id: green_a3
        type: u2
      - id: green_a2
        type: u2
      - id: blue_a3
        type: u2
      - id: blue_a2
        type: u2
  additional_two_byte_timings:
    seq:
      - id: two_byte_timings
        type: two_byte_timing
        repeat: expr
        repeat-expr: 6
      - id: unused
        type: u1
  white_point:
    seq:
      - id: index
        type: u1
      - id: unused
        type: b4
      - id: x_lsb
        type: b2
      - id: y_lsb
        type: b2
      - id: x_msb
        type: u1
      - id: y_msb
        type: u1
      - id: raw_gamma
        type: u1
    instances:
      x:
        value: x_msb << 2 | x_lsb
      y:
        value: y_msb << 2 | y_lsb
      gamma:
        value: (raw_gamma + 100) / 100
  additional_white_points:
    seq:
      - id: white_points
        type: white_point
        repeat: expr
        repeat-expr: 2
      - id: unused
        size: 3
  text_descriptor:
    seq:
      - id: text
        size: 13
  gtf:
    seq:
      - id: reserved
        type: u1
      - id: raw_start
        type: u1
      - id: raw_c
        type: u1
      - id: m
        type: u2
      - id: k
        type: u1
      - id: raw_j
        type: u1
    instances:
      start:
        value: raw_start * 2
      c:
        value: raw_c / 2.0
      j:
        value: raw_j / 2.0
  cvt:
    seq:
      - id: vermaj
        type: b4
      - id: vermin
        type: b4
      - id: raw_additional_clock_precision
        type: b6
      - id: max_width_msb
        type: b2
      - id: max_width_lsb
        type: u1
      - id: aspect0403
        type: b1
      - id: aspect1609
        type: b1
      - id: aspect1610
        type: b1
      - id: aspect0504
        type: b1
      - id: aspect1509
        type: b1
      - id: reserved1
        type: b3
      - id: preferred_aspect
        type: b3
        enum: preferred_aspect
      - id: reduced_blanking
        type: b1
      - id: standard_blanking
        type: b1
      - id: reserved2
        type: b3
      - id: horizontal_shrink
        type: b1
      - id: horizontal_stretch
        type: b1
      - id: vertical_shrink
        type: b1
      - id: vertical_stretch
        type: b1
      - id: reserved3
        type: b4
      - id: preferred_vertical_refresh
        type: u1
    instances:
      additional_clock_precision:
        value: raw_additional_clock_precision * 0.25
      max_width:
        value: max_width_msb << 8 | max_width_lsb
  display_range_limits:
    seq:
      - id: unused
        type: b4
      - id: hmax_msb
        type: b1
      - id: hmin_msb
        type: b1
      - id: vmax_msb
        type: b1
      - id: vmin_msb
        type: b1
      - id: vmin_lsb
        type: u1
      - id: vmax_lsb
        type: u1
      - id: hmin_lsb
        type: u1
      - id: hmax_lsb
        type: u1
      - id: raw_max_pixel_clock
        type: u1
      - id: timing_information_type
        type: u1
        enum: timing_information_types
      - id: gtf
        type: gtf
        if: timing_information_type == timing_information_types::secondary_gtf
      - id: cvt
        type: cvt
        if: timing_information_type == timing_information_types::cvt
    instances:
      minimum_vertical_rate:
        value: "(vmin_msb ? 1 : 0) << 8 | vmin_lsb"
      maximum_vertical_rate:
        value: "(vmax_msb ? 1 : 0) << 8 | vmax_lsb"
      minimum_horizontal_rate:
        value: "(hmin_msb ? 1 : 0) << 8 | hmin_lsb"
      maximum_horizontal_rate:
        value: "(hmax_msb ? 1 : 0) << 8 | hmax_lsb"
      max_pixel_clock:
        value: raw_max_pixel_clock * 10
  unknown_descriptor:
    seq:
      - id: data
        size: 16
  desc:
    seq:
      - id: type
        enum: desc_type
        type: u1
    instances:
      additional_standard_timings:
        type: additional_standard_timings
        pos: 5
        if: type == desc_type::additional_standard_timings
      three_byte_timings:
        type: three_byte_timings
        pos: 5
        if: type == desc_type::three_byte_timings
      display_color_management:
        type: display_color_management
        pos: 5
        if: type == desc_type::display_color_management
      additional_two_byte_timings:
        type: additional_two_byte_timings
        pos: 5
        if: type == desc_type::additional_two_byte_timings
      additional_white_points:
        type: additional_white_points
        pos: 5
        if: type == desc_type::additional_white_points
      display_name:
        type: text_descriptor
        pos: 5
        if: type == desc_type::display_name
      display_range_limits:
        type: display_range_limits
        pos: 4
        if: type == desc_type::display_range_limits
      unspecified_text:
        type: text_descriptor
        pos: 5
        if: type == desc_type::unspecified_text
      display_serial:
        type: text_descriptor
        pos: 5
        if: type == desc_type::display_serial
      unknown:
        type: unknown_descriptor
  descriptor_block:
    seq:
      - id: is_dtd
        type: u2
    instances:
      dtd:
        type: dtd
        pos: 0
        if: is_dtd != 0
      desc:
        type: desc
        pos: 3
        if: is_dtd == 0
  descriptor_blocks:
    seq:
      - id: descriptor_block
        type: descriptor_block
        size: 18
        repeat: expr
        repeat-expr: 4
  cea_861_speaker_data_block:
    seq:
      - id: reserved1
        type: b1
      - id: rear_center_lr
        type: b1
      - id: front_center_lr
        type: b1
      - id: rear_center
        type: b1
      - id: rear_lr
        type: b1
      - id: front_center
        type: b1
      - id: lfe
        type: b1
      - id: front_lr
        type: b1
      - id: reserved2
        type: u1
      - id: reserved3
        type: u1
  cea_861_vendor_data_block:
    seq:
      - id: raw_type1
        type: u2
      - id: raw_type2
        type: u1
    instances:
      type:
        value: raw_type1 << 16 | raw_type2
  cea_861_video_data_block:
    seq:
      - id: format
        enum: cea_861_video_format
        type: u1
        repeat: eos
  cea_861_audio_descriptor:
    seq:
      - id: reserved1
        type: b1
      - id: format
        type: b4
        enum: cea_861_audio_format
      - id: raw_channels
        type: b3
      - id: reserved2
        type: b1
      - id: khz_192
        type: b1
      - id: khz_176
        type: b1
      - id: khz_96
        type: b1
      - id: khz_88
        type: b1
      - id: khz_48
        type: b1
      - id: khz_44
        type: b1
      - id: khz_32
        type: b1
      - id: reserved3
        type: b5
        if: format == cea_861_audio_format::lpcm
      - id: bits_24
        type: b1
        if: format == cea_861_audio_format::lpcm
      - id: bits_20
        type: b1
        if: format == cea_861_audio_format::lpcm
      - id: bits_16
        type: b1
        if: format == cea_861_audio_format::lpcm
      - id: max_bitrate
        type: u1
        if: format == cea_861_audio_format::ac3 or format == cea_861_audio_format::mpeg1 or format == cea_861_audio_format::mp3 or format == cea_861_audio_format::mpeg2 or format == cea_861_audio_format::aac or format == cea_861_audio_format::dts or format == cea_861_audio_format::atrac
    instances:
      channels:
        value: raw_channels + 1
  cea_861_audio_data_block:
    seq:
      - id: descriptors
        type: cea_861_audio_descriptor
        repeat: eos
  cea_861_data_block:
    seq:
      - id: type
        enum: cea_861_data_block_type
        type: b3
      - id: size
        type: b5
      - id: audio_block
        type: cea_861_audio_data_block
        size: size
        if: type == cea_861_data_block_type::audio
      - id: video_block
        type: cea_861_video_data_block
        size: size
        if: type == cea_861_data_block_type::video
      - id: vendor_block
        type: cea_861_vendor_data_block
        size: size
        if: type == cea_861_data_block_type::vendor_defined
      - id: speaker_block
        type: cea_861_speaker_data_block
        size: size
        if: type == cea_861_data_block_type::speaker_allocation
  cea_861_data_blocks:
    seq:
      - id: data_blocks
        type: cea_861_data_block
        repeat: eos
  cea_861_dtd:
    seq:
      - id: is_dtd
        type: u2
    instances:
      dtd:
        type: dtd
        size: 18
        pos: 0
        if: is_dtd != 0
  cea_861_dtds:
    seq:
      - id: dtds
        type: cea_861_dtd
        size: 18
        repeat: until
        repeat-until: _.is_dtd == 0
  cea_861:
    seq:
      - id: type
        type: u1
        enum: extension_type
      - id: version
        type: u1
      - id: dtd_pos
        type: u1
      - id: underscan
        type: b1
      - id: basic_audio
        type: b1
      - id: ycbcr_444
        type: b1
      - id: ycbcr_422
        type: b1
      - id: native_dtds
        type: b4
      - id: data_block_collection
        type: cea_861_data_blocks
        size: dtd_pos - 4
        if: dtd_pos > 4
    instances:
      dtd_collection:
        type: cea_861_dtds
        pos: dtd_pos
        size-eos: true
        if: dtd_pos >= 4
  extension:
    seq:
      - id: type
        enum: extension_type
        type: u1
    instances:
      cea_861:
        type: cea_861
        pos: 0
        size: 127
        if: type == extension_type::cea_861
      checksum:
        type: u1
        pos: 127
      data:
        pos: 0
        size: 127
enums:
  fivebitlettermap:
    1: a
    2: b
    3: c
    4: d
    5: e
    6: f
    7: g
    8: h
    9: i
    10: j
    11: k
    12: l
    13: m
    14: n
    15: o
    16: p
    17: q
    18: r
    19: s
    20: t
    21: u
    22: v
    23: w
    24: x
    25: y
    26: z
  input_type:
    0: analog
    1: digital
  video_levels:
    0: v07000300
    1: v07140286
    2: v10000400
    3: v07000000
  bit_depth:
    0: undefined
    1: bits_6
    2: bits_8
    3: bits_10
    4: bits_12
    5: bits_14
    6: bits_16
    7: reserved
  video_interface:
    0: undefined
    1: hdmia
    2: hdmib
    4: mddi
    5: displayport
  digital_display_type:
    0: rgb444
    1: rgb444_ycrcb444
    2: rgb444_ycrcb422
    3: rgb444_ycrcb444_ycrcb422
  analog_display_type:
    0: black_and_white
    1: rgb_color
    2: non_rgb_color
    3: undefined
  timing_aspect:
    0: aspect1610
    1: aspect0403
    2: aspect0504
    3: aspect1609
  sync_signal_type:
    0: analog
    1: digital
  analog_sync_type:
    0: composite
    1: bipolar_composite
  digital_sync_type:
    0: composite
    1: separate
  sync_polarity:
    0: negative
    1: positive
  stereo_mode:
    0: none
    1: invalid
    2: field_sequential
    3: right_image_even
    4: opposite_field_sequential
    5: left_image_even
    6: four_way_interleaved
    7: side_by_side
  desc_type:
    0xF7: additional_standard_timings
    0xF8: three_byte_timings
    0xF9: display_color_management
    0xFA: additional_two_byte_timings
    0xFB: additional_white_points
    0xFC: display_name
    0xFD: display_range_limits
    0xFE: unspecified_text
    0xFF: display_serial
  preferred_vertical_rate:
    0: hz_50
    1: hz_60
    2: hz_75
    3: hz_85
  timing_information_types:
    0: default_gtf
    1: none
    2: secondary_gtf
    4: cvt
  preferred_aspect:
    0: aspect0403
    1: aspect1609
    2: aspect1610
    3: aspect0504
    4: aspect1509
  extension_type:
    2: cea_861
    112: unknown112
  cea_861_data_block_type:
    1: audio
    2: video
    3: vendor_defined
    4: speaker_allocation
  cea_861_audio_format:
    1: lpcm
    2: ac3
    3: mpeg1
    4: mp3
    5: mpeg2
    6: aac
    7: dts
    8: atrac
    9: dsd
    10: ddplus
    11: dtshd
    12: truehd
    13: dst
    14: wmapro
  cea_861_video_format:
    0: unknown0
    1: format_006400480ps060
    2: format_007200480ps060
    3: format_007200480pw060
    4: format_012800720pw060
    5: format_019201080iw060
    6: format_014400480is060
    7: format_014400480iw060
    8: format_014400240ps060
    9: format_014400240pw060
    10: format_028800480is060
    11: format_028800480iw060
    12: format_028800240ps060
    13: format_028800240pw060
    14: format_014400480ps060
    15: format_014400480pw060
    16: format_019201080pw060
    17: format_007200576ps050
    18: format_007200576pw050
    19: format_012800720pw050
    20: format_019201080iw050
    21: format_014400576is050
    22: format_014400576iw050
    23: format_014400288ps050
    24: format_014400288pw050
    25: format_028800576is050
    26: format_028800576iw050
    27: format_028800288ps050
    28: format_028800288pw050
    29: format_014400576ps050
    30: format_014400576pw050
    31: format_019201080pw050
    32: format_019201080pw024
    33: format_019201080pw025
    34: format_019201080pw030
    35: format_028800480ps060
    36: format_028800480pw060
    37: format_028800576ps050
    38: format_028800576pw050
    39: format_019201080iw050b
    40: format_019201080iw100
    41: format_012800720pw100
    42: format_007200576ps100
    43: format_007200576pw100
    44: format_014400576is100
    45: format_014400576iw100
    46: format_019201080iw120
    47: format_012800720pw120
    48: format_007200480ps120
    49: format_007200480pw120
    50: format_014400480is120
    51: format_014400480iw120
    52: format_007200576ps200
    53: format_007200576pw200
    54: format_014400576is200
    55: format_014400576iw200
    56: format_007200480ps240
    57: format_007200480pw240
    58: format_014400480is240
    59: format_014400480iw240
    60: format_012800720pw024
    61: format_012800720pw025
    62: format_012800720pw030
    63: format_019201080pw120
    64: format_019201080pw100
    65: format_012800720pu024
    66: format_012800720pu025
    67: format_012800720pu030
    68: format_012800720pu050
    69: format_012800720pu060
    70: format_012800720pu100
    71: format_012800720pu120
    72: format_019201080pu024
    73: format_019201080pu025
    74: format_019201080pu030
    75: format_019201080pu050
    76: format_019201080pu060
    77: format_019201080pu100
    78: format_019201080pu120
    79: format_016800720pu024
    80: format_016800720pu025
    81: format_016800720pu030
    82: format_016800720pu050
    83: format_016800720pu060
    84: format_016800720pu100
    85: format_016800720pu120
    86: format_025601080pu024
    87: format_025601080pu025
    88: format_025601080pu030
    89: format_025601080pu050
    90: format_025601080pu060
    91: format_025601080pu100
    92: format_025601080pu120
    93: format_038402160pw024
    94: format_038402160pw025
    95: format_038402160pw030
    96: format_038402160pw050
    97: format_038402160pw060
    98: format_040962160pc024
    99: format_040962160pc025
    100: format_040962160pc030
    101: format_040962160pc050
    102: format_040962160pc060
    103: format_038402160pu024
    104: format_038402160pu025
    105: format_038402160pu030
    106: format_038402160pu050
    107: format_038402160pu060
    108: format_012800720pw048
    109: format_012800720pu048
    110: format_016800720pu048
    111: format_019201080pw048
    112: format_019201080pu048
    113: format_025601080pu048
    114: format_038402160pw048
    115: format_040962160pc048
    116: format_038402160pu048
    117: format_038402160pw100
    118: format_038402160pw120
    119: format_038402160pu100
    120: format_038402160pu120
    121: format_051202160pu024
    122: format_051202160pu025
    123: format_051202160pu030
    124: format_051202160pu048
    125: format_051202160pu050
    126: format_051202160pu060
    127: format_051202160pu100
    193: format_051202160pu120
    194: format_076804320pw024
    195: format_076804320pw025
    196: format_076804320pw030
    197: format_076804320pw048
    198: format_076804320pw050
    199: format_076804320pw060
    200: format_076804320pw100
    201: format_076804320pw120
    202: format_076804320pu024
    203: format_076804320pu025
    204: format_076804320pu030
    205: format_076804320pu048
    206: format_076804320pu050
    207: format_076804320pu060
    208: format_076804320pu100
    209: format_076804320pu120
    210: format_102404320pu024
    211: format_102404320pu025
    212: format_102404320pu030
    213: format_102404320pu048
    214: format_102404320pu050
    215: format_102404320pu060
    216: format_102404320pu100
    217: format_102404320pu120
    218: format_040962160pc100
    219: format_040962160pc120

