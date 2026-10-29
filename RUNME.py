#!/usr/bin/env python3
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
"""Configure and build sacrecant.

This replaces the old RUNME, build/build.sh and build/common.sh shell scripts, and the ramdisk
generation that used to be in build/ramdisk.py. It does the following:

  1. works out the platform, compiler and SDL (SDL2 or SDL3) to use,
  2. writes src/cfg.hpp and build/Makefile.template for src/Makefile to consume,
  3. cleans, extracts the game data and generates the ramdisk sources,
  4. runs make, and
  5. optionally (--github) runs the unit tests.

Run ./RUNME.py --help for the options. Requires Python 3.12 or later and only the standard library.
"""

from __future__ import annotations

import sys

if sys.version_info < (3, 12):  # noqa: UP036
    sys.exit(f"RUNME.py needs Python 3.12 or later; this is Python {sys.version_info.major}.{sys.version_info.minor}")

import argparse
import dataclasses
import enum
import logging
import os
import platform
import re
import shlex
import shutil
import subprocess
import tarfile
from collections import defaultdict
from collections.abc import Iterable, Mapping, Sequence
from pathlib import Path, PurePosixPath
from typing import Any

TARGET = "sacrecant"
MAINTAINER = "goblinhack@gmail.com"
ISSUES_URL = "https://github.com/goblinhack/sacrecant"
CPP_STANDARD = "-std=c++26"

ROOT = Path(__file__).resolve().parent
SRC_DIR = ROOT / "src"
BUILD_DIR = ROOT / "build"
DATA_DIR = ROOT / "data"
OBJ_DIR = ROOT / ".o"
LOG_FILE = ROOT / "build.log"
VERSION_FILE = BUILD_DIR / "VERSION"
CONFIG_HEADER = SRC_DIR / "cfg.hpp"
MAKEFILE_TEMPLATE = BUILD_DIR / "Makefile.template"

#
# Macports installs GNU coreutils here and the Makefile recipes prefer them over the BSD ones.
#
GNU_BIN_DIR = Path("/opt/local/libexec/gnubin")

#
# LLD is faster and is needed to create PDB files. We only look in Xcode; override with $LLVM_PATH.
# (mold was tried but the binary seemed to hang.)
#
DEFAULT_LLVM_PATH = Path("/Applications/Xcode.app/Contents/Developer")

#
# From this clang version on we can use #embed for the ramdisk, else we need assembly with .incbin.
#
CLANG_EMBED_MIN_VERSION = 21
RAMDISK_ASM_FILES = 10

BANNER = r"""
   ██████  ▄▄▄       ▄████▄   ██▀███  ▓█████  ▄████▄   █▄▄       ███▄    █ ▄▄██████▓
 ▒██    ▒ ▒████▄    ▒██▀ ▀█  ▓██ ▒ ██▒▓█   ▀ ▒██▀ ▀█  ▒████▄     ██ ▀█   █ ▓  ██▒ █▒
 ░ ▓██▄   ▒██  ▀█▄  ▒██    ▄ ▓██ ░▄█ ▒▒███   ▒██    ▄ ▒██  ▀█▄  ▓██  ▀█ ██▒▒ ▓██░ ▒░
   ▒   ██▒░██▄▄▄▄██ ▒▓█▄ ▄██▒▒██▀▀█▄  ▒██  ▄ ▒▓█▄ ▄██▒░██▄▄▄▄██ ▓██▒  ▐▌██▒░ ▓██▓ ░ 
 ▒██████▒▒ ▓█   ▓██▒▒ ▓███▀ ░░██▓ ▒██▒░▒████▒▒ ▓███▀ ░ ▓█   ▓██▒▒██░   ▓██░  ▒███ ░ 
 ▒ █▓▒█▒ ░ ▒▒   ▓▒█░░ ░█ ▒  ░░ ▒▓ ░▒█░░░ ▒░ ░░ ░▒█▒  ░ ▒▒   ▓▒█░░ █░   ▒█▒   ▒ ░█   
 ░ █▒  ░ ░  ▒   ▒▒█░  ░  ▒     ░▒ ░ ▒░ ░ ░  ░  ░ █▒     ▒   ▒▒ ░░ █░   ░ ▒░    ░█   
 ░  ░  ░    ░   ▒   ░          ░░   ░    ░   ░          ░   ▒      ░   ░ ░   ░      
       ░        ░  ░░ ░         ░        ░  ░░ ░            ░  ░         ░          
                    ░                        ░                                      
 
"""

#
# The warning flags section of the generated makefile; @@STACK@@ is replaced by the stack check flags.
#
MAKEFILE_WARNINGS = r"""
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
#$(info $$MY_OS_NAME is [${MY_OS_NAME}])
#ifeq ($(MY_OS_NAME),Darwin)
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
GCC_COMPILER_WARNINGS=${CPP_STANDARD} ${GCC_WARNING_FLAGS} ${COMMON_WARNING_FLAGS} @@STACK@@
CLANG_COMPILER_WARNINGS=${CPP_STANDARD} ${CLANG_WARNING_FLAGS} ${COMMON_WARNING_FLAGS} @@STACK@@

"""

RUN_HINTS = f"""\
  ./{TARGET} --debug          # enable a reasonable level of debugging
  ./{TARGET} --debug2         # include level generation debugging
  ./{TARGET} --seed weekly    # to play the weekly seed
  ./{TARGET} --seed daily     # to play the daily seed
  ./{TARGET} --seed someseed  # to play a specific seed
  ./{TARGET} --level <n>      # to start at level number <n>
  ./{TARGET} --quickstart     # to jump past the initial menus
  ./{TARGET} --tests          # to run unit tests
  ./{TARGET}                  # to play the game"""


# --------------------------------------------------------------------------------------------------
# Logging
# --------------------------------------------------------------------------------------------------

STEP = 25  # a log level for the progress messages
ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


class Ansi:
    """ANSI colours; all empty strings when we are not writing to a terminal."""

    def __init__(self, enabled: bool) -> None:
        def code(bright: int, colour: int) -> str:
            return f"\x1b[{bright};{colour}m" if enabled else ""

        self.reset = code(0, 37)
        self.red = code(0, 31)
        self.green = code(0, 32)
        self.magenta = code(0, 35)
        self.cyan = code(0, 36)


