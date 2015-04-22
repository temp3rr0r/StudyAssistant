g++ -O2 `pkg-config --cflags --libs opencv` -I/usr/include/postgresql/ -L/usr/lib/postgresql/9.1/lib/ -lwiringPi -lpq -std=c++11 -o bin/StudyHelper_srv StudyHelper_srv.cpp
