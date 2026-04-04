#!/bin/bash
#
# Copyright (C) goblinhack@gmail.com
#
# This game is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This game is distributed in the hope that it will be fun,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this game; if not, write to the Free
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

. ./build/common.sh

TARGET=sacrecant

#
# The default MINGW32 and MINGW64 environments build binaries using the older
#  MSVCRT library that should be present on all Windows systems.
#
# The MINGW UCRT environments build binaries using the newer UCRT library that
# is only known to be present on Windows 10. It should produce binaries more
# compatible with MSVC-compiled binaries, but not with the MSVCRT environment.
#
MSYS_PATH=mingw64 	
MINGW_PKG_TYPE=mingw-w64

MSYS_PATH=clang64
MINGW_PKG_TYPE=mingw-w64-clang

MSYS_PATH=ucrt64 	
MINGW_PKG_TYPE=mingw-w64-ucrt

# Prefer gnu over freebsd coreutils
PATH=/opt/local/libexec/gnubin:$PATH
export PATH

help_full()
{
    case $(uname) in
    Linux)
      if [ "$DISTRO" = "fedora" ]; then
        log_warn "You may need to install:"
        log_warn "  dnf install -y findutils"
        log_warn "  dnf install -y SDL2_mixer-devel SDL2_mixer"
        log_warn "  dnf install -y SDL2 SDL2-devel"
        log_warn "  dnf install -y git"
        log_warn "  dnf install -y make automake gcc-c++"
        log_warn "  dnf install -y vim"
        log_warn "  dnf install -y libmikmod-devel"
        log_warn "  dnf install -y libfishsound-devel"
      fi

      if [ "$DISTRO" = "ubuntu" ]; then
        log_warn "You may need to install:"
        log_warn "  sudo apt-get install -y"
        log_warn "  sudo apt-get install -y build-essential"
        log_warn "  sudo apt-get install -y clang"
        log_warn "  sudo apt-get install -y clang-format"
        log_warn "  sudo apt-get install -y g++"
        log_warn "  sudo apt-get install -y git"
        log_warn "  sudo apt-get install -y libegl1"
        log_warn "  sudo apt-get install -y libfishsound1-dev"
        log_warn "  sudo apt-get install -y libflac-dev"
        log_warn "  sudo apt-get install -y libfluidsynth-dev"
        log_warn "  sudo apt-get install -y libfreetype6-dev"
        log_warn "  sudo apt-get install -y libgl1"
        log_warn "  sudo apt-get install -y libglvnd0"
        log_warn "  sudo apt-get install -y libglx0"
        log_warn "  sudo apt-get install -y libmikmod-dev"
        log_warn "  sudo apt-get install -y liboggz2-dev"
        log_warn "  sudo apt-get install -y libsdl2-2.0-0"
        log_warn "  sudo apt-get install -y libsdl2-dev"
        log_warn "  sudo apt-get install -y libsdl2-mixer-2.0-0"
        log_warn "  sudo apt-get install -y libsdl2-mixer-dev"
        log_warn "  sudo apt-get install -y libsmpeg-dev"
        log_warn "  sudo apt-get install -y libx11-6"
        log_warn "  sudo apt-get install -y libxext6"
        log_warn "  sudo apt-get install -y liblz4-dev"
        log_warn "  sudo apt-get install -y ssh"
        log_warn "  sudo apt-get install -y vim"
        log_warn "  sudo apt-get install -y xutils-d"
      fi
      ;;
    *MING*|*MSYS*)
      log_warn "You may need to install:"
      log_warn "  pacman -S git make vim base-devel --needed"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-ag"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-binutils"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-bzip2"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-clang"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-clang-tools-extra"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-crt-git"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-expat"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-flac"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-fluidsynth"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gcc"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gcc-libs"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gdb"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gdbm"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gettext"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-giflib"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-glib2"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-gmp"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-headers-git"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-isl"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libffi"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libiconv"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libjpeg-turbo"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libmad"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libmodplug"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libogg"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libpng"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libsndfile"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libsystre"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libtiff"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libtre-git"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libunwind"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libvorbis"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libwebp"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-libwinpthread-git"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-lld"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-llvm"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-lz4"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-mpc"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-mpfr"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-ncurses"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-openssl"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-portaudio"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-python3"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-readline"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-SDL"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-SDL2"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-SDL2_mixer"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-smpeg2"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-speex"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-speexdsp"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-termcap"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-windows-default-manifest"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-winpthreads-git"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-xz"
      log_warn "  pacman -Sy ${MINGW_PKG_TYPE}-x86_64-zl"
      ;;
    *Darwin*)
      log_warn "For MAC ports install:"
      log_warn "  sudo port install libsdl2 libsdl2_mixer"
      log_warn "For homebrew install:"
      log_warn "  brew install make llvm automake sdl2 sdl2_mixer"
      ;;
    esac
}

