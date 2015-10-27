GENERATE_ROOT_TEST_SCRIPT(${ENSARROOT_SOURCE_DIR}/sofia/macros/sofia/r3bsim.C)
add_test(r3bsim ${ENSARROOT_BINARY_DIR}/sofia/macros/sofia/r3bsim.sh)
SET_TESTS_PROPERTIES(r3bsim PROPERTIES TIMEOUT "100")
SET_TESTS_PROPERTIES(r3bsim PROPERTIES PASS_REGULAR_EXPRESSION "TestPassed;All ok")

add_subdirectory(sofia)
