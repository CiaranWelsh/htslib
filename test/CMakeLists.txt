##################################################################
#   htslib tests
#

# builds the following test executables
#BUILT_TEST_PROGRAMS = \
#	test/hts_endian \
#	test/fieldarith \
#	test/hfile \
#	test/pileup \
#	test/plugins-dlhts \
#	test/sam \
#	test/test_bgzf \
#	test/test_kfunc \
#	test/test_kstring \
#	test/test_realn \
#	test/test-regidx \
#	test/test_str2int \
#	test/test_view \
#	test/test_index \
#	test/test-vcf-api \
#	test/test-vcf-sweep \
#	test/test-bcf-sr \
#	test/fuzz/hts_open_fuzzer.o \
#	test/test-bcf-translate \
#	test/test-parse-reg


#BUILT_THRASH_PROGRAMS = \
#	test/thrash_threads1 \
#	test/thrash_threads2 \
#	test/thrash_threads3 \
#	test/thrash_threads4 \
#	test/thrash_threads5 \
#	test/thrash_threads6 \
#	test/thrash_threads7

# BINARY: Name of the binary for the test unit you are adding
# LINK_WITH_GOOGLETEST: ON or OFF.
# OUT_VARIABLE: test target will be added into this list
function(add_test_executable BINARY OUT_VARIABLE ADD_TO_CTEST)
    add_executable(${BINARY} ${ARGN})

    if (${ADD_TO_CTEST})
        add_test(TARGET ${BINARY})
    endif()
    target_link_libraries(${BINARY} PRIVATE hts-static m)

    add_dependencies(${BINARY} hts-static)
    set_target_properties(${BINARY} PROPERTIES LINKER_LANGUAGE C)

    # Add to an output scoped variable keeping track of binaries we need.
    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${BINARY}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${BINARY}" PARENT_SCOPE)
    endif ()
endfunction()



# todo Many of these tests are command line programs that require arguments.
#   this means we'll have to use add_custom_target to execute these tests.
#   however, since I didn't write this library it'll take some effort to
#   figure out the correct arguments for these programs. Therefore, I postpone.
add_test_executable(test_hts_endian TEST_PROGRAMS           YES  hts_endian.c)
add_test_executable(test_hts_open_fuzzer TEST_PROGRAMS      NO  fuzz/hts_open_fuzzer.c)
add_test_executable(test-fieldarith TEST_PROGRAMS           NO  fieldarith.c )
add_test_executable(test-hfile TEST_PROGRAMS                NO  hfile.c)
add_test_executable(test-pileup TEST_PROGRAMS               NO  pileup.c)
add_test_executable(test-plugins-dlhts TEST_PROGRAMS        NO  plugins-dlhts.c)
add_test_executable(test-sam TEST_PROGRAMS                  NO  sam.c)
add_test_executable(test-bgzf TEST_PROGRAMS                 NO  test_bgzf.c)
add_test_executable(test-kfunc TEST_PROGRAMS                NO  test_kfunc.c)
add_test_executable(test-kstring TEST_PROGRAMS              NO  test_kstring.c)
add_test_executable(test-parse-reg TEST_PROGRAMS            NO  test-parse-reg.c)
add_test_executable(test-realn TEST_PROGRAMS                NO  test_realn.c)
add_test_executable(test-regidx TEST_PROGRAMS               NO  test-regidx.c)
add_test_executable(test-str2int TEST_PROGRAMS              NO  test_str2int.c)
add_test_executable(test-view TEST_PROGRAMS                 NO  test_view.c)
add_test_executable(test-index TEST_PROGRAMS                NO  test_index.c)
add_test_executable(test-vcf-api TEST_PROGRAMS              NO  test-vcf-api.c)
add_test_executable(test-vcf-sweep TEST_PROGRAMS            NO  test-vcf-sweep.c)
add_test_executable(test-bcf-sr TEST_PROGRAMS               NO  test-bcf-sr.c)
add_test_executable(test-bcf-translate TEST_PROGRAMS        NO  test-bcf-translate.c)


add_test_executable(thrash_threads1 THRASH_TEST_PROGRAMS    No  thrash_threads1.c)
add_test_executable(thrash_threads2 THRASH_TEST_PROGRAMS    No  thrash_threads2.c)
add_test_executable(thrash_threads3 THRASH_TEST_PROGRAMS    No  thrash_threads3.c)
add_test_executable(thrash_threads4 THRASH_TEST_PROGRAMS    No  thrash_threads4.c)
add_test_executable(thrash_threads5 THRASH_TEST_PROGRAMS    No  thrash_threads5.c)
add_test_executable(thrash_threads6 THRASH_TEST_PROGRAMS    No  thrash_threads6.c)




install(TARGETS ${TEST_PROGRAMS} ${THRASH_TEST_PROGRAMS} DESTINATION ${CMAKE_INSTALL_BINDIR})