class ConsoleFormatter(logging.Formatter):
    """Formats messages in the style of the old shell scripts, with colours if wanted."""

    def __init__(self, ansi: Ansi) -> None:
        super().__init__()
        self.ansi = ansi

    def format(self, record: logging.LogRecord) -> str:
        a = self.ansi
        msg = record.getMessage()
        match record.levelno:
            case logging.CRITICAL:
                return f"{TARGET}: {a.red}FATAL ERROR: {msg}{a.reset}\nMail {MAINTAINER} for help"
            case logging.ERROR:
                return f"{TARGET}: {a.red}ERROR: {msg}{a.reset}"
            case logging.WARNING:
                return f"{TARGET}: {a.cyan}WARN: {msg}{a.reset}"
            case logging.INFO:
                return f"{TARGET}: {a.green}{msg}{a.reset}"
            case lvl if lvl == STEP:
                return f"{a.magenta}{msg}{a.reset}"
            case _:
                return msg


class FileFormatter(logging.Formatter):
    """The same as the console, minus the colours, for build.log."""

    def __init__(self) -> None:
        super().__init__()
        self.inner = ConsoleFormatter(Ansi(False))

    def format(self, record: logging.LogRecord) -> str:
        return ANSI_RE.sub("", self.inner.format(record))


log = logging.getLogger(TARGET)


def setup_logging() -> Ansi:
    """Log to the console, and to build.log which we remove if the build works."""
    ansi = Ansi(sys.stdout.isatty() and "NO_COLOR" not in os.environ)
    log.setLevel(logging.DEBUG)
    log.propagate = False

    console = logging.StreamHandler(sys.stdout)
    console.setFormatter(ConsoleFormatter(ansi))
    log.addHandler(console)

    file_handler = logging.FileHandler(LOG_FILE, mode="w", encoding="utf-8")
    file_handler.setFormatter(FileFormatter())
    log.addHandler(file_handler)
    return ansi


def close_log(remove: bool) -> None:
    for handler in list(log.handlers):
        handler.close()
        log.removeHandler(handler)
    if remove:
        LOG_FILE.unlink(missing_ok=True)


def step(msg: str) -> None:
    log.log(STEP, msg)


def echo(msg: str = "") -> None:
    log.log(logging.DEBUG, msg)


def info(label: str, value: object = "") -> None:
    """The aligned "label : value" lines that describe what was found."""
    log.info(f"{label:<27}: {value}")


class BuildError(Exception):
    """A failure that we report to the user, with the tech support info, and then exit."""


# --------------------------------------------------------------------------------------------------
# Running things
# --------------------------------------------------------------------------------------------------


def run(cmd: Sequence[str | os.PathLike[str]], *, cwd: Path | None = None, env: Mapping[str, str] | None = None, indent: str = "") -> int:
    """Run a command, with its output going to the console and to the log. Returns the exit code."""
    log.debug(f"{indent}$ {shlex.join(str(c) for c in cmd)}")
    try:
        with subprocess.Popen(
            [str(c) for c in cmd],
            cwd=cwd,
            env=None if env is None else dict(env),
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            errors="replace",
            bufsize=1,
        ) as proc:
            assert proc.stdout is not None
            for line in proc.stdout:
                log.debug(f"{indent}{line.rstrip()}")
            return proc.wait()
    except FileNotFoundError:
        log.debug(f"{indent}command not found: {cmd[0]}")
        return 127


def capture(cmd: Sequence[str | os.PathLike[str]], *, cwd: Path | None = None) -> str | None:
    """Run a command and return its output (stdout and stderr), or None if it is missing or fails."""
    try:
        done = subprocess.run(
            [str(c) for c in cmd], cwd=cwd, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace", check=False
        )
    except OSError:
        return None
    if done.returncode != 0:
        return None
    return (done.stdout + done.stderr).strip()


# --------------------------------------------------------------------------------------------------
# Options
# --------------------------------------------------------------------------------------------------


class Mode(enum.StrEnum):
    RELEASE = "rel"
    DEBUG = "dev"
    SANITY = "sanity"


class Compiler(enum.StrEnum):
    CLANG = "clang"
    GCC = "gcc"


@dataclasses.dataclass(frozen=True, slots=True)
class Options:
    mode: Mode
    profile: bool
    compiler: Compiler
    github: bool
    sdl3: bool
    web: bool
    gles: bool
    jobs: int | None
    clean: bool
    werror: bool | None

    @property
    def debug(self) -> bool:
        return self.mode in (Mode.DEBUG, Mode.SANITY)

    @property
    def sanity(self) -> bool:
        return self.mode is Mode.SANITY


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="RUNME.py",
        description="Build options:",
        allow_abbrev=False,
        epilog="The old single dash spellings (-dev, -sdl3 ...) are still accepted.",
    )

    def flag(*names: str, **kwargs: Any) -> None:
        # The old shell script accepted -dev as well as --dev, so we do too.
        parser.add_argument(*names, *(n[1:] for n in names if n.startswith("--") and len(n) > 3), **kwargs)

    flag("--rel", dest="mode", action="store_const", const=Mode.RELEASE, help="Release build (all optimizations) (DEFAULT)")
    flag("--dev", dest="mode", action="store_const", const=Mode.DEBUG, help="Enable additional debugging")
    flag("--sanity", dest="mode", action="store_const", const=Mode.SANITY, help="Enable additional stack checking")
    flag("--prof", dest="profile", action="store_true", help="Enable gprof")
    flag("--gcc", dest="compiler", action="store_const", const=Compiler.GCC, help="Prefer g++ over clang")
    flag("--clang", dest="compiler", action="store_const", const=Compiler.CLANG, help="Prefer clang over g++ (DEFAULT)")
    flag("--github", dest="github", action="store_true", help="CI/CD build (turns off modification of README) and runs the unit tests")
    flag("--sdl3", dest="sdl3", action="store_true", help="Build against SDL3 and SDL3_mixer instead of SDL2")
    flag("--web", dest="web", action="store_true", help="Enable emscripten build (implies --gles)")
    flag("--gles", dest="gles", action="store_true", help="Enable GLES build")
    parser.add_argument("-j", "--jobs", type=int, metavar="N", help="Number of parallel compile jobs (default: all cpus)")
    parser.add_argument("--no-clean", dest="clean", action="store_false", help="Incremental build; do not clean first")
    parser.add_argument(
        "--werror",
        action=argparse.BooleanOptionalAction,
        default=None,
        help="Treat warnings as errors (default: only for debug and sanity builds)",
    )
    parser.set_defaults(mode=Mode.RELEASE, compiler=Compiler.CLANG)
    return parser


