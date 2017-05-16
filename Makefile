TARGET=srtm3-backend
CXX=g++-4.9
LIBRARIES=-L/usr/local/lib
INCLUDES=-I/usr/local/include
CXXFLAGS=-std=c++11 -Wall -g -ggdb -O3 -pthread -lrestbed -lssl -lcrypto -ldl

OBJS=obj/srtm3-backend.o obj/SrtmHgtReader.o

$(TARGET):$(OBJS)
			 $(CXX) $(LIBRARIES) $(INCLUDES) $^ -o $@ $(CXXFLAGS)

obj/srtm3-backend.o:src/srtm3-backend.cc
			  $(CC) $(CXXFLAGS) $< -c -o $@

obj/SrtmHgtReader.o:src/SrtmHgtReader.cc include/SrtmHgtReader.h
			           $(CC) $(CXXFLAGS) $< -c -o $@

clean:
		 ${RM} $(OBJS) $(TARGET)
