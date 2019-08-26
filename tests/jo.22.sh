# read nested json elements

echo 'invalid json' > $$.1

${JO:-jo} huh=:$$.1

rm -f $$.1
