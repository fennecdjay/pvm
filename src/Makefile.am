lib_LTLIBRARIES = libpvm.la
libpvm_la_SOURCES = encoding/utf8.c \
                    ir/code.c \
                    ir/function.c \
                    ir/instruction.c \
                    ir/opcode.c \
                    ir/pool.c \
                    ir/sourceloctable.c \
                    pbcfile/parser.c \
                    pbcfile/scanner.c \
                    utils/utils.c

libpvm_ladir = $(includedir)/$(FORMATTED_VERSION)
nobase_libpvm_la_HEADERS = encoding/utf8.h \
                    ir/code.h \
                    ir/function.h \
                    ir/instruction.h \
                    ir/opcode.h \
                    ir/pool.h \
                    ir/sourceloctable.h \
                    pbcfile/parser.h \
                    pbcfile/scanner.h \
                    pbcfile/verification/stackemulator.h \
                    utils/common.h \
                    pvm.h

libpvm_la_CFLAGS = -fPIC --shared -licuuc

noinst_HEADERS = utils/utils.h

bin_PROGRAMS = pvm
pvm_SOURCES = main.c
pvm_LDADD = libpvm.la