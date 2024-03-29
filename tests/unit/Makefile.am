include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

#
# Local headers to build against and distribute but not to install
# since they are not part of the package.
#
noinst_HEADERS                                                      = \
    test_util.h                                                       \
    $(NULL)

if GOBO_BUILD_TESTS
# C preprocessor option flags that will apply to all compiled objects in this
# makefile.

AM_CPPFLAGS                                                        	= \
    -I$(top_srcdir)/include                                           \
    -I$(top_srcdir)/src                                               \
    -I$(top_srcdir)/src/core                                          \
    $(NULL)

if GOBO_EXAMPLES_POSIX
AM_CPPFLAGS                                                        += \
    -I$(top_srcdir)/examples/platforms                                \
    $(NULL)
endif

COMMON_LDADD                                                        = \
    $(top_builddir)/src/core/libgobo-core.a                           \
    -lpthread                                                         \
    $(NULL)

if GOBO_ENABLE_BUILTIN_MBEDTLS
COMMON_LDADD                                                       += \
    $(top_builddir)/third_party/mbedtls/libmbedcrypto.a               \
    $(NULL)
endif # GOBO_ENABLE_BUILTIN_MBEDTLS

check_PROGRAMS                                                      = \
    test-heap                                                         \
    test-aes                                                          \
    $(NULL)

# Test applications and scripts that should be built and run when the
# 'check' target is run.

TESTS                                                               = \
    $(check_PROGRAMS)                                                 \
    $(NULL)

# Source, compiler, and linker options for test programs.

test_heap_LDADD             = $(COMMON_LDADD)
test_heap_SOURCES           = test_platform.cpp test_heap.cpp

test_aes_LDADD              = $(COMMON_LDADD)
test_aes_SOURCES            = test_platform.cpp test_aes.cpp

PRETTY_FILES                                                        = \
    $(test_heap_SOURCES)                                              \
    $(test_aes_SOURCES)                                               \
    $(NULL)

if GOBO_BUILD_COVERAGE
CLEANFILES                   = $(wildcard *.gcda *.gcno)
endif # GOBO_BUILD_COVERAGE
endif # GOBO_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
