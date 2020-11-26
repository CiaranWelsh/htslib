########################################################
#   htslib preamble
#

cmake_minimum_required(VERSION 3.16)

set(HTS_VERSION_MAJOR 1)
set(HTS_VERSION_MINOR 11)
set(HTS_VERSION_MICRO 20)
#set(HTS_VERSION_PATCH "g51275bc") # invalid format for cmake
set(VERSION "${HTS_VERSION_MAJOR}.${HTS_VERSION_MINOR}.${HTS_VERSION_MICRO}")

project(htslib-cmake VERSION ${VERSION})


include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckLibraryExists)
include(CheckIncludeFile)
include(CheckFunctionExists)
include(CheckTypeSize)
include(GNUInstallDirs)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# put all runtime targets (exe and dll) into bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# put libraries into lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# archives
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)



########################################################
#   dependencies
#

find_package(BZip2)
find_package(LibLZMA)
find_package(ZLIB)
find_package(CURL)
find_package(Threads)

########################################################
#   make a config.h like the one produced by ./configure
#

# Some docs on autoconf like config file:
#   https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks

#/* Define if libcurl file access is enabled. */
check_include_file("curl/curl.h" HAVE_LIBCURL)

#/* Define if HTSlib should enable GCS support. */
option(ENABLE_GCS "Support for google cloud storage" OFF)

#/* Define if HTSlib should enable plugins. */
option(ENABLE_PLUGINS "Enable htslib plugins" OFF)

#/* Define if HTSlib should enable S3 support. */
option(ENABLE_S3 "Enable Amazon S3 support" OFF)


option(BUILD_TESTS "build htslib tests" OFF)
option(BUILD_BGZIP "build the bgzip program" OFF)
option(BUILD_HTSFILE "build the htsfile program" OFF)
option(BUILD_TABIX "build the tabix program" OFF)

#/* Define if you have the Common Crypto library. */
# need to wait until you do mac before you can figure this one out
set(HAVE_COMMONCRYPTO OFF)
#
#/* Define to 1 if you have the `drand48' function. */
check_symbol_exists(drand48 stdlib.h HAVE_DRAND48)

#/* Define to 1 if you have the `fdatasync' function. */
check_symbol_exists(fdatasync unistd.h HAVE_FDATASYNC)

#/* Define to 1 if you have the `fsync' function. */
check_symbol_exists(fsync unistd.h HAVE_FSYNC)

#/* Define to 1 if you have the `getpagesize' function. */
check_symbol_exists(getpagesize unistd.h HAVE_GETPAGESIZE)

#/* Define to 1 if you have the `gmtime_r' function. */
check_symbol_exists(gmtime_r time.h HAVE_GMTIME_R)

#/* Define if you have libcrypto-style HMAC(). */
check_include_file(openssl/hmac.h HAVE_HMAC)

#/* Define to 1 if you have the <inttypes.h> header file. */
check_include_file(inttypes.h HAVE_INTTYPES_H)

#/* Define to 1 if you have the `bz2' library (-lbz2). */
check_library_exists(bz2 BZ2_bzReadOpen ${BZIP2_LIBRARIES} HAVE_LIBBZ2)