# https://superuser.com/questions/1156036/how-to-tell-whether-it-is-fedora-or-debian-using-a-shell-script
known_compatible_distros=(
                        "wifislax"
                        "kali"
                        "parrot"
                        "backbox"
                        "blackarch"
                        "cyborg"
                        "ubuntu"
                        "debian"
                        "suse"
                        "centos"
                        "gentoo"
                        "fedora"
                        "red hat"
                        "redhat"
                        "arch"
                        "openmandriva"
                        "darwin"
                    )

# Determine OS platform
# https://askubuntu.com/questions/459402/how-to-know-if-the-running-platform-is-ubuntu-or-centos-with-help-of-a-bash-scri
function detect_distro_phase0() {
  UNAME=$(uname | tr "[:upper:]" "[:lower:]")
  # If Linux, try to determine specific distribution
  if [ "$UNAME" == "linux" ]; then
      # If available, use LSB to identify distribution
      if [ -f /etc/lsb-release -o -d /etc/lsb-release.d ]; then
          export DISTRO=$(lsb_release -i | cut -d: -f2 | sed s/'^\t'//)
      # Otherwise, use release info file
      else
          export DISTRO=$(ls -d /etc/[A-Za-z]*[_-][rv]e[lr]* | grep -v "lsb" | cut -d'/' -f3 | cut -d'-' -f1 | cut -d'_' -f1)
      fi
  fi

  # For everything else (or if above failed), just use generic identifier
  [ "$DISTRO" == "" ] && export DISTRO=$UNAME
  unset UNAME
}

#First phase of Linux distro detection based on uname output
function detect_distro_phase1() {

    for i in "${known_compatible_distros[@]}"; do
        uname -a | grep "${i}" -i > /dev/null
        if [ "$?" = "0" ]; then
            DISTRO="${i}"
            break
        fi
    done
}

#Second phase of Linux distro detection based on architecture and version file
function detect_distro_phase2() {
    osversionfile_dir=/etc

    if [ "${DISTRO}" = "Unknown Linux" ]; then
        if [ -f ${osversionfile_dir}"fedora-release" ]; then
            DISTRO="fedora"
        elif [ -f ${osversionfile_dir}"centos-release" ]; then
            DISTRO="centos"
        elif [ -f ${osversionfile_dir}"gentoo-release" ]; then
            DISTRO="gentoo"
        elif [ -f ${osversionfile_dir}"redhat-release" ]; then
            DISTRO="redhat"
        elif [ -f ${osversionfile_dir}"SuSE-release" ]; then
            DISTRO="suse"
        elif [ -f ${osversionfile_dir}"debian_version" ]; then
            DISTRO="debian"
        fi
    fi
}

detect_distro_phase0
detect_distro_phase1
detect_distro_phase2
log_info "Distro/os                  : $DISTRO"

MY_OS_NAME=$(uname)
case "$MY_OS_NAME" in
    *MING*|*MSYS*)
        for i in \
            $(which sdl2-config) \
            /${MSYS_PATH}/bin/sdl2-config
        do
            if [ -x "$i" ]; then
                SDL2_CONFIG=$i
                break
            fi
        done
    ;;
    *)
        for i in \
            $(which sdl2-config) \
            /opt/local/bin/sdl2-config \
            /usr/bin/sdl2-config
        do
            if [ -x "$i" ]; then
                SDL2_CONFIG=$i
                break
            fi
        done
    ;;
