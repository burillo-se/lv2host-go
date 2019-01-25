// THE AUTOGENERATED LICENSE. ALL THE RIGHTS ARE RESERVED BY ROBOTS.

// WARNING: This file has automatically been generated on Fri, 25 Jan 2019 22:19:09 GMT.
// Code generated by https://git.io/c-for-go. DO NOT EDIT.

package lv2host

/*
#cgo pkg-config: lilv-0
#cgo LDFLAGS: -L./lv2host/ -llv2host-c -lstdc++
#include "lv2host/lv2host-c.h"
#include <stdlib.h>
#include "cgo_helpers.h"
*/
import "C"
import (
	"runtime"
	"unsafe"
)

// Alloc function as declared in lv2host/lv2host-c.h:15
func Alloc(SampleRate float64) *CLV2Host {
	cSampleRate, _ := (C.double)(SampleRate), cgoAllocsUnknown
	__ret := C.lv2host_alloc(cSampleRate)
	__v := *(**CLV2Host)(unsafe.Pointer(&__ret))
	return __v
}

// Free function as declared in lv2host/lv2host-c.h:20
func Free(Host *CLV2Host) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	C.lv2host_free(cHost)
}

// Reset function as declared in lv2host/lv2host-c.h:25
func Reset(Host *CLV2Host) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	C.lv2host_reset(cHost)
}

// Activate function as declared in lv2host/lv2host-c.h:33
func Activate(Host *CLV2Host) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	C.lv2host_activate(cHost)
}

// Deactivate function as declared in lv2host/lv2host-c.h:38
func Deactivate(Host *CLV2Host) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	C.lv2host_deactivate(cHost)
}

// HasLatency function as declared in lv2host/lv2host-c.h:43
func HasLatency(Host *CLV2Host) int32 {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	__ret := C.lv2host_has_latency(cHost)
	__v := (int32)(__ret)
	return __v
}

// Latency function as declared in lv2host/lv2host-c.h:50
func Latency(Host *CLV2Host) int32 {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	__ret := C.lv2host_latency(cHost)
	__v := (int32)(__ret)
	return __v
}

// NumPlugins function as declared in lv2host/lv2host-c.h:55
func NumPlugins(Host *CLV2Host) uint {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	__ret := C.lv2host_num_plugins(cHost)
	__v := (uint)(__ret)
	return __v
}

// ListAvailablePlugins function as declared in lv2host/lv2host-c.h:60
func ListAvailablePlugins(Host *CLV2Host) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	C.lv2host_list_available_plugins(cHost)
}

// ListPluginParameters function as declared in lv2host/lv2host-c.h:66
func ListPluginParameters(Host *CLV2Host, Index uint32) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	cIndex, _ := (C.uint)(Index), cgoAllocsUnknown
	C.lv2host_list_plugin_parameters(cHost, cIndex)
}

// AddPluginInstance function as declared in lv2host/lv2host-c.h:75
func AddPluginInstance(Host *CLV2Host, Uri string) int32 {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	Uri = safeString(Uri)
	cUri, _ := unpackPCharString(Uri)
	__ret := C.lv2host_add_plugin_instance(cHost, cUri)
	runtime.KeepAlive(Uri)
	__v := (int32)(__ret)
	return __v
}

// SetPluginParameter function as declared in lv2host/lv2host-c.h:83
func SetPluginParameter(Host *CLV2Host, Index uint32, Sym string, Value float32) int32 {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	cIndex, _ := (C.uint)(Index), cgoAllocsUnknown
	Sym = safeString(Sym)
	cSym, _ := unpackPCharString(Sym)
	cValue, _ := (C.float)(Value), cgoAllocsUnknown
	__ret := C.lv2host_set_plugin_parameter(cHost, cIndex, cSym, cValue)
	runtime.KeepAlive(Sym)
	__v := (int32)(__ret)
	return __v
}

// ProcessBuffer function as declared in lv2host/lv2host-c.h:94
func ProcessBuffer(Host *CLV2Host, Left unsafe.Pointer, Right unsafe.Pointer, Samples uint32) {
	cHost, _ := (C.CLV2Host)(unsafe.Pointer(Host)), cgoAllocsUnknown
	cLeft, _ := Left, cgoAllocsUnknown
	cRight, _ := Right, cgoAllocsUnknown
	cSamples, _ := (C.uint)(Samples), cgoAllocsUnknown
	C.lv2host_process_buffer(cHost, cLeft, cRight, cSamples)
}