def parse_options(argv: Sequence[str]) -> Options:
    parser = build_parser()
    args = parser.parse_args(argv)
    if args.jobs is not None and args.jobs < 1:
        parser.error("--jobs must be at least 1")
    return Options(
        mode=args.mode,
        profile=args.profile,
        compiler=args.compiler,
        github=args.github,
        sdl3=args.sdl3,
        web=args.web,
        gles=args.gles or args.web,
        jobs=args.jobs,
        clean=args.clean,
        werror=args.werror,
    )


# --------------------------------------------------------------------------------------------------
# The platform
# --------------------------------------------------------------------------------------------------


class Platform(enum.StrEnum):
    LINUX = "linux"
    MACOS = "macos"
    MINGW = "mingw"
    MSYS = "msys"
    OTHER = "other"


@dataclasses.dataclass(frozen=True, slots=True)
class Host:
    platform: Platform
    distro: str
    uname: str  # what "uname" would say, as it ends up in the makefile
    msys_prefix: Path  # where mingw is installed; only meaningful for MINGW
    msys_pkg_type: str

    @property
    def exe_suffix(self) -> str:
        return ".exe" if self.platform is Platform.MINGW else ""


# The MINGW environments, and the pacman package prefix for each:
#
#   mingw64 - the default; binaries use the older MSVCRT library that should be present on all Windows systems.
#   clang64 - the same, built with clang.
#   ucrt64  - the newer UCRT library that is only known to be present on Windows 10. It should produce
#             binaries more compatible with MSVC-compiled binaries, but not with the MSVCRT environment.
#
MSYS_PACKAGE_TYPES = {
    "mingw64": "mingw-w64",
    "clang64": "mingw-w64-clang",
    "ucrt64": "mingw-w64-ucrt",
}
DEFAULT_MSYSTEM = "ucrt64"


def detect_host() -> Host:
    system = platform.system()
    msystem = os.environ.get("MSYSTEM", "").lower()

    if system.startswith("MSYS"):
        kind = Platform.MSYS
    elif system.startswith("MINGW") or msystem:
        kind = Platform.MINGW
    elif system == "Darwin":
        kind = Platform.MACOS
    elif system == "Linux":
        kind = Platform.LINUX
    else:
        kind = Platform.OTHER

    distro = system.lower()
    if kind is Platform.LINUX:
        try:
            release = platform.freedesktop_os_release()
            distro = release.get("ID", distro).lower()
            like = release.get("ID_LIKE", "").lower().split()
            if distro not in ("ubuntu", "debian", "fedora"):
                distro = next((d for d in ("ubuntu", "debian", "fedora") if d in like), distro)
        except OSError:
            pass

    if msystem not in MSYS_PACKAGE_TYPES:
        msystem = DEFAULT_MSYSTEM
    prefix = Path(os.environ.get("MSYSTEM_PREFIX", f"/{msystem}"))

    uname = os.uname().sysname if hasattr(os, "uname") else system
    return Host(kind, distro, uname, prefix, MSYS_PACKAGE_TYPES[msystem])


# --------------------------------------------------------------------------------------------------
# Help with installing the things we need
# --------------------------------------------------------------------------------------------------

FEDORA_PACKAGES = [
    "automake",
    "findutils",
    "gcc-c++",
    "git",
    "libfishsound-devel",
    "libmikmod-devel",
    "make",
    "SDL2",
    "SDL2-devel",
    "SDL2_mixer",
    "SDL2_mixer-devel",
    "vim",
]

APT_PACKAGES = [
    "build-essential",
    "clang",
    "clang-format",
    "g++",
    "git",
    "libegl1",
    "libfishsound1-dev",
    "libflac-dev",
    "libfluidsynth-dev",
    "libfreetype6-dev",
    "libgl1",
    "libglvnd0",
    "libglx0",
    "libgles-dev",
    "libmikmod-dev",
    "liboggz2-dev",
    "libsdl2-2.0-0",
    "libsdl2-dev",
    "libsdl2-mixer-2.0-0",
    "libsdl2-mixer-dev",
    "libsmpeg-dev",
    "libx11-6",
    "libxext6",
    "liblz4-dev",
    "python3",
    "ssh",
    "vim",
    "xutils-dev",
]

MINGW_PACKAGES = [
    "ag",
    "binutils",
    "bzip2",
    "clang",
    "clang-tools-extra",
    "crt-git",
    "expat",
    "flac",
    "fluidsynth",
    "gcc",
    "gcc-libs",
    "gdb",
    "gdbm",
    "gettext",
    "giflib",
    "glib2",
    "gmp",
    "headers-git",
    "isl",
    "libffi",
    "libiconv",
    "libjpeg-turbo",
    "libmad",
    "libmodplug",
    "libogg",
    "libpng",
    "libsndfile",
    "libsystre",
    "libtiff",
    "libtre-git",
    "libunwind",
    "libvorbis",
    "libwebp",
    "libwinpthread-git",
    "lld",
    "llvm",
    "lz4",
    "mpc",
    "mpfr",
    "ncurses",
    "openssl",
    "portaudio",
    "python3",
    "readline",
    "SDL",
    "SDL2",
    "SDL2_mixer",
    "smpeg2",
    "speex",
    "speexdsp",
    "termcap",
    "windows-default-manifest",
    "winpthreads-git",
    "xz",
    "zlib",
]


def offer_install(command: Sequence[str], packages: Iterable[str]) -> None:
    """Show what we think is needed, and if we have a person to ask, offer to install it."""
    packages = list(packages)
    log.warning("You may need to install:")
    for package in packages:
        log.warning(f"  {shlex.join([*command, package])}")

    if not sys.stdin.isatty():
        return
    try:
        answer = input("Enter 'y' to run the above? ")
    except EOFError:
        return
    if answer.strip().lower() == "y":
        subprocess.run([*command, *packages], check=False)


