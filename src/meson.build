pvm_sources = [
    'pbc/byteUtils.c',
    'pbc/imageStream.c',
    'utils/utils.c',
    'utils/asprintf.c',
    'utils/vasprintf-c99.c',
]

pvm_headers = [
    'pbc/byteUtils.h',
    'pbc/imageStream.h',
    'pbc/pbcFile.h',
    'utils/asprintf.h',
    'utils/utils.h',
]

pvm_deps = [
    dependency('icu-uc'),
]

includes = [
    include_directories('.'),
]

test_args = [
  '-Wall',
  '-Wextra',
  '-Werror',
  '-Wno-unused-parameter',
  '-Wno-narrowing',
  '-Wno-uninitialized',
  '-Wno-overflow',
  '-Wno-empty-translation-unit',
  '-g',
]

conf = configuration_data()
conf.set('is_lil_endian_uwu', build_machine.endian() == 'little' ? 1 : 0)

conf_h = configure_file(
    input: 'config.h.in',
    output: 'config.h',
    configuration: conf,
)

cxx = meson.get_compiler('cpp')
cxx_args = []

foreach arg: test_args
    if cxx.has_multi_arguments(arg)
        cxx_args += arg
    endif
endforeach

cc = meson.get_compiler('c')
cc_args = []

foreach arg: test_args
    if cc.has_multi_arguments(arg)
        cc_args += arg
    endif
endforeach

libpvm = shared_library(
    'pvm',
    sources: pvm_sources + pvm_headers,
    dependencies: pvm_deps,
    cpp_args: cxx_args,
    c_args: cc_args,
    include_directories: includes,
    install: true,
)

executable(
    'pvm',
    link_with: libpvm,
    sources: ['main.c'],
    cpp_args: cxx_args,
    c_args: cc_args,
    include_directories: includes,
    install: true,
)
