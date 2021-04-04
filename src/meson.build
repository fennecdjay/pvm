subdirs = [
    'encoding',
    'ir',
    'pbcfile',
    'utils',
    'interp',
]

foreach subdir: subdirs
    subdir(subdir)
endforeach

# Awful but it works

libpvm_sources = []

foreach src: utils_sources
    libpvm_sources += 'utils' / src
endforeach

foreach src: ir_sources
    libpvm_sources += 'ir' / src
endforeach

foreach src: pbcfile_sources
    libpvm_sources += 'pbcfile' / src
endforeach

foreach src: encoding_sources
    libpvm_sources += 'encoding' / src
endforeach

foreach src: interpreter_sources
    libpvm_sources += 'interp' / src
endforeach

libpvm_inst_headers = []

foreach header: utils_inst_headers
    libpvm_inst_headers += 'utils' / header
endforeach

foreach header: ir_inst_headers
    libpvm_inst_headers += 'ir' / header
endforeach

foreach header: pbcfile_inst_headers
    libpvm_inst_headers += 'pbcfile' / header
endforeach

foreach header: encoding_inst_headers
    libpvm_inst_headers += 'encoding' / header
endforeach

foreach header: interpreter_inst_headers
    libpvm_inst_headers += 'interp' / header
endforeach

libpvm_noinst_headers = []

foreach header: utils_noinst_headers
    libpvm_noinst_headers += 'utils' / header
endforeach

foreach header: ir_noinst_headers
    libpvm_noinst_headers += 'ir' / header
endforeach

foreach header: pbcfile_noinst_headers
    libpvm_noinst_headers += 'pbcfile' / header
endforeach

foreach header: encoding_noinst_headers
    libpvm_noinst_headers += 'encoding' / header
endforeach

foreach header: interpreter_noinst_headers
    libpvm_noinst_headers += 'interp' / header
endforeach

libpvm_deps = [
    dependency('icu-uc'),
    dependency('libjit'),
]

libpvm_headers = libpvm_noinst_headers + libpvm_inst_headers
formatted_version = 'pvm-@0@'.format(meson.project_version())

test_cc_args = [
  '-Wall',
  '-Wextra',
  '-Werror',
  '-Wno-unused-function',
  '-Wno-unused-parameter',
  '-Wno-switch',
  '-Wno-pedantic',
  '-g',
]

rt_paths = [
    get_option('prefix') / get_option('datadir') / 'pvm' / 'rt',
]

sepchar = build_machine.system() == 'windows' ? ';' : ':'

conf = configuration_data()
conf.set('default_pbcpath', sepchar.join(rt_paths))
conf.set('is_lil_endian_uwu', build_machine.endian() == 'little' ? 1 : 0)

confh = configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: conf,
)

include_dirs = []
foreach subdir: subdirs
    include_dirs += include_directories(subdir)
endforeach

libpvm_noinst_headers += [confh]

cc = meson.get_compiler('c')
global_cc_args = []

foreach arg: test_cc_args
    if cc.has_multi_arguments(arg)
        global_cc_args += arg
    endif
endforeach

fsize = cc.sizeof('float')
if fsize != 4
    error('float must be 32 bits, it is currently @0@ bits'.format(fsize * 8))
endif

dsize = cc.sizeof('double')
if dsize != 8
    error('double must be 64 bits, it is currently @0@ bits'.format(dsize * 8))
endif

libpvm = shared_library(
    'pvm',
    sources: libpvm_sources + libpvm_headers,
    c_args: global_cc_args,
    dependencies: libpvm_deps,
    include_directories: include_dirs,
    install: true,
)

install_headers(
    libpvm_inst_headers,
    subdir: formatted_version + '/pvm',
)

executable(
    'pvm',
    sources: 'main.c',
    c_args: global_cc_args,
    link_with: libpvm,
    dependencies: libpvm_deps,
    install: true,
)

pkgc = import('pkgconfig')

pkgc.generate(
    libpvm,
    description: 'The Pretzel Virtual Machine',
    filebase: formatted_version,
    subdirs: formatted_version,
    version: meson.project_version(),
)