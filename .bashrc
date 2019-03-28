alias ls='ls --color=auto'
alias ..='cd ..'

shopt -s histappend



c="\e["
e="${c}0m"

export PS1="╔═❰\u@${c}38;5;208m\h${e} ${c}0;93m\w${e}❱\n╚⫸ "