def show_install_help(host: Host) -> None:
    match host.platform:
        case Platform.LINUX if host.distro == "fedora":
            offer_install(["dnf", "install", "-y"], FEDORA_PACKAGES)
        case Platform.LINUX if host.distro in ("ubuntu", "debian"):
            offer_install(["sudo", "apt-get", "install", "-y"], APT_PACKAGES)
        case Platform.MINGW | Platform.MSYS:
            packages = [f"{host.msys_pkg_type}-x86_64-{p}" for p in MINGW_PACKAGES]
            offer_install(["pacman", "-S", "--needed", "git", "make", "vim", "base-devel"], packages)
        case Platform.MACOS:
            log.warning("For MAC ports install:")
            log.warning("  sudo port install libsdl2 libsdl2_mixer")
            log.warning("  sudo port install SDL3 SDL3_mixer")
            log.warning("For homebrew install:")
            log.warning("  brew install make llvm automake sdl2 sdl2_mixer")


# --------------------------------------------------------------------------------------------------
# SDL
# --------------------------------------------------------------------------------------------------


@dataclasses.dataclass(slots=True)
class Sdl:
    cflags: list[str]
    libs: list[str]
    include_path: str  # passed to the makefile as SDL2_INC_PATH, for SDL3 as well


def find_sdl2_config(host: Host) -> Path | None:
    candidates: list[str | None] = [shutil.which("sdl2-config")]
    if host.platform is Platform.MINGW:
        candidates.append(str(host.msys_prefix / "bin" / "sdl2-config"))
    else:
        candidates += ["/opt/local/bin/sdl2-config", "/usr/bin/sdl2-config"]
    return next((Path(c) for c in candidates if c and os.access(c, os.X_OK)), None)


def configure_sdl2(host: Host, opts: Options) -> Sdl:
    config = find_sdl2_config(host)

    def config_output(*args: str) -> str:
        return (capture([config, *args]) or "") if config else ""

    cflags = config_output("--cflags").split()
    include_dirs = [c[2:] for c in cflags if c.startswith("-I") and len(c) > 2]
    include_path = include_dirs[-1] if include_dirs else ""

    mixer_header: Path | None = None
    if include_path and Path(include_path).is_dir():
        mixer_header = next(Path(include_path).rglob("SDL_mixer.h"), None)

    info("SDL2 config", config or "")
    info("SDL2 version", config_output("--version"))
    info("SDL2 libs", config_output("--libs"))
    info("SDL2 static libs", config_output("--static-libs"))
    info("SDL2 CFLAGS", " ".join(cflags))
    info("SDL2 prefix", config_output("--prefix"))
    info("SDL2 exec-prefix", config_output("--exec-prefix"))
    info("SDL2 include path", include_path)
    info("SDL2 mixer.h", mixer_header or "")

    if config is None or mixer_header is None:
        log.error("I need SDL2 and SDL2 mixer installed")
        show_install_help(host)
        raise BuildError("SDL2 and SDL2 mixer not found")

    libs = config_output("--libs").split()
    if not libs or not cflags:
        raise BuildError(f"{config} did not tell us how to compile and link")

    if opts.web:
        # Emscripten brings its own SDL; we only needed the include path
        return Sdl([], [], include_path)

    # Gives warnings at runtime on MACOS if we leave in -D_REENTRANT
    return Sdl([c for c in cflags if c != "-D_REENTRANT"], [*libs, "-lSDL2_mixer"], include_path)


def configure_sdl3(host: Host, opts: Options) -> Sdl:
    """Build against SDL3 and SDL3_mixer instead of SDL2. This defines USE_SDL3 for the sources."""
    if opts.web:
        raise BuildError("The web build does not support SDL3 yet; drop --sdl3 or --web")

    modules = ["sdl3", "sdl3-mixer"]
    if capture(["pkg-config", "--exists", *modules]) is None:
        log.error("I need SDL3 and SDL3 mixer installed (pkg-config must find sdl3 and sdl3-mixer)")
        show_install_help(host)
        raise BuildError("SDL3 and SDL3 mixer not found")

    def pkg_config(*args: str) -> str:
        return capture(["pkg-config", *args, *modules]) or ""

    cflags = [c for c in pkg_config("--cflags").split() if c != "-D_REENTRANT"]
    libs = pkg_config("--libs").split()
    info("SDL3 version", capture(["pkg-config", "--modversion", "sdl3"]))
    info("SDL3 mixer version", capture(["pkg-config", "--modversion", "sdl3-mixer"]))
    info("SDL3 CFLAGS", " ".join(cflags))
    info("SDL3 libs", " ".join(libs))
    include_path = capture(["pkg-config", "--variable=includedir", "sdl3"]) or ""
    return Sdl([*cflags, "-DUSE_SDL3"], libs, include_path)


# --------------------------------------------------------------------------------------------------
# The compiler and the flags
# --------------------------------------------------------------------------------------------------


@dataclasses.dataclass(slots=True)
class Toolchain:
    cc: str  # what goes in the makefile as CC
    chosen: str  # "gcc", "clang" or "em++"; selects which set of warning flags the makefile uses


def choose_toolchain(host: Host, opts: Options) -> Toolchain:
    if opts.web:
        return Toolchain("em++", "em++")

    if host.platform is Platform.MINGW:
        bindir = host.msys_prefix / "bin"
        gxx, clangxx = str(bindir / "g++.exe"), str(bindir / "clang++.exe")
        exists = lambda path: os.access(path, os.X_OK)  # noqa: E731
    else:
        gxx, clangxx = "g++", "clang++"
        exists = lambda name: shutil.which(name) is not None  # noqa: E731

    #
    # Take g++ if we have it, else clang++. Unless asked for gcc, prefer clang++ when we have it.
    #
    order = [(gxx, "gcc"), (clangxx, "clang")]
    if opts.compiler is Compiler.CLANG:
        order.reverse()
    for cc, chosen in order:
        if exists(cc):
            return Toolchain(cc, chosen)

    log.error("No g++ or clang compiler found")
    show_install_help(host)
    raise BuildError("No compiler found")


def read_version() -> str:
    match = re.search(r'MYVER\s*=\s*"?([^"\s]+)"?', VERSION_FILE.read_text(encoding="utf-8"))
    if match is None:
        raise BuildError(f"Cannot find MYVER in {VERSION_FILE}")
    return match.group(1)