esac

sdl_help()
{
    log_err "I need SDL2 and SDL2 mixer installed"
    help_full
    exit 1
}

gcc_help()
{
    log_err "No g++ or clang compiler found"
    help_full
    exit 1
}

SDL2_SCORE=0

if [ "$SDL2_CONFIG" != "" ]; then
    SDL2_INC_PATH=$($SDL2_CONFIG --cflags | sed -e 's/.*-I\([a-zA-Z\/@_0-9:\.]*\) .*/\1/g' -e 's/\-.*//g')

    if [[ "$SDL2_INC_PATH" != "" ]]; then
        find "$SDL2_INC_PATH" | grep -q SDL_mixer.h
        if [ $? -eq 0 ]; then
            SDL2_SCORE=$((SDL2_SCORE + 1))
            SDL2_MIXER=$(find "$SDL2_INC_PATH" -name SDL_mixer.h)
        fi
    fi
fi

log_info "SDL2 config                : $SDL2_CONFIG"
log_info "SDL2 version               : "$($SDL2_CONFIG --version 2>/dev/null)
log_info "SDL2 libs                  : "$($SDL2_CONFIG --libs 2>/dev/null)
log_info "SDL2 static libs           : "$($SDL2_CONFIG --static-libs 2>/dev/null)
log_info "SDL2 CFLAGS                : "$($SDL2_CONFIG --cflags 2>/dev/null)
log_info "SDL2 prefix                : "$($SDL2_CONFIG --prefix 2>/dev/null)
log_info "SDL2 exec-prefix           : "$($SDL2_CONFIG --exec-prefix 2>/dev/null)
log_info "SDL2 include path          : $SDL2_INC_PATH"
log_info "SDL2 mixer.h               : $SDL2_MIXER"
#log_info "SDL2 found                 : $SDL2_SCORE"

if [[ $SDL2_SCORE != "1" ]]; then
    sdl_help
    exit 1
fi

#
# Gives warnings at runtime on MACOS
#
SDL_LIBS=$($SDL2_CONFIG --libs)
if [ $? -ne 0 ]
then
    sdl_help
    exit 1
fi

CFLAGS+=$($SDL2_CONFIG --cflags | sed 's/ \-D_REENTRANT//g')
if [ $? -ne 0 ]
then
    sdl_help
    exit 1
fi

#
# -funwind-tables and -rdynamic for backtrace info on linux.
# But it seemed to help little.
#
LDLIBS="$SDL_LIBS"
LDLIBS+=" -lSDL2_mixer"

#
# Common config file
#
CONFIG_H=src/cfg.hpp
echo "#include \"my_cfg.hpp\"" > $CONFIG_H
CFLAGS+=" -include cfg.hpp"
rm -f src/precompiled.hpp.gch

EXTRA_CHECKS=" -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-common"

echo "#define MYVER \"$MYVER\"" >> $CONFIG_H

