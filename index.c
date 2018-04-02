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
  napi_value result;

  if (status_code == PICOSAT_SATISFIABLE) {
    size_t nvars = picosat_variables(pico_ptr);
    int * data[1 + nvars];
    data[0] = status_code;

    // copy variable solutions
    for (i = 1; i <= nvars; i++) {
      int val = picosat_deref(pico_ptr, i) * i;
      data[i] = val;
    }

    napi_create_arraybuffer(env, sizeof(data), &data, &result);
  } else {
    int * data[1] = { status_code };
    napi_create_arraybuffer(env, sizeof(data), &data, &result);
  }

  picosat_reset(pico_ptr);

  return result;
}

NAPI_INIT() {
  NAPI_EXPORT_FUNCTION(node_picosat_sat)
}