def worth_enabling_werror(opts: Options) -> bool:
    """
    Better to leave off for production, and Windows builds also fail due to missing headers.
    Hard coded on for the maintainer.
    """
    if opts.werror is not None:
        return opts.werror
    if opts.debug:
        return True
    gitconfig = Path.home() / ".gitconfig"
    try:
        return "goblinhack" in gitconfig.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return False


@dataclasses.dataclass(slots=True)
class Settings:
    """Everything that ends up in build/Makefile.template."""

    cflags: list[str] = dataclasses.field(default_factory=list)
    ldflags: list[str] = dataclasses.field(default_factory=list)
    ldlibs: list[str] = dataclasses.field(default_factory=list)
    exe: str = ""
    dsym: str = ""
    werror: bool = False
    cc: str = ""
    chosen: str = ""
    cores: int | None = None
    sdl_include_path: str = ""
    stack_check: str = ""


def find_lz4(host: Host, sdl: Sdl, cflags: list[str]) -> bool:
    """Is lz4 available? If it is in an unusual place, add that to the include path."""
    candidates = [
        (Path("/usr/include/lz4.h"), None),
        (Path("/opt/local/include/lz4.h"), "/opt/local/include"),
        (host.msys_prefix / "include" / "lz4.h", str(host.msys_prefix / "include")),
    ]
    if sdl.include_path:
        # last resort
        candidates.append((Path(sdl.include_path).parent / "lz4.h", f"{sdl.include_path}/.."))
    for header, include in candidates:
        if header.is_file():
            if include:
                cflags.append(f"-I{include}")
            return True
    return False


def have_libunwind(host: Host) -> bool:
    match host.platform:
        case Platform.MINGW:
            return (host.msys_prefix / "lib" / "libunwind.a").is_file()
        case Platform.LINUX:
            return capture(["pkg-config", "--print-provides", "libunwind"]) is not None
    return False


def configure_settings(host: Host, opts: Options, sdl: Sdl) -> Settings:
    s = Settings()
    s.cflags += sdl.cflags
    s.cflags.append("-include cfg.hpp")
    s.ldlibs += sdl.libs
    s.sdl_include_path = sdl.include_path
    s.werror = worth_enabling_werror(opts)
    sanitizers = "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-common"

    gl_libs = "-lGLESv2"

    if not opts.web:
        match host.platform:
            case Platform.MSYS:
                raise BuildError("Please compile for ming64, not msys")

            case Platform.MINGW:
                s.exe = ".exe"
                s.ldlibs = [lib for lib in s.ldlibs if lib != "-lmingw32"]
                s.ldlibs += ["-funwind-tables", "-lstdc++", "-lstdc++exp"]
                s.ldlibs.append(gl_libs if opts.gles else "-lopengl32")
                s.ldlibs.append("-lpthread")
                if not opts.sdl3:
                    s.ldlibs.append(str(host.msys_prefix / "lib" / "libSDL2_mixer.a"))
                s.ldlibs.append(f"-L{host.msys_prefix / 'lib' / 'binutils'} -lbfd -lintl -ldbghelp -liberty")

                if have_libunwind(host):
                    s.ldlibs.append("-lunwind")
                    info("Have libunwind", "Yes")
                else:
                    info("Have libunwind", "No")

                #
                # Clang supports PDB debug file creation, but you need to do the following to enable it.
                #
                # -fno-emulated-tls is needed to work around a thread_local issue with lld
                #
                s.cflags.append("-g")
                s.ldflags.append("-g -Wl,--pdb=")  # the executable name is used for the PDB file
                if opts.compiler is Compiler.CLANG:
                    # GCC cannot be used with precompiled headers, and does not like these
                    s.cflags += ["-gcodeview", "-fno-emulated-tls"]
                    s.ldflags += ["-fno-emulated-tls", "-fuse-ld=lld"]

            case Platform.MACOS:
                s.ldlibs.append("-funwind-tables")
                s.ldlibs.append(gl_libs if opts.gles else "-Wl,-framework,Opengl")

                # Run in the background as it is slow and is only needed for debugging
                s.dsym = "dsymutil ${TARGET} &"

                if opts.sanity:
                    s.cflags.append(sanitizers)
                    s.ldflags.append(sanitizers)
                s.ldflags.append("-rdynamic")

                # Can't seem to link libunwind and get it to work, so we only report it
                info("Have libunwind", "Yes" if Path("/opt/local/libexec/llvm-devel/lib/libunwind/libunwind.a").is_file() else "No")

            case Platform.LINUX:
                s.ldlibs.append(gl_libs if opts.gles else "-lGL")
                if opts.sanity:
                    s.cflags.append(sanitizers)
                    s.ldflags.append(sanitizers)
                s.ldflags.append("-rdynamic")

                if have_libunwind(host):
                    s.ldlibs.append("-lunwind")
                    info("Have libunwind", "Yes")
                else:
                    info("Have libunwind", "No")

    if opts.web:
        s.exe = ".html"

    if opts.profile:
        s.cflags.append("-pg")
        s.ldflags.append("-pg")

    if opts.sanity:
        s.cflags.append("-DSANITY_BUILD")
        s.stack_check = "-fstack-protector-all"

    s.cflags.append("-DDEBUG_BUILD" if opts.debug else "-DRELEASE_BUILD")

    if opts.web:
        s.cflags.append("-DWEB_BUILD")
    if opts.gles:
        s.cflags.append("-DGLES_BUILD")
    if opts.github:
        s.cflags.append("-DGITHUB_BUILD")

    if find_lz4(host, sdl, s.cflags):
        #
        # LZ0 to LZ4 comparisons:
        #
        # LZ0 compressed 1239Mb (1299692095 bytes) -> 17Mb (18108144 bytes) took 122 ms
        # LZ0 decompress 17Mb (18108144 bytes) -> 1239Mb (1299692095 bytes) took 1311 ms (appdata/sacrecant/saved-slot-0)
        #
        # LZ4 compressed 1239Mb (1299692095 bytes) -> 15Mb (15982242 bytes) took 121 ms
        # LZ4 decompress 15Mb (15982242 bytes) -> 1239Mb (1299692095 bytes) took 63 ms (appdata/sacrecant/saved-slot-0)
        #
        s.cflags.append("-DUSE_LZ4")
        s.ldlibs.append("-llz4")
        info("Have LZ4", "Yes")
    else:
        info("Have LZ4", "No")

    llvm_path = Path(os.environ.get("LLVM_PATH", DEFAULT_LLVM_PATH))
    if (llvm_path / "bin" / "lld").is_file() and os.access(llvm_path / "bin" / "lld", os.X_OK):
        s.ldflags.append("-fuse-ld=lld")
        info("Have lld", "Yes")
    else:
        info("Have lld", "No")
    info("Have llvm", llvm_path / "bin")

    if opts.web:
        s.cflags += ["-s USE_SDL=2", "-s USE_SDL_IMAGE=2", "-s SDL2_IMAGE_FORMATS='[\"tga\"]'"]
        s.ldflags += [
            "--profiling-funcs",
            "-s ASSERTIONS=2",
            "-s USE_WEBGL2=1",
            "-s STACK_SIZE=655360",
            "-s ALLOW_MEMORY_GROWTH",
            "-s GL_UNSAFE_OPTS",
            "-s GL_FFP_ONLY",
            "-s LEGACY_GL_EMULATION=1",
            "-s WASM=1",
            "-s USE_SDL_MIXER=2",
            "-s USE_SDL=2",
            "-s USE_SDL_IMAGE=2",
            "-s SDL2_IMAGE_FORMATS='[\"tga\"]'",
        ]

    toolchain = choose_toolchain(host, opts)
    s.cc, s.chosen = toolchain.cc, toolchain.chosen

    if host.platform is Platform.MINGW and not opts.web:
        # To resolve WinMain, add these at the end again
        s.ldlibs.append("-lmingw32 -mwindows")
        if not opts.sdl3:
            lib = host.msys_prefix / "lib"
            s.ldlibs.append(f"{lib / 'libSDL2main.a'} -L{lib} -lSDL2main -lSDL2")

    if opts.web:
        s.ldlibs = []

    cpus = getattr(os, "process_cpu_count", os.cpu_count)()
    s.cores = opts.jobs or cpus
    return s


