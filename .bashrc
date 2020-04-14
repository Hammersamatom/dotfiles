alias ls='ls --color=auto'
alias ll='ls -la'
alias ..='cd ..'
alias gitinfo="git log --color=always --pretty=format:'%C(always,red)%H %C(always,green)%an %C(always,blue)%cs %Creset%s'"
shopt -s histappend
alias gliph=arrowGlyph
alias gitty=gitBranch
alias pith=pathShort
# \u is username
# \h is hostname
# \w is the CWD

FD="\001\e[39m\002"
F=( "\001\e[30m\002"
    "\001\e[31m\002"
    "\001\e[32m\002"
    "\001\e[33m\002"
    "\001\e[34m\002"
    "\001\e[35m\002"
    "\001\e[36m\002"
    "\001\e[37m\002"
    "\001\e[90m\002"
    "\001\e[91m\002"
    "\001\e[92m\002"
    "\001\e[93m\002"
    "\001\e[94m\002"
    "\001\e[95m\002"
    "\001\e[96m\002"
    "\001\e[97m\002" )

BD="\001\e[49m\002"
B=( "\001\e[40m\002"
    "\001\e[41m\002"
    "\001\e[42m\002"
    "\001\e[43m\002"
    "\001\e[44m\002"
    "\001\e[45m\002"
    "\001\e[46m\002"
    "\001\e[47m\002"
    "\001\e[100m\002"
    "\001\e[101m\002"
    "\001\e[102m\002"
    "\001\e[103m\002"
    "\001\e[104m\002"
    "\001\e[105m\002"
    "\001\e[106m\002"
    "\001\e[107m\002" )








function pathShort {
    local dirs=$(dirs)
    local length=${#dirs}
    if [[ length -lt 10 ]]
    then
        echo -e "$dirs"
    else
        local outvar=""
        
        if [ ${dirs:0:1} = "/" ]
        then
            outvar="/"
        fi
        
        IFS=$'\n' folders=( $(echo $dirs | sed 's/\//\n/g') )

        for ((i = 0; i < ${#folders[@]}; i++))
        do
            if [ $i != $(echo ${#folders[@]}-1 | bc) ]
            then
                outvar="${outvar}${folders[i]:0:1}/"
            else
                outvar="${outvar}${folders[i]}"
            fi
        done

        echo "$outvar"
    fi
}

function gitBranch {
    local branch=$(git branch 2>/dev/null | grep \* | sed 's/\* //g')
    if [[ $branch != "" ]]
    then
        echo " ${branch}"
    fi
}

#LINE1="${B[8]} \u@\h ${F[8]}   ${B[5]}${FD} \$(pathShort) ${F[5]}   ${B[4]}${FD} \$(gitBranch) ${F[4]}${BD}${FD}"

# Version two of the function has an odd way to input items
#   $1 Singular input that is formatted like this
#       color + seperator + text
#       3     + \`        + hello


# The output MUST be echoed using the -e switch in order or colors to activate and escape corctly
arrowGlyph () {
    local sections=("8" "$USER@$HOSTNAME" "4" "$(pathShort)" "5" "$(gitBranch)")

    local tempArray=()
    for (( j = 0; j < $(( ${#sections[@]} / 2 )); j++ ))
    do
        if [[ ${sections[ $(( 1 + j * 2 )) ]} != "" ]]
        then
            tempArray+=( ${sections[ $(( j * 2     )) ]} )
            tempArray+=( ${sections[ $(( j * 2 + 1 )) ]} )
        fi
    done

    local output=""
    
    for (( i = 0; i < $(( ${#tempArray[@]} / 2 )); i++ ))
    do
        local num=$(( $i*2 ))
        local selcolor=${sections[ $num ]}
        local nextcolor=${sections[ $(( $num + 2 )) ]}


        output="${output}${B[ ${selcolor} ]} ${sections[ $(( ${num} + 1 )) ]} ${F[ ${selcolor} ]}"
       
        # Check for the end of the array
        if [[ $i == $(( ${#tempArray[@]} / 2 - 1 )) ]]
        then
            output="${output}${BD}${FD}"
        else
            output="${output}${B[ ${nextcolor} ]}${FD}"
        fi
    done

    echo -e "$output"
}

#        a.k.a Unicode E0B0

#LINE1="${FD}${B[8]} \u@\h ${F[8]}${B[5]}${FD} \$(pathShort) ${F[5]}${B[4]}${FD} \$(gitBranch) ${F[4]}${BD}${FD}"
#LINE2="${B[8]} ${F[8]}${BD}${FD}"
#LINE1="${B[8]} \u@\h ${F[8]}   ${B[5]}${FD} \$(pathShort) ${F[5]}   ${B[4]}${FD} \$(gitBranch) ${F[4]}${BD}${FD}"

LINE2="${B[8]} ${F[8]}${BD}${FD}"
PS1="\$(arrowGlyph)\n$LINE2"


VISUAL=nvim
EDITOR=nvim
export VISUAL
export EDITOR
