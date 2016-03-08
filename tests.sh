#!/bin/sh

[ "$(./jo -a jo)" = '["jo"]' ] ||
	{ echo "Test 1: Jo logo failed" >&2; exit 1; }
[ "$(./jo n=7 a=Hello)" = '{"n":7,"a":"Hello"}' ] ||
	{ echo "Test 2: Object failed" >&2; exit 2; }