def render_makefile(s: Settings, host: Host, opts: Options) -> str:
    optimisation = "-O3 -ffast-math -g" if opts.mode is Mode.RELEASE else "-Og -g"
    flags = " ".join(part for part in ("-Werror" if s.werror else "", *s.cflags, optimisation) if part)
    compiler_warnings = "$(GCC_COMPILER_WARNINGS)" if s.chosen == "gcc" else "$(CLANG_COMPILER_WARNINGS)"
    core_args = f"-j {s.cores}" if s.cores else ""

    return (
        f"COMPILER_FLAGS={flags}\n"
        f"CC={s.cc}\n"
        f"TARGET={TARGET}\n"
        f"CHOSEN_COMPILER={s.chosen}\n"
        f"CORES={core_args}\n"
        f"CPP_STANDARD={CPP_STANDARD}\n"
        f"DSYM={s.dsym}\n"
        f"EXE={s.exe}\n"
        f"LDLIBS={' '.join(s.ldlibs)}\n"
        f"MY_OS_NAME={host.uname}\n"
        f"SDL2_INC_PATH={s.sdl_include_path}\n"
        f"{MAKEFILE_WARNINGS.replace('@@STACK@@', s.stack_check)}"
        f"LDFLAGS={' '.join(s.ldflags)}\n"
        f"COMPILER_WARNINGS={compiler_warnings}\n"
    )


# --------------------------------------------------------------------------------------------------
# Game data and the ramdisk
# --------------------------------------------------------------------------------------------------

DATA_ARCHIVES = ("gfx", "sounds", "music1", "music2")


def remove_mac_junk() -> None:
    """Remove the mac dot underscore files."""
    for path in ROOT.rglob("._*"):
        if path.is_file():
            path.unlink(missing_ok=True)


def extract_data_archives() -> None:
    """The game data is kept in tarballs; extract any that have not been or that have been updated."""
    for name in DATA_ARCHIVES:
        archive = DATA_DIR / f"{name}.tgz"
        folder = DATA_DIR / name
        if not archive.is_file():
            continue
        if folder.is_dir() and archive.stat().st_mtime <= folder.stat().st_mtime:
            continue
        step(f"Extracting {archive.relative_to(ROOT)}")
        try:
            with tarfile.open(archive, "r:gz") as tar:
                tar.extractall(ROOT, filter="data")
        except (tarfile.TarError, OSError) as err:
            raise BuildError(f"Failed to extract archive {archive}: {err}") from err
        folder.mkdir(exist_ok=True)
        folder.touch()
    remove_mac_junk()


#
# The files that go into the ramdisk, as globs relative to the data directory.
#
RAMDISK_PATTERNS = (
    "*/*.tga",
    "*/*/*.tga",
    "*/*/*/*.tga",
    "sounds/*/*.ogg",
    "music1/*.ogg",
    "music2/*.ogg",
)

#
# We rebuild the ramdisk if any of these are newer than the game.
#
RAMDISK_INPUTS = (
    "gfx.tgz",
    "sounds.tgz",
    "music1.tgz",
    "music2.tgz",
    "fonts/*.tga",
    "gfx/*.tga",
    "gfx/*/*.tga",
    "gfx/*/*/*.tga",
    "sounds/*/*.ogg",
    "music1/*.ogg",
    "music2/*.ogg",
)


def ramdisk_is_up_to_date(target: Path) -> bool:
    """If the game exists, see if any graphics, sounds etc... are newer than it."""
    try:
        target_time = target.stat().st_mtime
    except OSError:
        return False
    for pattern in RAMDISK_INPUTS:
        for path in DATA_DIR.glob(pattern):
            if path.stat().st_mtime > target_time:
                info("Ramdisk input is newer", path.relative_to(ROOT))
                return False
    return True


def ramdisk_needs_assembly(toolchain: Toolchain) -> bool:
    """
    Newer clang has #embed, so we can include the data files directly. Anything else needs the data to
    be added via assembly. The generated code works out which to use, but both have to be present.
    """
    if toolchain.chosen != "clang":
        return True
    match = re.search(r"clang version (\d+)", capture([toolchain.cc, "--version"]) or "")
    # Apple's version numbers do not match LLVM's, but they are always behind the 21 we need
    return match is None or int(match.group(1)) < CLANG_EMBED_MIN_VERSION


