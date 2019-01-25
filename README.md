lv2host-go
==========

The package provides Go bindings for a simple in-project LV2 plugin host.
All the binding code has automatically been generated with rules defined in [lv2host.yml](/lv2host.yml).

### Usage

```
$ go get github.com/burillo-se/lv2host-go/lv2host
```

### Rebuilding the package

You will need to get the [c-for-go](https://git.io/cgogen) tool installed first.

```
$ git clone https://github.com/burillo-se/lv2host-go && cd lv2host-go
$ make clean
$ make
```