case "$MY_OS_NAME" in
    *MSYS*)
        log_err "Please compile for ming64, not msys"
        exit 1
        ;;
    *MING*|*MSYS*)
        PATH=/${MSYS_PATH}/bin:$PATH
        EXE=".exe"
        CFLAGS+=" -I/${MSYS_PATH}/ginclude "
        LDLIBS+=" -L/${MSYS_PATH}/glib/"
        LDLIBS=$(echo $LDLIBS | sed -e 's/-lmingw32 //g')
        LDLIBS+=" -funwind-tables"
        LDLIBS+=" -lstdc++"
        LDLIBS+=" -lstdc++exp"
        LDLIBS+=" -lopengl32"
        LDLIBS+=" -lpthread"
        LDLIBS+=" /${MSYS_PATH}/lib/libSDL2_mixer.a"
        LDLIBS+=" -L/${MSYS_PATH}/lib/binutils -lbfd -lintl -ldbghelp -liberty"

        if [ -f /${MSYS_PATH}/lib/libunwind.a ]; then
          LDLIBS+=" -lunwind"
          log_info "Have libunwind             : Yes"
        else
          log_info "Have libunwind             : No"
        fi

        #
        # Clang supports PDB debug file creation, but you need to do the following to enable it.
        #
        # -fno-emulated-tls is needed to work around a thread_local issue with lld
        #
        CFLAGS+=" -g"
        if [[ $OPT_GCC = "" ]]; then
          #
          # GCC says cannot be used with precompiled headers
          #
          CFLAGS+=" -gcodeview"
          #
          # GCC does not like
          #
          CFLAGS+=" -fno-emulated-tls"
        fi

        #
        # The space after pdb= is intentional to use the executable name for PDB file generation.
        #
        LDFLAGS+=" -g -Wl,--pdb= "
        if [[ $OPT_GCC = "" ]]; then
          #
          # GCC does not like
          #
          LDFLAGS+=" -fno-emulated-tls"
          #
          # Clang linker
          #
          LDFLAGS+=" -fuse-ld=lld "
        fi

        ;;
    *Darwin*)
        EXE=""
        LDLIBS+=" -funwind-tables"
        LDLIBS+=" -Wl,-framework,Opengl"
        #
        # Run in the background as it is slow and is only needed for debugging
        #
        DSYM="dsymutil \${TARGET_GAME} &"

        if [[ $OPT_SANITY_BUILD != "" ]]; then
            CFLAGS+="$EXTRA_CHECKS"
            LDFLAGS+="$EXTRA_CHECKS"
        fi
        LDFLAGS+=" -rdynamic"

        if [[ -f /opt/local/libexec/llvm-devel/lib/libunwind/libunwind.a ]]; then
          #
          # Can't seem to link it and get it to work!
          #
          # LDLIBS+=" -L/opt/local/libexec/llvm-devel/lib/libunwind -lunwind"
          log_info "Have libunwind             : Yes"
        else
          log_info "Have libunwind             : No"
        fi
        ;;
    *inux*)
        EXE=""
        LDLIBS+=" -lGL"

        if [[ $OPT_SANITY_BUILD != "" ]]; then
            CFLAGS+="$EXTRA_CHECKS"
            LDFLAGS+="$EXTRA_CHECKS"
        fi
        LDFLAGS+=" -rdynamic"

        pkg-config --print-provides libunwind >/dev/null 2>/dev/null
        if [[ $? -eq 0 ]]; then
          LDLIBS+=" -lunwind"
          log_info "Have libunwind             : Yes"
        else
          log_info "Have libunwind             : No"
        fi
        ;;
    *)
        EXE=""
        ;;
esac

#
# Better to leave off for production
#
# Windows builds also fail due to missing headers
#
WERROR=""
if [[ $OPT_DEBUG_BUILD != "" ]]; then
    WERROR="-Werror"
fi

cd src || exit

if [[ $OPT_PROFILE_BUILD != "" ]]; then
    CFLAGS+=" -pg"
    LDFLAGS+=" -pg"
fi

if [[ $OPT_SANITY_BUILD != "" ]]; then
    CFLAGS+=" -DSANITY_BUILD"
fi

if [[ $OPT_DEBUG_BUILD != "" || $OPT_SANITY_BUILD != "" ]]; then
    CFLAGS+=" -DDEBUG_BUILD"
else
    CFLAGS+=" -DRELEASE_BUILD"
fi

if [[ $OPT_GITHUB_BUILD != "" ]]; then
    CFLAGS+=" -DGITHUB_BUILD"
fi

OPT_LZ4=
if [[ -f /usr/include/lz4.h ]]; then
    OPT_LZ4=1
elif [[ -f /opt/local/include/lz4.h ]]; then
    CFLAGS+=" -I/opt/local/include"
    OPT_LZ4=1
elif [[ -f /${MSYS_PATH}/include/lz4.h ]]; then
    CFLAGS+=" -I/${MSYS_PATH}/include"
    OPT_LZ4=1
elif [[ -f $SDL2_INC_PATH/../lz4.h ]]; then
    #
    # last resort
    #
    CFLAGS+=" -I$SDL2_INC_PATH/.."
    OPT_LZ4=1
fi

