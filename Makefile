APPLICATION=tenochtitlan

IDIR=./include
CXX=g++
CXXFLAGS=-I$(IDIR) -I./lib/libev-4.15 -L./lib/libev-4.15/.libs -std=c++11 -Wall -g

ODIR=build

LIBS=-lm -lev

_DEPS = management/disposable.h management/applicationlifecyclelistener.h socket/socketexception.h \
	socket/tcpsocket.h socket/servertcpsocket.h socket/clienttcpsocket.h socket/tcpclientconnection.h \
	socket/tcpclientconnectionhandler.h server/socketserver.h server/socketserverworker.h \
	server/httpsocketserverworker.h server/rawsocketserverworker.h server/socketserverworkercreator.h \
	server/socketserverthread.h parser/jsonparser.h parser/httpparser.h http/httpserver.h \
	http/httprequestprocessor.h http/httpentity.h http/httpsocketserverworkercreator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = management/applicationlifecyclelistener.o socket/socketexception.o socket/tcpsocket.o \
	socket/servertcpsocket.o socket/clienttcpsocket.o socket/tcpclientconnection.o server.o \
	server/socketserver.o server/socketserverworker.o server/httpsocketserverworker.o \
	server/rawsocketserverworker.o server/socketserverthread.o parser/jsonparser.o parser/httpparser.o \
	http/httpserver.o http/httprequestprocessor.o http/httpentity.o http/httpsocketserverworkercreator.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: $(OBJS)
	$(CXX) -o $(APPLICATION) $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm $(APPLICATION)