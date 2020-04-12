alias ls='ls --color=auto'
alias ll='ls -la'
alias ..='cd ..'
alias gitinfo="git log --color=always --pretty=format:'%C(always,red)%H %C(always,green)%an %C(always,blue)%cs %Creset%s'"
shopt -s histappend

# \u is username
# \h is hostname
# \w is the CWD

FGD="\e[39m"
FG1="\e[30m"
FG2="\e[31m"
FG3="\e[32m"
FG4="\e[33m"
FG5="\e[34m"
FG6="\e[35m"
FG7="\e[36m"
FG8="\e[37m"
FG9="\e[90m"
FG10="\e[91m"
FG11="\e[92m"
FG12="\e[93m"
FG13="\e[94m"
FG14="\e[95m"
FG15="\e[96m"
FG16="\e[97m"

BGD="\e[49m"
BG1="\e[40m"
BG2="\e[41m"
BG3="\e[42m"
BG4="\e[43m"
BG5="\e[44m"
BG6="\e[45m"
BG7="\e[46m"
BG8="\e[47m"
BG9="\e[100m"
BG10="\e[101m"
BG11="\e[102m"
BG12="\e[103m"
BG13="\e[104m"
BG14="\e[105m"
BG15="\e[106m"
BG16="\e[107m"

c="\[\e["
e="\[\e[0m\]"

export PS1="╔═❰ \u@${c}38;5;208m\]\h${e} ${c}0;93m\]\w${e} ❱\n╚${c}0;36m\]⫸${e}  "




VISUAL=nvim
EDITOR=nvim
export VISUAL
export EDITOR
