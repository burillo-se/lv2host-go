lv2host-go
==========

The package provides Go bindings for a simple in-project LV2 plugin host.
All the binding code has automatically been generated with rules defined in [lv2host.yml](/lv2host.yml).

### Usage

```
$ go get github.com/burillo-se/lv2host-go/lv2host
```
This library relies on a precompiled C/C++ library. Compiled Linux static library is included within the repository, so in most cases, things should work, however if you're not on Linux (or if for some reason the precompiled library doesn't work for you), the C/C++ library depends on [lilv](http://drobilla.net/software/lilv), a C library for writing LV2 plugin hosts.

### Rebuilding the package

You will need to get the [c-for-go](https://git.io/cgogen) tool installed first.

```
$ git clone https://github.com/burillo-se/lv2host-go && cd lv2host-go
$ make clean
$ make
```
