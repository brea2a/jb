#compdef jo

# Completion function for zsh
# Store this file in a directory listed in $fpath for it to be picked up
# by compinit. It needs to be named with an initial underscore, e.g. _jo

local curcontext="$curcontext"
local -i aopt nm=$compstate[nmatches]
local -a expl line state state_descr
local -A opt_args

_arguments -C -s -A "-*" \
  '(-h)-p[pretty-print JSON on output]' \
  '(-d -h)-a[create an array of words]' \
  '(-v -V -h)-B[disable interpretation of true/false/null strings]' \
  "(-v -V -h)-e[if stdin is empty don't wait for input - quit]" \
  '(- *)-v[show version information]' \
  '(-a -B -e -h -v *)-V[show version in JSON]' \
  '(-a -h -v -V)-d+[key will be object path separated by given delimiter]:key delimiter' \
  '(- *)-h[show usage information]' \
  '*::word:->words'

if [[ -n $state ]]; then
  aopt=$+opt_args[-a]
  _arguments \
    '*-s[coerce type guessing to string]: :->words' \
    '*-b[coerce type guessing to bool]: :->words' \
    '*-n[coerce type guessing to number]: :->words' \
    '*: :->words'

  if (( aopt )); then
    _message -e words 'array element'
  elif compset -P 1 '*:='; then
    _alternative 'files:file:_files' 'operators:stdin:(-)'
  elif compset -P 1 '*='; then
    if compset -P '[@%:]'; then
      _files
    else
      _describe -t operators "file prefix" '(
        @:substitute\ file\ as-is
        %:substitute\ file\ in\ base64-encoded\ form
        \\::substitute\ file\ as\ JSON
      )' -S ''
      _message -e values value
    fi
  elif compset -P 1 '?*@'; then
    _description booleans expl 'boolean'
    compadd -M 'm:{a-zA-Z}={A-Za-z} m:{10}={TF}' "$expl[@]" True False
  else
    if compset -P '[^-]*'; then
      _describe -t suffixes suffix '(
        @:boolean\ element
        \=:value
        \\:=:substitute\ JSON\ file
        \[\]:array\ element
      )' -S ''
    fi
    _message -e keys key
  fi
fi

[[ nm -ne compstate[nmatches] ]]
