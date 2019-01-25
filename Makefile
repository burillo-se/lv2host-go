all:
	c-for-go lv2host.yml

clean:
	rm -f lv2host/cgo_helpers.go lv2host/cgo_helpers.h lv2host/doc.go lv2host/types.go lv2host/const.go
	rm -f lv2host/lv2host.go

test:
	cd lv2host && cd lv2host && make && cd .. && go build
