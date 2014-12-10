all : asd asd_fast

asd : asd.cc
	g++ -D _SUNOS asd.cc /materialy/AISDI/tree/main.cc /materialy/AISDI/tree/timer.cc /materialy/AISDI/tests/ltest_tree.so -o asd 
asd_fast : asd.cc
	g++ -O2 -D NDEBUG -D _SUNOS asd.cc /materialy/AISDI/tree/main.cc /materialy/AISDI/tree/timer.cc /materialy/AISDI/tests/ltest_tree.so -o asd_fast
del :
	rm asd

debug : 
	g++ -g -D _SUNOS asd.cc /materialy/AISDI/tree/main.cc /materialy/AISDI/tree/timer.cc /materialy/AISDI/tests/ltest_tree.so -o asd_debug 
	gdb asd_debug

view:
	lynx /materialy/AISDI/tree/info/index.html
