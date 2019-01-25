all: static

static: liblv2host-c.a

dynamic: liblv2host-c.so

liblv2host-c.a: lv2host.o lv2host-c.o
	ar rcs liblv2host-c.a lv2host.o lv2host-c.o

liblv2host-c.so: lv2host.cpp lv2host-c.cpp
	gcc -o liblv2host-c.so -O3 -shared -fPIC -lstdc++ -llilv-0 -I/usr/include/lilv-0/ lv2host.cpp lv2host-c.cpp

lv2host.o: lv2host.cpp
	gcc -O3 -c -static -lstdc++ -llilv -I/usr/include/lilv-0/ lv2host.cpp

lv2host-c.o: lv2host-c.cpp
	gcc -O3 -c -static -lstdc++ -llilv -I/usr/include/lilv-0/ lv2host-c.cpp

clean:
	rm *.o *.a *.so
