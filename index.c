#include <node_api.h>
#include <napi-macros.h>

#include "lib/picosat.h"

NAPI_METHOD(node_picosat_sat) {
  napi_value argv[2];
  size_t argc = 2;
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  PicoSAT *pico_ptr = picosat_init();

  int * formula;
  size_t formula_len;
  napi_get_arraybuffer_info(env, argv[0], (void **) &formula, &formula_len);
  size_t i;
  for(i = 0; i < (formula_len / sizeof(int32_t)); i++) {
    picosat_add(pico_ptr, formula[i]);
  }

  int * assumptions;
  size_t assumptions_len;
  napi_get_arraybuffer_info(env, argv[1], (void **) &assumptions, &assumptions_len);
  for(i = 0; i < (assumptions_len / sizeof(int32_t)); i++) {
    picosat_assume(pico_ptr, assumptions[i]);
  }

  int status_code = picosat_sat(pico_ptr, -1);
  napi_value result_array;

  if (status_code == PICOSAT_SATISFIABLE) {
    size_t nvars = picosat_variables(pico_ptr);
    napi_create_array_with_length(env, nvars + 1, &result_array);

    // get and set the variable solutions
    size_t i;
    for (i = 1; i <= nvars; i++) {
      int val = picosat_deref(pico_ptr, i) * i;
      napi_value int_val;
      napi_create_int32(env, val, &int_val);
      napi_set_element(env, result_array, i, int_val);
    }
  } else {
    // returns an array with just the status code as first element
    napi_create_array_with_length(env, 1, &result_array);
  }

  // set the status code as first element of the array
  napi_value js_status_code;
  napi_create_int32(env, status_code, &js_status_code);
  napi_set_element(env, result_array, 0, js_status_code);

  picosat_reset(pico_ptr);

  return result_array;
}

NAPI_INIT() {
  NAPI_EXPORT_FUNCTION(node_picosat_sat)
}
