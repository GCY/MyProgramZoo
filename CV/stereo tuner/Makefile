all: main.cpp
	g++ -o2 -o main main.cpp -L /usr/local/opt/libffi/lib `pkg-config --cflags gtk+-3.0 gmodule-2.0 --libs` `pkg-config opencv4 --cflags --libs` `pkg-config gmodule-export-2.0 --libs` -std=c++11 -m64
	./main -left l.jpg -right r.jpg -intrinsics intrinsics.yml -extrinsics extrinsics.yml
	#./main -left scene1.row3.col3.ppm -right scene1.row3.col5.ppm
