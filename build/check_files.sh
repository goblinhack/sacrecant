#!/bin/bash

untar() {
    tar -zxf $*
    if [[ $? -ne 0 ]]; then
        echo $0: Failed to extract archive $*
        exit 1
    fi
}

make_tar() {
    # https://stackoverflow.com/questions/51655657/tar-ignoring-unknown-extended-header-keyword-libarchive-xattr-security-selinux
    COPYFILE_DISABLE=1 tar --no-xattrs -zcvf $*
    if [[ $? -ne 0 ]]; then
        tar zcvf $*
        if [[ $? -ne 0 ]]; then
            echo $0: Failed to create archive $*
            exit 1
        fi
    fi
}

if [[ ! -d data/gfx ]]; then
    echo $0: Need to extract graphics archive
    untar data/gfx.tgz
    DONE=1
fi

if [[ ! -d data/sounds ]]; then
    echo $0: Need to extract sounds archive
    untar data/sounds.tgz
    DONE=1
fi

if [[ ! -d data/music1 ]]; then
    echo $0: Need to extract music1 archive
    untar data/music1.tgz
    DONE=1
fi

if [[ ! -d data/music2 ]]; then
    echo $0: Need to extract music2 archive
    untar data/music2.tgz
    DONE=1
fi

if [[ $DONE -eq 1 ]]; then
    exit 0
fi

#
# Remove mac dot underscore files
#
find . -type f -name '._*' -delete

COUNT=$(find data/gfx -newer data/gfx.tgz -type f | wc -l)
if [[ $COUNT -gt 0 ]];
then
    echo $0: Need to retar graphics tarball due to updates
    make_tar data/gfx.tgz data/gfx
else
    COUNT=$(find data/gfx.tgz -newer data/gfx -type f | wc -l)
    if [[ $COUNT -gt 0 ]];
    then
        echo $0: Need to extract data/gfx.tgz as it is newer
        untar data/gfx.tgz
        touch data/gfx
    fi
fi

COUNT=$(find data/sounds -newer data/sounds.tgz -type f | wc -l)
if [[ $COUNT -gt 0 ]];
then
    echo $0: Need to retar sounds tarball due to updates
    make_tar data/sounds.tgz data/sounds
else
    COUNT=$(find data/sounds.tgz -newer data/sounds -type f | wc -l)
    if [[ $COUNT -gt 0 ]];
    then
        echo $0: Need to extract data/sounds.tgz as it is newer
        untar data/sounds.tgz
        touch data/sounds
    fi
fi

COUNT=$(find data/music1 -newer data/music1.tgz -type f | wc -l)
if [[ $COUNT -gt 0 ]];
then
    echo $0: Need to retar music1 tarball due to updates
    make_tar data/music1.tgz data/music1
else
    COUNT=$(find data/music1.tgz -newer data/music1 -type f | wc -l)
    if [[ $COUNT -gt 0 ]];
    then
        echo $0: Need to extract data/music1.tgz as it is newer
        untar data/music1.tgz
        touch data/music1
    fi
fi

COUNT=$(find data/music2 -newer data/music2.tgz -type f | wc -l)
if [[ $COUNT -gt 0 ]];
then
    echo $0: Need to retar music2 tarball due to updates
    make_tar data/music2.tgz data/music2
else
    COUNT=$(find data/music2.tgz -newer data/music2 -type f | wc -l)
    if [[ $COUNT -gt 0 ]];
    then
        echo $0: Need to extract data/music2.tgz as it is newer
        untar data/music2.tgz
        touch data/music2
    fi
fi

#
# Remove mac dot underscore files
#
find . -type f -name '._*' -delete
