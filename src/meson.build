libpvm_sources = [
    'code.c',
    'function.c',
    'instruction.c',
    'opcode.c',
    'parser.c',
    'pool.c',
    'scanner.c',
    'sourceloctable.c',
    'utils.c',
    'utf8.c',
]

libpvm_inst_headers = [
    'code.h',
    'common.h',
    'function.h',
    'instruction.h',
    'opcode.h',
    'parser.h',
    'pool.h',
    'scanner.h',
    'sourceloctable.h',
    'utf8.h',
]

libpvm_noinst_headers = [
    'utils.h',
]

libpvm_deps = [
    dependency('icu-uc'),
]

libpvm_headers = libpvm_noinst_headers + libpvm_inst_headers
formatted_version = 'pvm-@0@'.format(meson.project_version())

test_cc_args = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-Wno-pedantic',
    '-Wno-unused-parameter',
    '-Wno-switch',
    '-Wno-unused-function',
    '-Wno-maybe-uninitialized',
]

conf = configuration_data()
conf.set('is_lil_endian_uwu', build_machine.endian() == 'little' ? 1 : 0)

confh = configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: conf,
)

libpvm_noinst_headers += [confh]

cc = meson.get_compiler('c')
global_cc_args = []

foreach arg: test_cc_args
    if cc.has_multi_arguments(arg)
        global_cc_args += arg
    endif
endforeach

libpvm = shared_library(
    'pvm',
    sources: libpvm_sources + libpvm_headers,
    c_args: global_cc_args,
    dependencies: libpvm_deps,
    install: true,
)

install_headers(
    libpvm_inst_headers,
    subdir: formatted_version + '/pvm',
)

install_headers(
    'pvm.h',
    subdir: formatted_version,
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