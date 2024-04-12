wave: main.c wave.h wave.c
	gcc main.c wave.c -o wave
	
clean:
	rm -f wave
	
run: wave
	./wave -e 1 .25 < crosstown.wav > output1.wav
	
debug: main.c wave.c wave.h
	gcc –ggdb main.c wave.c –o wave
	
testRun: wave
	./wave -r -s .9 -f -v .75 -o 1 -e .1 .25 -i 1 < rain.wav > output1.wav
	./waveTest.exe -r -s .9 -f -v .75 -o 1 -e .1 .25 -i 1 < rain.wav > output2.wav