#/* Define if libdeflate is available. */
find_library(DEFLATE_LIBRARY
        NAMES ${CMAKE_SHARED_LIBRARY_PREFIX}deflate${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
MESSAGE(STATUS "DEFLATE_LIBRARY ${DEFLATE_LIBRARY}")
if (${DEFLATE_LIBRARY})
    set(HAVE_LIBDEFLATE TRUE)
endif ()

#/* Define to 1 if you have the `lzma' library (-llzma). */
check_library_exists(lzma lzma_version_number ${LIBLZMA_LIBRARIES} HAVE_LIBLZMA)

#/* Define to 1 if you have the `z' library (-lz). */
check_library_exists(z zlibVersion ${ZLIB_LIBRARIES} HAVE_LIBZ)

#/* Define to 1 if you have the <lzma.h> header file. */
check_include_file(lzma.h HAVE_LZMA_H)

#/* Define to 1 if you have the <memory.h> header file. */
check_include_file(memory.h HAVE_MEMORY_H)

#/* Define to 1 if you have a working `mmap' system call. */
check_symbol_exists(mmap sys/mman.h HAVE_MMAP)

#/* Define to 1 if you have the `srand48_deterministic' function. */
set(HAVE_SRAND48_DETERMINISTIC)
check_symbol_exists(srand48_deterministic stdlib.h HAVE_SRAND48_DETERMINISTIC)

#/* Define to 1 if you have the <stdint.h> header file. */
check_include_file(stdint.h HAVE_STDINT_H)

#/* Define to 1 if you have the <stdlib.h> header file. */
check_include_file(stdlib.h HAVE_STDLIB_H)

#/* Define to 1 if you have the <strings.h> header file. */
check_include_file(strings.h HAVE_STRINGS_H)

#/* Define to 1 if you have the <string.h> header file. */
check_include_file(string.h HAVE_STRING_H)

#/* Define to 1 if you have the <sys/param.h> header file. */
check_include_file(sys/param.h HAVE_SYS_PARAM_H)

#/* Define to 1 if you have the <sys/stat.h> header file. */
check_include_file(sys/stat.h HAVE_SYS_STAT_H)

#/* Define to 1 if you have the <sys/types.h> header file. */
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

#/* Define to 1 if you have the <unistd.h> header file. */
check_include_file(unistd.h HAVE_UNISTD_H)

#/* Define to the address where bug reports for this package should be sent. */
set(PACKAGE_BUGREPORT)

#/* Define to the full name of this package. */
set(PACKAGE_NAME htslib-cmake)

#/* Define to the full name and version of this package. */
set(PACKAGE_STRING "${PROJECT_NAME}-${PROJECT_VERSION}")

#/* Define to the one symbol short name of this package. */
set(PACKAGE_TARNAME)
#
#/* Define to the home page for this package. */
set(PACKAGE_URL)
#
#/* Define to the version of this package. */
set(PACKAGE_VERSION ${PROJECT_VERSION})
#
#/* Platform-dependent plugin filename extension. */
set(PLUGIN_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})

#/* Define to 1 if you have the ANSI C header files. */
check_include_file(math.h STDC_HEADERS)

#
#/* Enable large inode numbers on Mac OS X 10.5.  */
##ifndef _DARWIN_USE_64_BIT_INODE
## define _DARWIN_USE_64_BIT_INODE 1
##endif
#
#/* Number of bits in a file offset, on hosts where this is settable. */
set(_FILE_OFFSET_BITS)
#
#/* Define for large files, on AIX-style hosts. */
set(_LARGE_FILES)
#
#/* Needed for PTHREAD_MUTEX_RECURSIVE */
set(_XOPEN_SOURCE)

# Added by Ciaran Welsh for windows

## check to see if ssize_t exists. On linux yes, windows no.
check_type_size(ssize_t SSIZE_T LANGUAGE C)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

set(HTS_VERSION_TEXT ${PROJECT_VERSION})
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

########################################################
#   Build
#


set(
        SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/bcf_sr_sort.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/bgzf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/errmod.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/faidx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/header.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_gcs.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_libcurl.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_net.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_s3.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_s3_write.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hts.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hts_os.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/kfunc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/knetfile.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/kstring.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/md5.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/multipart.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/plugin.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/probaln.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/realn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/regidx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/region.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/sam.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/synced_bcf_reader.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/tabix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/tbx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/textutils.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcf_sweep.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcfutils.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ssize_t_typedef.h"
)

set(
        HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/bcf_sr_sort.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/header.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/hfile_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/hts_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/sam_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/textutils_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thread_pool_internal.h"
)

set(
        CRAM_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_codecs.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_decode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_encode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_external.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_index.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_io.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_samtools.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_stats.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/mFILE.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/open_trace_file.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/pooled_alloc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/rANS_static.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/string_alloc.c"
)

set(
        CRAM_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_codecs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_decode.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_encode.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_index.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_io.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_samtools.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_stats.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/cram_structs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/mFILE.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/misc.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/open_trace_file.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/os.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/pooled_alloc.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/rANS_byte.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/rANS_static.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cram/string_alloc.h"
)

set(HTS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/htslib")
add_library(hts-static STATIC hfile_s3.c ${SOURCE_FILES} ${HEADER_FILES} ${CRAM_SOURCE_FILES} ${CRAM_HEADER_FILES})
target_include_directories(hts-static PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cram>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${HTS_INCLUDE_DIR}>
        )

set(LINK_TARGETS)
if("$<TARGET_EXISTS:BZip2::BZip2>")
    set(LINK_TARGETS ${LINK_TARGETS} BZip2::BZip2)
endif()
if("$<TARGET_EXISTS:LibLZMA::LibLZMA>")
    set(LINK_TARGETS ${LINK_TARGETS} LibLZMA::LibLZMA)
endif()
if("$<TARGET_EXISTS:ZLIB::ZLIB>")
    set(LINK_TARGETS ${LINK_TARGETS} ZLIB::ZLIB)
endif()
if("$<TARGET_EXISTS:CURL::libcurl>")
    set(LINK_TARGETS ${LINK_TARGETS} CURL::libcurl)
endif()
if("$<TARGET_EXISTS:Threads::Threads>")
    set(LINK_TARGETS ${LINK_TARGETS} Threads::Threads)
endif()

message(STATUS "Found link targets: ${LINK_TARGETS}")

target_link_libraries(hts-static PUBLIC ${LINK_TARGETS})

add_library(hts SHARED hfile_s3.c ${SOURCE_FILES} ${HEADER_FILES} ${CRAM_SOURCE_FILES} ${CRAM_HEADER_FILES})
target_include_directories(hts PUBLIC
        $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
target_link_libraries(hts PUBLIC $<TARGET_PROPERTY:hts-static,LINK_LIBRARIES>)
set_target_properties(hts PROPERTIES
    SOVERSION ${PROJECT_VERSION}
)
install(TARGETS hts hts-static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT htslib)
install(DIRECTORY htslib DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT htslib)
install(FILES bgzip.1 htsfile.1 tabix.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1 COMPONENT htslib)
install(FILES faidx.5 sam.5 vcf.5 DESTINATION ${CMAKE_INSTALL_MANDIR}/man5 COMPONENT htslib)
install(FILES  htslib-s3-plugin.7 DESTINATION ${CMAKE_INSTALL_MANDIR}/man7 COMPONENT htslib)

set(BGZIP_SOURCES bgzip.c bgzf.c htslib/bgzf.h htslib/hts.h)
set(HTSFILE_SOURCES htsfile.c htslib/hts.h htslib/sam.h htslib/vcf.h)
set(TABIX_SOURCES tabix.c htslib/tbx.h htslib/sam.h htslib/vcf.h
        htslib/kseq.h htslib/bgzf.h htslib/hts.h
        htslib/regidx.h htslib/hts_defs.h htslib/hts_log.h)

if (BUILD_BGZIP)
    add_executable(bgzip ${BGZIP_SOURCES})
    target_include_directories(bgzip PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(bgzip PRIVATE hts-static)
    install(TARGETS bgzip DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bgzip)
endif ()

if (BUILD_HTSFILE)
    add_executable(htsfile ${HTSFILE_SOURCES})
    target_include_directories(htsfile PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(htsfile PRIVATE hts-static)
    install(TARGETS htsfile DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT htsfile)
endif ()

if (BUILD_TABIX)
    add_executable(tabix ${TABIX_SOURCES})
    target_include_directories(tabix PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(tabix PRIVATE hts-static)
    install(TARGETS tabix DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT tabix)
endif ()


if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()