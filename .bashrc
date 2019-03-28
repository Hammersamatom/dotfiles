alias ls='ls --color=auto'
alias ..='cd ..'

shopt -s histappend

#c="\[\033["
#e="${c}m\]"
c="\e["
e="${c}0m"


#export PS1=""$'\u2770'"\u@\h \w"$'\u2771'" "
export PS1="╔═❰\u@${c}38;5;208m\h${e} ${c}0;93m\w${e}❱\n╚⫸ "