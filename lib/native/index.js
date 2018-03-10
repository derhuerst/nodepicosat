'use strict'

const path = require('path')
const bindings = require('node-gyp-build')(path.join(__dirname, '..', '..'))

const solveUnsafe = (formula, assumptions) => {
  return bindings.node_picosat_sat(formula, assumptions)
}

module.exports = solveUnsafe
