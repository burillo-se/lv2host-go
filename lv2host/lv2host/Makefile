all: liblv2host-c.a

liblv2host-c.a: lv2host.o lv2host-c.o
	ar rcs liblv2host-c.a lv2host.o lv2host-c.o

lv2host.o: lv2host.cpp
	gcc -O3 -c -static -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic -llilv-0 -I/usr/include/lilv-0/ lv2host.cpp

lv2host-c.o: lv2host-c.cpp
	gcc -O3 -c -static -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic -llilv-0 -I/usr/include/lilv-0/ lv2host-c.cpp

clean:
	rm *.o *.a