if [[ $OPT_LZ4 != "" ]]; then
    #
    # LZ0 to LZ4 comparisons:
    #
    # LZ0 compressed 1239Mb (1299692095 bytes) -> 17Mb (18108144 bytes) took 122 ms
    # LZ0 decompress 17Mb (18108144 bytes) -> 1239Mb (1299692095 bytes) took 1311 ms (appdata/sacrecant/saved-slot-0)
    #
    # LZ4 compressed 1239Mb (1299692095 bytes) -> 15Mb (15982242 bytes) took 121 ms
    # LZ4 decompress 15Mb (15982242 bytes) -> 1239Mb (1299692095 bytes) took 63 ms (appdata/sacrecant/saved-slot-0)
    #
    CFLAGS+=" -DUSE_LZ4"
    LDLIBS+=" -llz4"
    log_info "Have LZ4                   : Yes"
else
    log_info "Have LZ4                   : No"
fi

#
# LLD is faster
#
# Needed to create PDB files
#
LLVM_PATH=$(clang++ -v 2>&1 | grep InstalledDir | sed 's/^.* //g' | sed 's/\(^.*\)\/.*/\1/g')

if [[ $(which xxxx_mold) ]]; then # binary seems to hang when using mold
  LDFLAGS+=" -fuse-ld=mold"
  log_info "Have mold                  : Yes"
elif [ -x $LLVM_PATH/bin/lld ]; then
  LDFLAGS+=" -fuse-ld=lld"
  log_info "Have lld                   : Yes"
else
  log_info "Have mold                  : No"
  log_info "Have lld                   : No"
fi

log_info "Have llvm                  : $LLVM_PATH/bin"

#
# Doesn't seem to work yet - at least on macos
#
#LDFLAGS+=" -flto"
#CFLAGS+=" -flto"

#
# Hard code on for me
#
$(grep -q goblinhack ~/.gitconfig 2>/dev/null)
GOBLINHACK=$?
if [[ $GOBLINHACK ]]; then
  WERROR="-Werror"
fi

MAKEFILE=../build/Makefile.template

if [[ $OPT_RELEASE_BUILD != "" ]]; then
    echo "COMPILER_FLAGS=$WERROR $CFLAGS -O3 -ffast-math -g" > $MAKEFILE
else
    echo "COMPILER_FLAGS=$WERROR $CFLAGS -Og -g" > $MAKEFILE
fi

if [[ $OPT_SANITY_BUILD != "" ]]; then
    GCC_STACK_CHECK="-fstack-protector-all -D_FORTIFY_SOURCE=2"
    GCC_STACK_CHECK="-fstack-protector-all"
else
    GCC_STACK_CHECK=
fi

CHOSEN_COMPILER=

case "$MY_OS_NAME" in
    *MING*|*MSYS*)
        #
        # Defaults
        #
        if [[ $CHOSEN_COMPILER = "" ]]; then
          CC=/${MSYS_PATH}/bin/g++.exe
          if [[ -x $CC ]]; then
            echo "CC=/${MSYS_PATH}/bin/g++.exe" >> $MAKEFILE
            CHOSEN_COMPILER="gcc"
          fi
        fi

        if [[ $CHOSEN_COMPILER = "" ]]; then
          CC=/${MSYS_PATH}/bin/clang++.exe
          if [[ -x $CC ]]; then
            echo "CC=/${MSYS_PATH}/bin/clang++.exe" >> $MAKEFILE
            CHOSEN_COMPILER="clang"
          fi
        fi

        #
        # Choice
        #
        if [[ $OPT_GCC = "" ]]; then
          CC=/${MSYS_PATH}/bin/clang++.exe
          if [[ -x $CC ]]; then
            echo "CC=/${MSYS_PATH}/bin/clang++.exe" >> $MAKEFILE
            CHOSEN_COMPILER="clang"
          fi
        fi

        #
        # To resolve WinMain, add these at the end again
        #
        LDLIBS+=" -lmingw32 -mwindows /${MSYS_PATH}/lib/libSDL2main.a -L/${MSYS_PATH}/lib -lSDL2main -lSDL2"
    ;;
    *)
        #
        # Defaults
        #
        g++ --version > /dev/null 2>/dev/null
        if [ $? -eq 0 ]
        then
            echo "CC=g++" >> $MAKEFILE
            CC=g++
            CHOSEN_COMPILER="gcc"
        fi

        if [[ $CHOSEN_COMPILER = "" ]]; then
            clang++ --version > /dev/null 2>/dev/null
            if [ $? -eq 0 ]; then
                echo "CC=clang++" >> $MAKEFILE
                CC=clang++
                CHOSEN_COMPILER="clang"
            fi
        fi

        #
        # Choice
        #
        if [[ $OPT_GCC = "" ]]; then
            clang++ --version > /dev/null 2>/dev/null
            if [ $? -eq 0 ]; then
                echo "CC=clang++" >> $MAKEFILE
                CC=clang++
                CHOSEN_COMPILER="clang"
            fi
        fi
    ;;
