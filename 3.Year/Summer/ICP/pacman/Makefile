# ICP - pacman
# @brief Makefile
# @authors	Matus Vrablik(xvrabl05)

all:
	cmake -S . -B out/build/ && make --directory=out/build/

run: clean all
	out/build/pacman	

clean:
	rm -rf out/build/*
	rm -rf doc/*

pack: clean
	zip -x */.\* -r xvrabl05-xhorva14.zip src/ Headers/ Forms/ Resources/ CMakeLists.txt CMakePresets.json out/build/ README.txt doc examples Makefile Doxyfile -x out/build/*\* doc/*\*

doxygen:
	doxygen Doxyfile