def generate_ramdisk(toolchain: Toolchain, target: Path) -> None:
    """
    Why a ramdisk? Oryx tiles request that the images are hidden reasonably, so this is my attempt;
    tarring the images during build and in the release adding the images into the binary itself, so we
    never ship the raw images.

    I no longer use Oryx, but it still seems useful.
    """
    if not DATA_DIR.is_dir():
        raise BuildError(f"No {DATA_DIR.relative_to(ROOT)} directory; is the game data missing?")

    if ramdisk_is_up_to_date(target):
        info("Ramdisk", "up to date")
        return

    files: dict[PurePosixPath, list[str]] = defaultdict(list)
    for pattern in RAMDISK_PATTERNS:
        for path in sorted(DATA_DIR.glob(pattern)):
            rel = PurePosixPath(path.relative_to(ROOT).as_posix())
            files[rel.parent].append(rel.name)

    entries = [(folder / name, name) for folder, names in sorted(files.items()) for name in names]
    if not entries:
        raise BuildError(f"No game data found in {DATA_DIR.relative_to(ROOT)}")
    info("Ramdisk", f"{len(entries)} files")

    if ramdisk_needs_assembly(toolchain):
        asm: list[list[str]] = [["// Ramdisk data for files that due to licensing cannot be exposed\n\n"] for _ in range(RAMDISK_ASM_FILES)]
        for count, (rel_path, name) in enumerate(entries):
            c_name = re.sub(r"[\-.]", "_", name)
            asm[count % RAMDISK_ASM_FILES].append(
                f".align 4\n.globl data_{c_name}_start_\ndata_{c_name}_start_:\n.incbin \"../{rel_path}\"\n"
                f".align 4\n.globl data_{c_name}_end_\ndata_{c_name}_end_:\n\n"
            )
        for number, chunks in enumerate(asm):
            if sys.platform == "linux":
                # This weird thing appears harmless but is a warning. Not sure how to check for it.
                chunks.append('\n.section .note.GNU-stack,"",%progbits\n')
            (SRC_DIR / f"ramdisk_data_{number}.S").write_text("".join(chunks), encoding="utf-8")

    #
    # This file references the assembly above to allow for easy loading of the files
    #
    out = ['#include "my_ramdisk.hpp"\n\nvoid ramdisk_init (void)\n{\n']
    for rel_path, name in entries:
        c_name = re.sub(r"[\-.]", "_", name)
        out.append(
            "    {\n"
            "#ifdef __has_embed\n"
            f'        static const unsigned char start [] = {{\n#embed "../{rel_path}"\n}};\n'
            "        ramdisk_t r;\n"
            "        r.data = start;\n"
            "        r.len = sizeof(start);\n"
            "#else\n"
            f'        extern unsigned char *data_{c_name}_start_\n           asm("data_{c_name}_start_");\n'
            f'        extern unsigned char *data_{c_name}_end_\n           asm("data_{c_name}_end_");\n'
            f"        static const unsigned char *const start =\n           (const unsigned char *const) (char*)&data_{c_name}_start_;\n"
            f"        static const unsigned char *const end   =\n           (const unsigned char *const) (char*)&data_{c_name}_end_;\n"
            "        ramdisk_t r;\n"
            "        r.data = start;\n"
            "        r.len = end - start;\n"
            "#endif\n"
            f'        ramdisk_data["{rel_path}"] = r;\n'
            "    }\n\n"
        )
    out.append("}\n")
    (SRC_DIR / "ramdisk_data.cpp").write_text("".join(out), encoding="utf-8")


# --------------------------------------------------------------------------------------------------
# Building
# --------------------------------------------------------------------------------------------------


def write_config_files(version: str, makefile: str) -> None:
    #
    # Common config file
    #
    CONFIG_HEADER.write_text(f'#include "my_cfg.hpp"\n#define MYVER "{version}"\n', encoding="utf-8")
    (SRC_DIR / "precompiled.hpp.gch").unlink(missing_ok=True)
    MAKEFILE_TEMPLATE.write_text(makefile, encoding="utf-8")


def make_command() -> str:
    make = shutil.which("make")
    if make is None:
        raise BuildError("make not found; please install it")
    return make


def run_make(*args: str, env: Mapping[str, str] | None = None, indent: str = "") -> int:
    return run([make_command(), "-C", SRC_DIR, *args], env=env, indent=indent)


WINDOWS_RELEASE_SCRIPT = """\
#!/bin/sh
###############################################################################
# Execute the following to build the release
###############################################################################
#
# Clean up the old installs
#
rm {target}.*installer.exe

#
# Update the version
#
echo $0: updating the version to {version}
sed -i "s/<version>.*/<version>{version}<\\/version>/g" build/windows/windows.xml
(cd build/windows/ ; ./windows.sh)


#
# All commits since last tag
#
echo $0: all commits since last tag
git log $(git describe --tags --abbrev=0)..HEAD --oneline | grep -E "feat:|bug:" | sort +1 -r

#
# Lay a tag
#
echo $0: laying a tag
git tag -m "tag: {version}" -a v{version}
git push origin --tags

echo $0: all done
"""

WINDOWS_RUN_SCRIPT = """\

#!/bin/sh
###############################################################################
# Execute the following, or run ./{target}.sh
###############################################################################
./{target}.exe $*
"""


def write_windows_helpers(version: str) -> None:
    """On Windows we leave some helper scripts behind for making a release, and for running the game."""
    log.info("Run:")
    release = ROOT / f"{TARGET}-create-release.sh"
    release.write_text(WINDOWS_RELEASE_SCRIPT.format(target=TARGET, version=version), encoding="utf-8", newline="\n")
    release.chmod(0o755)

    runner = ROOT / f"{TARGET}.sh"
    text = WINDOWS_RUN_SCRIPT.format(target=TARGET)
    runner.write_text(text, encoding="utf-8", newline="\n")
    runner.chmod(0o755)
    echo(text)


