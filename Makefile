APPLICATION=tenochtitlan

IDIR=./include
CXX=g++
CXXFLAGS=-I$(IDIR) -I./lib/libev -std=c++11 -Wall -g

ODIR=build

LIBS=./lib/libev/.libs/libev.a

_DEPS = management/logger.h management/disposable.h management/applicationlifecyclelistener.h socket/buffer.h socket/socketexception.h \
	socket/tcpsocket.h socket/servertcpsocket.h socket/clienttcpsocket.h socket/tcpclientconnection.h \
	socket/tcpclientconnectionhandler.h server/socketserver.h server/socketserverworker.h \
	http/httpsocketserverworker.h server/rawsocketserverworker.h server/socketserverworkercreator.h \
	server/socketserverthread.h parser/jsonparser.h parser/httpparser.h http/httpserver.h \
	http/httprequestprocessor.h http/httpentity.h http/httpsocketserverworkercreator.h \
	http/component/httpcomponent.h http/component/httpcomponentfactory.h http/component/httpcontext.h \
	http/component/restcomponent.h http/component/staticresourcecomponent.h util/properties.h \
	http/component/inputvalidationcomponent.h http/content/httpcontent.h http/content/httpfilecontent.h \
	http/content/httpstringcontent.h util/filereadercache.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = management/logger.o management/applicationlifecyclelistener.o socket/buffer.o socket/socketexception.o socket/tcpsocket.o \
	socket/servertcpsocket.o socket/clienttcpsocket.o socket/tcpclientconnection.o server.o \
	server/socketserver.o server/socketserverworker.o http/httpsocketserverworker.o \
	server/rawsocketserverworker.o server/socketserverthread.o parser/jsonparser.o parser/httpparser.o \
	http/httpserver.o http/httprequestprocessor.o http/httpentity.o http/httpsocketserverworkercreator.o \
	http/component/httpcomponent.o http/component/httpcomponentfactory.o http/component/httpcontext.o \
	http/component/restcomponent.o http/component/staticresourcecomponent.o util/properties.o \
	http/component/inputvalidationcomponent.o http/content/httpcontent.o http/content/httpfilecontent.o \
	http/content/httpstringcontent.o util/filereadercache.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(APPLICATION) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm -f $(APPLICATION)