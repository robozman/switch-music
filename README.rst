Switch Music
============

A very WIP music player for the Nintendo Switch. There are literally memory leaks in this program.

Built using libnx, SDL2, and SDL2_mixer.

Dependencies
------------
Libraries

* Ordered in proper order for LIBS (I should really just use pkg-config)
* devkitPro pacman package in parenthesis

- libSDL2_ttf       (switch-sdl2_tff)            
- libSDL2_image     (switch-sdl2_image)        
- libSDL2_mixer     (switch-sdl2_mixer)
- libSDL2           (switch-sdl2)    
- libfreetype       (switch-freetype)        
- libbz2            (switch-bzip2)    
- libpng            (switch-libpng)    
- libjpeg           (switch-libjpeg)    
- libz              (switch-zlib)
- libvorbisidec     (switch-libvorbisidec)                
- libogg            (swtich-libogg)
- libmpg123         (swtich-mpg123)
- libmodplug        (swtich-libmodplug)
- libnx             (libnx)
- libm              (bundled inside one of the previous?)

Tools


* bin2o (general-tools)


Install Command

.. code-block:: bash

    sudo pacman -S switch-sdl2_ttf switch-sdl2_image switch-sdl2_mixer \
    switch-sdl2 switch-freetype switch-bzip2 switch-libpng switch-libjpeg \
    switch-zlib switch-libvorbisidec switch-libogg switch-mpg123 switch-libmodplug \
    libnx


Building
--------

Run ``make`` in root directory

Supports -j, but values more then 4 cause errors with the TTF file (at least for me)