esac

if [[ $CHOSEN_COMPILER = "" ]]; then
    log_err "No compiler found"
    exit 1
fi

#
# How many cores?
#
CORES=""

case "$MY_OS_NAME" in
    *Darwin*)
      CORES=$(/usr/sbin/system_profiler -detailLevel full SPHardwareDataType 2>/dev/null | grep Cores | sed 's/.*: //g' | awk '{print $1}')
    ;;
    *inux*)
      CORES=$(grep -E -c "cpu cores|processor" /proc/cpuinfo)
    ;;
    *MING*|*MSYS*)
      CORES=$(grep -E -c "cpu cores|processor" /proc/cpuinfo)
    ;;
esac

if [ "$CORES" != "" ]
then
    log_info "Compiling ($CORES cpus)"

    CORES="-j $CORES"
else
    log_info "Compiling"
fi

#
# Create the makefile
#
cat >>$MAKEFILE <<%%
CHOSEN_COMPILER=$CHOSEN_COMPILER
CORES=$CORES
CPP_STANDARD=-std=c++26
DSYM=$DSYM
EXE=$EXE
LDLIBS=$LDLIBS
MY_OS_NAME=$MY_OS_NAME
SDL2_INC_PATH=$SDL2_INC_PATH

COMMON_WARNING_FLAGS=-Wall -Wextra -Wpedantic
#
# Additional warnings for uninitialized variables; seem to be gcc only
#
COMMON_WARNING_FLAGS+=-Wuninitialized
#
# When compiling C, give string constants the type const char[length] so that copying the address of
# one into a non-const char * pointer produces a warning. These warnings help you find at compile time
# code that can try to write into a string constant, but only if you have been very careful about using
# const in declarations and prototypes.
#
COMMON_WARNING_FLAGS+=-Wwrite-strings
#
# Warn whenever a local variable or type declaration shadows another variable, parameter.
#
COMMON_WARNING_FLAGS+=-Wshadow
#
# Warn whenever a function parameter is unused aside from its declaration. This option is not enabled by
# -Wunused unless -Wextra is also specified.
#
COMMON_WARNING_FLAGS+=-Wno-unused-parameter
#
# Warn if anything is declared more than once in the same scope, even in cases where multiple declaration is valid and changes nothing.
#
COMMON_WARNING_FLAGS+=-Wredundant-decls
#
# Enable -Wformat plus additional format checks. Currently equivalent to -Wformat -Wformat-nonliteral -Wformat-security -Wformat-y2k.
#
COMMON_WARNING_FLAGS+=-Wformat=2
#
# If -Wformat is specified, also warn if the format string is not a string literal and so cannot be checked, unless the format
# function takes its format arguments as a va_list.
#
COMMON_WARNING_FLAGS+=-Wno-format-nonliteral
#
# A warning is generated if the precision of a value may change.
#
COMMON_WARNING_FLAGS+=-Wfloat-conversion

CLANG_WARNING_FLAGS=

#
# To silence #emded for clang
#
#\$(info \$\$MY_OS_NAME is [\${MY_OS_NAME}])
#ifeq (\$(MY_OS_NAME),Darwin)
CLANG_WARNING_FLAGS+=-Wno-c2x-extensions # needed on macos clang build
#endif

