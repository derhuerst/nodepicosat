#!/bin/bash
set -e
echo 'building asm.js for browsers'

FUNCTIONS='["_picosat_init","_picosat_add","_picosat_assume","_picosat_sat","_picosat_variables","_picosat_deref","_picosat_reset"]'

emcc lib/picosat.c \
	-O3 \
	--memory-init-file 0 \
	--post-js lib/browser/post.js \
	-o lib/browser/picosat.asm.js \
	-s EXPORTED_FUNCTIONS=$FUNCTIONS \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]'
