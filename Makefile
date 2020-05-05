build:
	g++ -Wall -c library.cpp -o build/llibrary.o
	ar rcs build/llibrary.a build/llibrary.o
	g++ -Wall -c sender.cpp -o build/sender.o
	g++ -Wall -c server.cpp -o build/server.o
	g++ build/sender.o build/llibrary.a -o build/sender -lgmp
	g++ build/server.o build/llibrary.a -o build/server -lgmp


clean:
	rm -f build/sender
	rm -f build/sender.o
	rm -f build/server
	rm -f build/server.o
	rm -f build/main
	rm -f build/main.o
	rm -f build/llibrary.a
	rm -f build/llibrary.o
	rm -f build/stream.txt
	rm -f build/key_file.txt

run:
	gnome-terminal -- ./build/server $(key_size) $(output)
	gnome-terminal -- ./build/sender $(output)

kill:
	killall -9 ./build/server
	killall -9 ./build/sender
	rm -f stream.txt
	rm -f key_file.txt