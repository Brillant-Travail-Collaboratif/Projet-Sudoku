#include "test_suites.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int main(void) {
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  if (register_grid_tests() != 0 || register_tile_tests() != 0 ||
      register_generator_tests() != 0 || register_solver_tests() != 0 ||
      register_io_tests() != 0 || register_tables_tests() != 0) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  unsigned int failures = CU_get_number_of_failures();
  CU_cleanup_registry();

  return failures == 0 ? 0 : 1;
}