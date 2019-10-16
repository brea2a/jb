# bash completion for jo(1)

_jo() {

    # Don't split words on =, for =@ and =% handling
    COMP_WORDBREAKS=${COMP_WORDBREAKS//=}

    # No completion if an exit causing flag is around
    local i
    for i in ${!COMP_WORDS[@]}; do
        [[ $i -ne $COMP_CWORD && ${COMP_WORDS[i]} == -*[hvV]* ]] && return 0
    done

    # Complete available options following a dash
    if [[ $2 == -* ]]; then
        COMPREPLY=( $(compgen -W '-a -B -h -p -v -V' -- "$2") )
        return 0
    fi

    # Complete filenames on =@ and =%
    if [[ $2 == *=[@%]* ]]; then
        local file prefix
        file="${2#*=[@%]}"
        prefix="${2:0:${#2}-${#file}}"
        compopt -o filenames
        COMPREPLY=( $(compgen -f -- "$file") )
        if [[ ${#COMPREPLY[@]} -eq 1 ]]; then
            if [[ -d "${COMPREPLY[0]}" ]]; then
                COMPREPLY[0]+=/
                compopt -o nospace
            fi
            COMPREPLY[0]="$prefix${COMPREPLY[0]}"
        fi
        return 0
    fi

} &&
complete -F _jo jo