#
# Don't fail if a compiler option is unknown
#
CLANG_WARNING_FLAGS+=-Wno-unknown-warning-option
#
# Additional warnings
#
CLANG_WARNING_FLAGS+=-Wconditional-uninitialized 
CLANG_WARNING_FLAGS+=-Wmaybe-uninitialized
CLANG_WARNING_FLAGS+=-Wuninitialized=verbose
#
# To silence Xcode warning: "variable length arrays are a C99 feature [-Wvla-extension]"?
#
CLANG_WARNING_FLAGS+=-Wno-vla-extension

#
# Compiler flags for each compiler
#
GCC_COMPILER_WARNINGS=\${CPP_STANDARD} \${GCC_WARNING_FLAGS} \${COMMON_WARNING_FLAGS} $GCC_STACK_CHECK
CLANG_COMPILER_WARNINGS=\${CPP_STANDARD} \${CLANG_WARNING_FLAGS} \${COMMON_WARNING_FLAGS} $GCC_STACK_CHECK

LDFLAGS=$LDFLAGS
%%

if [[ $CHOSEN_COMPILER = "gcc" ]]; then
    echo "COMPILER_WARNINGS=\$(GCC_COMPILER_WARNINGS)" >> $MAKEFILE
else
    echo "COMPILER_WARNINGS=\$(CLANG_COMPILER_WARNINGS)" >> $MAKEFILE
fi

log_info "CFLAGS                     : $CFLAGS"
log_info "LDFLAGS                    :$LDFLAGS"
log_info "LDLIBS                     : $LDLIBS"

#
# Cleanup pre-build
#
log_info "Cleaning"
make clobber | sed 's/^/  /g'

log_info "OS version                 :"
cat /etc/*-release 2>/dev/null
lsb_release -a 2>/dev/null
cat /proc/version 2>/dev/null
uname -a

log_info "Game version               : $MYVER"

log_info "Compiler version           :"
$CC --version

#
# Useful for debugging
#
# cat $MAKEFILE

cd ..

echo USE_PRECOMPILED=yep make -f build/Makefile $CORES "$@" all
USE_PRECOMPILED=yep make -f build/Makefile $CORES "$@" all

if [ $? -eq 0 ]
then
    case "$MY_OS_NAME" in
        *MING*|*MSYS*)
            log_info "Run:"

            cat >${TARGET}-create-release.sh <<%%
#!/bin/sh
###############################################################################
# Execute the following to build the release
###############################################################################
#
# Clean up the old installs
#
rm ${TARGET}.*installer.exe

#
# Update the version
#
echo \$0: updating the version to $MYVER
sed -i "s/<version>.*/<version>$MYVER<\/version>/g" build/windows/windows.xml
(cd build/windows/ ; ./windows.sh)


#
# All commits since last tag
#
echo \$0: all commits since last tag
git log \$(git describe --tags --abbrev=0)..HEAD --oneline | grep -E "feat:|bug:" | sort +1 -r

#
# Lay a tag
#
echo \$0: laying a tag
git tag -m "tag: $MYVER" -a v$MYVER
git push origin --tags

echo \$0: all done
%%
            chmod +x ${TARGET}-create-release.sh

            cat >${TARGET}.sh <<%%

#!/bin/sh
###############################################################################
# Execute the following, or run ./${TARGET}.sh
###############################################################################
./${TARGET}.exe \$*
%%

            cat ${TARGET}.sh
            chmod +x ${TARGET}.sh
            ;;
        Linux)
            log_info "Run:"
            echo "  ./${TARGET}"
            ;;
        *)
            log_info "Run:"
            echo "  ./${TARGET}"
            ;;
    esac

    rm -f Makefile.bak
else
    log_die "Build failed"
    exit 1
fi

#
# Execute unit tests?
#
if [[ $OPT_GITHUB_BUILD != "" ]]; then
    log_info "Running tests:"

    if [[ -f .o/${TARGET}.pdb ]]; then
      cp .o/${TARGET}.pdb .
    fi

    LSAN_OPTIONS=max_leaks=10 ./${TARGET} --tests --debug
    if [[ $? -ne 0 ]]; then
        ls -la
        cat appdata/${TARGET}/stdout.txt
        exit 1
    fi
fi

exit 0
