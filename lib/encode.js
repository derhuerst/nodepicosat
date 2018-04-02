'use strict'

const encodeInt32Array = (arr) => {
  const view = Int32Array.from(arr)
  return view.buffer
}

module.exports = encodeInt32Array
