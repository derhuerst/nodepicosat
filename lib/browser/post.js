Module.solveUnsafe = (function() {
  // todo: let, const
  var init = Module.cwrap('picosat_init', 'number', [])
  var add = Module.cwrap('picosat_add', null, ['number', 'number'])
  var assume = Module.cwrap('picosat_assume', null, ['number', 'number'])
  var sat = Module.cwrap('picosat_sat', 'number', ['number', 'number'])
  var variables = Module.cwrap('picosat_variables', 'number', ['number'])
  var deref = Module.cwrap('picosat_deref', 'number', ['number', 'number'])
  var reset = Module.cwrap('picosat_reset', 'null', ['number'])

  return function solveUnsafe(formula, assumptions) {
    var solver = init()

    for (var i = 0; i < formula.byteLength; i++) {
      add(solver, formula.readInt8(i))
    }
    for (var i = 0; i < assumptions.byteLength; i++) {
      assume(solver, assumptions.readInt8(i))
    }

    var status = sat(solver, -1)
    var res = [status]
    if (status === 10) { // satisfiable
      var nrOfVariables = variables(solver)
      for (var i = 1; i <= nrOfVariables; i++) {
        res[i] = deref(solver, i) * i
      }
    }

    reset(solver)
    return res
  }
})()