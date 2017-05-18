# type coercion

# coerce key=val
for v in "" string \"quoted\" 12345 true false null; do
  ${JO:-jo} -- -s s="$v" -n n="$v" -b b="$v" a="$v"
done

# coerce array items
${JO:-jo} -a -- -s 123 -n "This is a test" -b C_Rocks 456

### These should NOT be coerced

# @ booleans
for v in 0 1; do
  ${JO:-jo} -- -s s@"$v" -n n@"$v" -b b@"$v" a@"$v"
done

# @/% file inclusions
${JO:-jo} -- -s s=@${srcdir:=.}/AUTHORS -n n=@${srcdir:=.}/AUTHORS -b b=@${srcdir:=.}/AUTHORS a=@${srcdir:=.}/AUTHORS
${JO:-jo} -- -s s=%${srcdir:=.}/AUTHORS -n n=%${srcdir:=.}/AUTHORS -b b=%${srcdir:=.}/AUTHORS a=%${srcdir:=.}/AUTHORS
