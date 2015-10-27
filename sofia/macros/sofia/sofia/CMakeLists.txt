GENERATE_ROOT_TEST_SCRIPT(${ENSARROOT_SOURCE_DIR}/sofia/macros/sofia/sofia/sofiasim.C)
add_test(sofiasim ${ENSARROOT_BINARY_DIR}/sofia/macros/sofia/sofia/sofiasim.sh)
SET_TESTS_PROPERTIES(sofiasim PROPERTIES TIMEOUT "100")
SET_TESTS_PROPERTIES(sofiasim PROPERTIES PASS_REGULAR_EXPRESSION "TestPassed;All ok")