def run_unit_tests(exe: Path) -> None:
    """Used by the CI/CD build."""
    log.info("Running tests:")
    pdb = OBJ_DIR / f"{TARGET}.pdb"
    if pdb.is_file():
        shutil.copy2(pdb, ROOT)

    env = {**os.environ, "LSAN_OPTIONS": "max_leaks=10"}
    if run([exe, "--tests", "--debug", "--repeat", "3"], cwd=ROOT, env=env) == 0:
        return

    appdata = os.environ.get("APPDATA")
    if appdata:
        for entry in sorted(Path(appdata).iterdir()):
            echo(str(entry))
        shutil.copytree(Path(appdata) / TARGET, ROOT / "appdata" / TARGET, dirs_exist_ok=True)

    for entry in sorted(ROOT.iterdir()):
        echo(str(entry))
    stdout = ROOT / "appdata" / TARGET / "stdout.txt"
    if stdout.is_file():
        echo(stdout.read_text(encoding="utf-8", errors="replace"))
    raise BuildError("The unit tests failed")


def build(opts: Options, host: Host) -> None:
    version = read_version()

    sdl = configure_sdl3(host, opts) if opts.sdl3 else configure_sdl2(host, opts)
    settings = configure_settings(host, opts, sdl)
    toolchain = Toolchain(settings.cc, settings.chosen)
    exe = ROOT / f"{TARGET}{settings.exe}"

    write_config_files(version, render_makefile(settings, host, opts))

    info("CFLAGS", " ".join(settings.cflags))
    info("LDFLAGS", " ".join(settings.ldflags))
    info("LDLIBS", " ".join(settings.ldlibs))

    log.info(f"Compiling ({settings.cores} cpus)" if settings.cores else "Compiling")

    if opts.clean:
        log.info("Cleaning")
        if run_make("clobber", indent="  ") != 0:
            raise BuildError("make clobber failed")

    extract_data_archives()
    generate_ramdisk(toolchain, exe)

    info("OS version")
    for release_file in sorted(Path("/etc").glob("*-release")):
        echo(release_file.read_text(encoding="utf-8", errors="replace").rstrip())
    echo(" ".join(platform.uname()))

    info("Game version", version)
    cc_version = capture([settings.cc, "--version"])
    info("Compiler version", cc_version.splitlines()[0] if cc_version else "unknown")

    log.info("Building...")
    make_env = {**os.environ, "USE_PRECOMPILED": "yep"}
    if run_make("all", *(["-j", str(settings.cores)] if settings.cores else []), env=make_env) != 0:
        raise BuildError("Build failed")

    log.info("Compiled!")

    if host.platform is Platform.MACOS and not opts.web:
        run(["dsymutil", exe], cwd=ROOT)
    elif host.platform is Platform.MINGW:
        write_windows_helpers(version)

    log.info("Run:")
    if opts.web:
        echo("  python -m http.server")
    else:
        echo(RUN_HINTS)

    (SRC_DIR / "Makefile.bak").unlink(missing_ok=True)
    (ROOT / "Makefile.bak").unlink(missing_ok=True)

    if opts.github:
        run_unit_tests(exe)


# --------------------------------------------------------------------------------------------------
# Tech support
# --------------------------------------------------------------------------------------------------

SECRET_ENV_RE = re.compile(r"KEY|TOKEN|SECRET|PASSW|PASSPHRASE|CREDENTIAL|AUTH", re.IGNORECASE)


def tech_support(host: Host) -> None:
    """Record some useful info to send back home on fail. Secrets are removed from the environment."""

    def section(title: str) -> None:
        echo(f"{title}\n{'=' * 53}")

    def show(cmd: Sequence[str], pattern: str | None = None) -> None:
        output = capture(cmd)
        if output is None:
            return
        lines = output.splitlines()
        if pattern is not None:
            lines = [line for line in lines if re.search(pattern, line, re.IGNORECASE)]
        for line in lines:
            echo(line)

    log.info("DEBUG: System:")
    section("System")
    echo(" ".join(platform.uname()))
    echo(f"Python {platform.python_version()} ({sys.executable})")
    echo()

    log.info("DEBUG: Environment:")
    section("Environment")
    for key, value in sorted(os.environ.items()):
        echo(f"{key}={'<removed>' if SECRET_ENV_RE.search(key) else value}")
    echo()

    log.info("DEBUG: Installed packages:")
    section("Packages")
    show(["apt", "--installed", "list"], r"SDL|gcc|clang")
    show(["pacman", "-Q"])  # msys2
    echo()

    log.info("DEBUG: Installed compilers:")
    section("Compilers")
    for name in ("clang++", "clang", "g++", "gcc", "em++"):
        path = shutil.which(name)
        if path:
            echo(path)
            show([path, "--version"])
    gxx = host.msys_prefix / "bin" / "g++.exe"
    if gxx.is_file():
        show([str(gxx), "--version"])
    echo()

    log.info("DEBUG: Make:")
    section("Make")
    echo(shutil.which("make") or "make not found")
    show(["make", "--version"])
    echo()

    for makefile in (ROOT / "Makefile", MAKEFILE_TEMPLATE, SRC_DIR / "Makefile"):
        if makefile.is_file():
            log.info(f"DEBUG: {makefile.relative_to(ROOT)}:")
            section(str(makefile.relative_to(ROOT)))
            echo(makefile.read_text(encoding="utf-8", errors="replace"))
            echo()


# --------------------------------------------------------------------------------------------------
# main
# --------------------------------------------------------------------------------------------------


def main(argv: Sequence[str] | None = None) -> int:
    opts = parse_options(sys.argv[1:] if argv is None else argv)

    os.chdir(ROOT)
    ansi = setup_logging()
    echo(f"{ansi.red}{BANNER}{ansi.reset}")

    if GNU_BIN_DIR.is_dir():
        # Prefer gnu over freebsd coreutils
        os.environ["PATH"] = f"{GNU_BIN_DIR}{os.pathsep}{os.environ.get('PATH', '')}"

    host = detect_host()
    info("Distro/os", host.distro)

    try:
        build(opts, host)
    except BuildError as err:
        log.error(str(err))
        log.error("Build failed.")
        log.info(f"Gathering tech support info to {LOG_FILE.name}")
        tech_support(host)
        log.error(f"Could you send {LOG_FILE.name} to {MAINTAINER}?")
        log.error(f"Or file an issue at {ISSUES_URL}")
        close_log(remove=False)
        return 1
    except KeyboardInterrupt:
        log.error("Interrupted")
        close_log(remove=True)
        return 130

    #
    # For quick re-makes
    #
    step("Built successfully")
    close_log(remove=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
