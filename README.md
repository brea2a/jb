# jo

![jo logo](tests/jo-logo.png)

This is `jo`, a small utility to create JSON objects

```bash
$ jo -p name=jo n=17 parser=false
{
    "name": "jo",
    "n": 17,
    "parser": false
}
```

or arrays

```bash
$ seq 1 10 | jo -a
[1,2,3,4,5,6,7,8,9,10]
```

It has a [manual](jo.md), and you can read [why I wrote jo](http://jpmens.net/2016/03/05/a-shell-command-to-create-json-jo/).

## Build from Release tarball

To build from [a release](https://github.com/jpmens/jo/releases) you will need a C compiler to install from a source tarball which you download from the [Releases page](https://github.com/jpmens/jo/releases).

```bash
tar xvzf jo-1.0.tar.gz
cd jo-1.0
./configure
make check
make install
```

[![asciicast](https://asciinema.org/a/4y7471mjfhvv2x4mdqmwfhu31.png)](https://asciinema.org/a/4y7471mjfhvv2x4mdqmwfhu31)


## Build from Github

[![Build Status](https://api.travis-ci.org/jpmens/jo.svg?branch=master)](https://travis-ci.org/jpmens/jo)

To install from the repository, you will need a C compiler as well as a relatively recent version of _automake_ and _autoconf_.

```bash
git clone git://github.com/jpmens/jo.git
cd jo
autoreconf -i
./configure
make check
make install
```

## Homebrew

```bash
brew install jo
```

## Ubuntu

To install on Ubuntu, use [this PPA](https://launchpad.net/~duggan/+archive/ubuntu/jo):

```
apt-add-repository ppa:duggan/jo --yes
apt-get update -q
apt-get install jo
```

## Others

* [voidlinux](https://github.com/voidlinux/void-packages/tree/master/srcpkgs/jo)
* [ArchLinux](https://aur.archlinux.org/packages/jo/)
* [OpenBSD](http://openports.se/textproc/jo)
* [pkgsrc](http://pkgsrc.se/textproc/jo)
