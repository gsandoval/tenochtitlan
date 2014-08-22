APPLICATION=tenochtitlan

IDIR =./include
CXX=g++
CXXFLAGS=-I$(IDIR) -std=c++11 -Wall -g

ODIR=build
LDIR =../lib

LIBS=-lm

_DEPS = management/disposable.h management/applicationlifecyclelistener.h socket/socketexception.h \
	socket/tcpsocket.h socket/servertcpsocket.h socket/clienttcpsocket.h socket/tcpclientconnection.h \
	socket/tcpclientconnectionhandler.h server/socketserver.h server/socketserverworker.h \
	server/httpsocketserverworker.h server/rawsocketserverworker.h server/socketserverworkercreator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = management/applicationlifecyclelistener.o socket/socketexception.o socket/tcpsocket.o \
	socket/servertcpsocket.o socket/clienttcpsocket.o socket/tcpclientconnection.o server.o \
	server/socketserver.o server/httpsocketserverworker.o server/rawsocketserverworker.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: $(OBJS)
	g++ -o $(APPLICATION) $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm $(APPLICATION)