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
include(CheckIncludeFileCXX)
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

if (MSVC)
    # get rid of warning:
    #   LINK : warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs;
    #       use /NODEFAULTLIB:library
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" "/NODEFAULTLIB")
endif ()

########################################################
#   dependencies
#

find_package(BZip2 REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

# This was akward. Installed on vcpkg then went to
# D:\vcpkg\installed\x64-windows-static\share\pthread
# to figure out what variables pthreads imports.
# Looks like PThreads4W_LIBRARY_RELEASE holds our release library
if (WIN32)
    find_package(pthreads CONFIG REQUIRED)
else()
    find_package(Threads REQUIRED)

endif ()
# note, it seems that on windows and linux, openssl is a requirement for HMAC
# while on apple we instead need common crypto
find_package(OpenSSL)

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
set(CMAKE_REQUIRED_INCLUDES "${CMAKE_REQUIRED_INCLUDES}" "${OPENSSL_INCLUDE_DIR}")

# This was hard on windows using check_include_file so we
# use the full wack find_path instead.
find_path(HAVE_HMAC
        NAMES
        openssl/hmac.h
        HINTS
        "${OPENSSL_INCLUDE_DIR}"
        )

#/* Define to 1 if you have the <inttypes.h> header file. */
check_include_file(inttypes.h HAVE_INTTYPES_H)

#/* Define to 1 if you have the `bz2' library (-lbz2). */
check_library_exists(bz2 BZ2_bzReadOpen ${BZIP2_LIBRARIES} HAVE_LIBBZ2)

#/* Define if libdeflate is available. */
find_library(DEFLATE_LIBRARY
        NAMES ${CMAKE_SHARED_LIBRARY_PREFIX}deflate${CMAKE_SHARED_LIBRARY_SUFFIX}
        )

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

#/* Define to 1 if you have the unistd. This way is preferable to #ifdef _WIN32 because
# _WIN32 is also defined on WSL. */
check_include_file(unistd.h HAVE_UNISTD_H)

# check to see if we have time.h
check_include_file(time.h HAVE_TIME_H)

# check to see if we have getopt
check_include_file(getopt.h HAVE_GETOPT_H)

# check to see if we have usleep
check_include_file(usleep.h HAVE_USLEEP_H)

# on linux we have include strings while windows its string (no 's')
check_include_file(string.h HAVE_STRING)
check_include_file(strings.h HAVE_STRINGS)

# Check whether we have dirent.h
check_include_file(dirent.h HAVE_DIRENT_H)

# Check whether we have dlfcn.h
check_include_file(dlfcn.h HAVE_DLFCN_H)

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

# check to see if ssize_t exists. On linux yes, windows no.
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


# todo error if no ssl OR apple equivalent. Since one or the other is reuqired.

########################################################
#   Build
#

set(
        SOURCE_FILES
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
        "${CMAKE_CURRENT_SOURCE_DIR}/probaln.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/realn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/regidx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/region.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/sam.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/synced_bcf_reader.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/tbx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcf_sweep.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/vcfutils.c"

        # todo check whether these sources are needed by htslib
        "${CMAKE_CURRENT_SOURCE_DIR}/bcf_sr_sort.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/plugin.c"
        #        "${CMAKE_CURRENT_SOURCE_DIR}/tabix.c" # has main
        "${CMAKE_CURRENT_SOURCE_DIR}/textutils.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/bgzf.c"
)
if (WIN32)
    set(
            SOURCE_FILES ${SOURCE_FILES}
            "${CMAKE_CURRENT_SOURCE_DIR}/_unistd.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/_time.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/_time.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/_usleep.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/_usleep.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/_getopt.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/_getopt.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/_dirent.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/_dlfcn.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/_dlfcn.h"
    )
endif ()
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
set(INCLUDE_DIRECTORIES)
if ("${BZip2_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} BZip2::BZip2)
endif ()
if ("${LibLZMA_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} LibLZMA::LibLZMA)
endif ()
if ("${ZLIB_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} ZLIB::ZLIB)
endif ()
if ("${CURL_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} CURL::libcurl)
endif ()
if (${Threads_FOUND})
    set(LINK_TARGETS ${LINK_TARGETS} Threads::Threads)
endif ()
if ("${OpenSSL_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} OpenSSL::SSL OpenSSL::Crypto)
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${OPENSSL_INCLUDE_DIR}")
endif ()
if ("${PThreads4W_FOUND}")
    set(LINK_TARGETS ${LINK_TARGETS} $<IF:$<CONFIG:Debug>,${PThreads4W_LIBRARY_DEBUG},${PThreads4W_LIBRARY_RELEASE}>)
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${PThreads4W_INCLUDE_DIR}")
endif ()
set(
        INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES}
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

# make static library
target_link_libraries(hts-static PUBLIC ${LINK_TARGETS})
target_include_directories(hts-static PUBLIC ${INCLUDE_DIRECTORIES})
target_compile_definitions(hts-static PUBLIC HTS_STATIC_DEFINE)

# shared library
add_library(hts SHARED hfile_s3.c ${SOURCE_FILES} ${HEADER_FILES} ${CRAM_SOURCE_FILES} ${CRAM_HEADER_FILES})
target_include_directories(hts PUBLIC ${INCLUDE_DIRECTORIES})
target_link_libraries(hts PUBLIC ${LINK_TARGETS})
set_target_properties(hts PROPERTIES SOVERSION ${PROJECT_VERSION})
target_compile_definitions(hts PUBLIC hts_EXPORTS)

include(GenerateExportHeader)
generate_export_header(hts)

install(TARGETS hts hts-static DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT htslib)
install(DIRECTORY htslib DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT htslib)
install(FILES bgzip.1 htsfile.1 tabix.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1 COMPONENT htslib)
install(FILES faidx.5 sam.5 vcf.5 DESTINATION ${CMAKE_INSTALL_MANDIR}/man5 COMPONENT htslib)
install(FILES htslib-s3-plugin.7 DESTINATION ${CMAKE_INSTALL_MANDIR}/man7 COMPONENT htslib)


##########################################################################################
#       Executables
#

set(BGZIP_SOURCES bgzip.c bgzf.c htslib/bgzf.h htslib/hts.h)
set(HTSFILE_SOURCES htsfile.c htslib/hts.h htslib/sam.h htslib/vcf.h)
set(TABIX_SOURCES tabix.c htslib/tbx.h htslib/sam.h htslib/vcf.h
        htslib/kseq.h htslib/bgzf.h htslib/hts.h
        htslib/regidx.h htslib/hts_defs.h htslib/hts_log.h)

if (BUILD_BGZIP)
    add_executable(bgzip ${BGZIP_SOURCES})
    target_include_directories(bgzip PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(bgzip PRIVATE hts-static)
    install(TARGETS bgzip RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bgzip)
endif ()

if (BUILD_HTSFILE)
    add_executable(htsfile ${HTSFILE_SOURCES})
    target_include_directories(htsfile PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(htsfile PRIVATE hts-static)
    install(TARGETS htsfile RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT htsfile)
endif ()

if (BUILD_TABIX)
    add_executable(tabix ${TABIX_SOURCES})
    target_include_directories(tabix PRIVATE $<TARGET_PROPERTY:hts-static,INCLUDE_DIRECTORIES>)
    target_link_libraries(tabix PRIVATE hts-static)
    install(TARGETS tabix RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT tabix)
endif ()


if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
