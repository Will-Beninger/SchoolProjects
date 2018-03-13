all: face_detection

face_detection: face_detection.cpp
	g++ -I /usr/include/netpbm -o face_detection face_detection.cpp -Wall -lnetpbm

clean:
	rm -f *~

clean-all: clean
	rm -f face_detection
