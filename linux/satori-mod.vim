" Vim color file
"
" This theme was originally written by:
"     Rudá Moura <ruda.moura@gmail.com>
" Download the original theme at:
"     http://www.vim.org/scripts/script.php?script_id=1614
"
" I just made a couple little adjustments to
" Constant and Number. It's a simple and elegant
" theme that looks good on a black background.

highlight clear Normal
set background&

highlight clear

if exists("syntax_on")
  syntax reset
endif

let colors_name = "satori"

" Vim colors
highlight Normal     ctermfg=NONE    cterm=NONE
highlight Comment    ctermfg=Cyan    cterm=NONE
highlight Constant   ctermfg=Green   cterm=NONE
highlight Number     ctermfg=Cyan    cterm=NONE
highlight Identifier ctermfg=NONE    cterm=NONE
highlight Statement  ctermfg=NONE    cterm=Bold
highlight PreProc    ctermfg=Blue    cterm=NONE
highlight Type       ctermfg=Magenta cterm=NONE
highlight Special    ctermfg=Magenta cterm=NONE
highlight Search     ctermbg=Green   cterm=NONE
highlight MatchParen ctermbg=NONE    cterm=Inverse
highlight Error      ctermbg=Red     cterm=NONE
highlight Todo	     ctermbg=Cyan    cterm=NONE
