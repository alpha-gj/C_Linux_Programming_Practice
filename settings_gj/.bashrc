# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'
alias lm='ls -al'
# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

SVNROOT=https://svnsvr/svn/rudy


#Linux hot key
alias update='sudo apt-get update'
alias cls='clear'
alias his='history'
alias m='make'
alias md='make > /dev/null'
alias mi='make install'
alias mid='make install > /dev/null'
alias mc='make clean'
alias ma='make all'
alias mad='make all > /dev/null'
alias mmi='make; make install'
alias cdd='cd /home/gj/Desktop'
alias cdl='cd /home/gj/Downloads'

#linux_c
alias gc='cd /home/gj/git/linux_C_practice/c'
alias gl='cd /home/gj/git/linux_C_practice/tlpi'

#CS-6022
alias cdr='cd /home/gj/git/ralink'
alias cdrm='cd /home/gj/git/ralink/mkimages'
alias cdw3='cd /home/gj/git/ralink/apps/watchdog3'
alias cdw4='cd /home/gj/git/ralink/apps/watchdog4'
alias cdm='cd /home/gj/git/ralink/apps/mfgbox'
alias cdp='cd /home/gj/git/ralink/prefix'
alias cdn='cd /home/gj/git/ralink/apps/network2'
alias cdt='cd /tftpboot'
alias cdmf='cd /home/gj/git/ralink/mkimages/factory'
alias so='cdr; source ./confs/cs6022_ov4689.conf; cd -'
alias soa='cdr; source ./confs/cs6022_ov4689_sta.conf; cd -'
alias cdrw='cd /home/gj/git/ralink/driver/modules/mt7603e_4.0.0.8_rev2'

#CS-3262
alias cdv='cd /home/gj/git/vatics'
alias cdvp='cd /home/gj/git/vatics/prefix'
alias cdvm='cd /home/gj/git/vatics/mkimages'
alias soc='cdv; source ./confs/cs3262.conf; cd -'
alias sod='cdv; source ./confs/dcs962.conf; cd -; echo this is NOR version'
alias son='cdv; source ./confs/dcs962n.conf; cd -; echo this is NAND version'
alias soh='cdv; source ./confs/dcs8100lh.conf; cd -; echo this is NAND version'
alias cdac='cd /home/gj/git/vatics/apps/libs/ahal/CS-3262'
alias cdah='cd /home/gj/git/vatics/apps/libs/ahal/h'
alias cdlw='cd /home/gj/git/vatics/apps/libs/wlan'
alias cdvw='cd /home/gj/git/vatics/driver/wifi'
alias cdvn='cd /home/gj/git/vatics/apps/network2'
alias cdmfg='cd /home/gj/git/vatics/apps/mfgbox'
alias cddog='cd /home/gj/git/vatics/apps/watchdog'
alias cpvk='cdv; cp mkimages/factory/kernel.bin mkimages/../prefix/'

#export PATH=/home/gj/git/vatics/tools:/opt/Vatics/vtcs_toolchain/arm-eabi-uclibc/usr/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games




