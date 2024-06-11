alias ls='ls --color=auto'
alias ll='ls -lh --color --group-directories-first'
alias ..='cd ..'
alias gitinfo="git log --color=always --pretty=format:'%C(always,red)%H %C(always,green)%an %C(always,blue)%cs %Creset%s'"
alias cdls='cd "$@" && ls'

VISUAL=nvim
EDITOR=nvim
export VISUAL
export EDITOR

HISTSIZE=-1
HISTFILESIZE=-1
shopt -s histappend


PATH="$PATH:/media/TrueVol/Projects/Other/dotfiles"
PS1="\$(ps1lol \$?)\n "
