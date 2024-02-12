ALL = midi_tester mconvert 

all: $(ALL)

midi_tester: midi_tester.o cosc302-midi-base.o cosc302-midi.o 
	g++ -o midi_tester midi_tester.o cosc302-midi-base.o cosc302-midi.o

mconvert: mconvert.o cosc302-midi-base.o cosc302-midi.o 
	g++ -o mconvert mconvert.o cosc302-midi-base.o cosc302-midi.o

midi_tester.o: cosc302-midi.h
cosc302-midi-base.o: cosc302-midi.h
cosc302-midi.o: cosc302-midi.h
mconvert.o: cosc302-midi.h

clean:
	rm -f *.o $(ALL) core

.SUFFIXES: .cpp .o

.cpp.o:
	g++ -c $*.cpp
