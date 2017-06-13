"===alias===
ab m make
ab mi make install
ab mmi make; make install
ab mc make clean

set tags=/home/gj/git/vatics/apps/tags
set tags=/home/gj/git/vatics/sdk/tags
"set tags=/home/gj/git/vatics/driver/tags
set tags=/home/gj/git/ralink/apps/tags
"set autochdir
"=======分隔線========
"配色方案


" 去掉輸入錯誤的提示聲音

set noeb

" 在處理未保存或只讀文件的時候，彈出確認

set confirm

" 自動縮進

set autoindent

set cindent

" 自動換行

set wrap

" 整詞換行

set linebreak

" Tab鍵的寬度

set tabstop=4

" 統一縮進為4

set softtabstop=4

set shiftwidth=4

" 不要用空格代替製表符

set noexpandtab

" 在行和段開始處使用製表符

set smarttab

" 顯示行號

set number

" 歷史記錄數

set history=1000

"禁止生成臨時文件

set nobackup

set noswapfile

"搜索忽略大小寫

set ignorecase

"搜索逐字元高亮

set hlsearch

set incsearch

"行內替換

set gdefault

"編碼設置

set enc=utf-8

set fencs=utf-8,ucs-bom,shift-jis,gb18030,gbk,gb2312,cp936

"語言設置

set helplang=cn

set encoding=utf8

set langmenu=zh_CN.UTF-8

set imcmdline

nmap <F4> :q<cr>

nnoremap <silent> <C-l> :nohl<CR><C-L>
nnoremap <silent> <F8> :A<CR>


map <F8> :e %:p:s,.h$,.X123X,:s,.cpp$,.h,:s,.X123X$,.cpp,<CR>
au BufReadPost * if line("'\"") > 0|if line("'\"") <= line("$")|exe("norm '\"")|else|exe "norm $"|endif|endif""""'")"")""'")""'")

" wiki
set nu
set ai
set cursorline
set bg=dark
set tabstop=4
set shiftwidth=4

" option
""set mouse=a
set ruler
set incsearch

" good setting
filetype indent on
inoremap ( ()<Esc>i
inoremap {<CR> {<CR>}<Esc>ko
inoremap {{ {}<ESC>i
inoremap " ""<Esc>i
inoremap ' ''<Esc>i
inoremap [ []<Esc>i

set nu
""set cursorline
set tabstop=4
set shiftwidth=4

" Color configuration
set bg=light
color evening  " Same as :colorscheme evening
hi LineNr cterm=bold ctermfg=DarkGrey ctermbg=NONE
hi CursorLineNr cterm=bold ctermfg=LightYellow ctermbg=NONE

" tags
set tags=./tags,tags;
