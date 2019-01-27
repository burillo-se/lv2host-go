all: static

static: liblv2host-c.a

dynamic: liblv2host-c.so

liblv2host-c.a: lv2host.o lv2host-c.o
	ar rcs build/liblv2host-c.a build/lv2host.o build/lv2host-c.o

liblv2host-c.so: lv2host.cpp lv2host-c.cpp
	gcc -o build/liblv2host-c.so -O3 -shared -fPIC -lstdc++ -llilv-0 -I/usr/include/lilv-0/ lv2host.cpp lv2host-c.cpp

lv2host.o: lv2host.cpp
	mkdir -p build
	gcc -o build/lv2host.o -O3 -c -static -lstdc++ -llilv -I/usr/include/lilv-0/ lv2host.cpp

lv2host-c.o: lv2host-c.cpp
	gcc -o build/lv2host-c.o -O3 -c -static -lstdc++ -llilv -I/usr/include/lilv-0/ lv2host-c.cpp

clean:
	rm -rf build/
