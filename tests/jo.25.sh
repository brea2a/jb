# overwrite values of existing object keys

${JO:-jo} a=1 b=2 a=3
${JO:-jo} -D a=1 b=2 a=3

tmp=`${JO:-jo} 1=a 2=b 3=c`
${JO:-jo} -d . stage="$tmp" down=up stage.2=x stage\[3\]=y stage.4=d stage\[1\]=h
${JO:-jo} -D -d . stage="$tmp" down=up stage.2=x stage\[3\]=y stage.4=d stage\[1\]=h
