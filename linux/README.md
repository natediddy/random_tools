Random Linux Tools
=========

build_timer
---------------
Times the time it takes to complete an Android build. <br />
You must setup the Android build environment first:
        
    . build/envsetup.sh
    lunch my_product-eng

Use `build_timer --help` for details



cmbacon
---------------
Downloads or views latest CyanogenMod ROMs for any supported Android device. <br />
A self-installing package is available if you click the "Download" button near the top of the Github page. <br />
To use the self-installing package:

    su -c 'sh cm_bacon_downloader.pkg'
        --or--
    sudo sh cm_bacon_downloader.pkg

This will install the script to `/usr/local/bin/cmbacon`.
Usage is:
       
    cmbacon [stable|nightly|rc|show] [DEVICE1] [DEVICE2] ... [DEVICEn]

Use `cmbacon help` for more details.



porter
---------------
For Arch Linux only. <br />
Provides a way to use the Arch Build System (ABS) similarly to BSD Ports or Gentoo Portage. <br />
Use `porter --help` for more details. <br />



satori-mod.vim
---------------
A slightly modified theme for use with Vim. <br />
Original theme can be found at: http://www.vim.org/scripts/script.php?script_id=1614 <br />
Place the file in `~/.vim/colors`. <br />
Then edit your `~/.vimrc` to include the line:

    colorscheme satori-mod

The next time you open a file in Vim the theme will be present.



dotfiles/*
---------------
Some of my personal home directory configuration files.
