libpvm_sources = [
    'code.c',
    'function.c',
    'instruction.c',
    'opcode.c',
    'scanner.c',
    'sourceloctable.c',
    'utils.c',
]

libpvm_inst_headers = [
    'code.h',
    'common.h',
    'function.h',
    'instruction.h',
    'opcode.h',
    'scanner.h',
    'sourceloctable.h',
]

libpvm_noinst_headers = [
    'utils.h',
]

libpvm_headers = libpvm_noinst_headers + libpvm_inst_headers
test_cc_args = [
    '-Wall',
    '-Werror',
    '-Wno-pedantic',
    '-Wno-unused-parameter',
]

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
    install: true,
)

install_headers(
    libpvm_inst_headers,
    subdir: 'pvm-@0@/pvm'.format(meson.project_version()),
)

install_headers(
    'pvm.h',
    subdir: 'pvm-@0@'.format(meson.project_version()),
)

executable(
    'pvm',
    sources: 'main.c',
    c_args: global_cc_args,
    link_with: libpvm,
    install: true,
)