CUR_DIR				= $(shell pwd)
MACHINE				= $(shell uname -m)
ARCH				= $(MACHINE)

ifeq ("i686", "$(MACHINE)")
ARCH				= i386
endif

ifeq ("amd64", "$(MACHINE)")
ARCH				= x86_64
endif

PLATFORM  			= $(shell uname -s | tr '[:upper:]' '[:lower:]')_$(ARCH)
OPLATFORM  			= $(shell uname -s | tr '[:upper:]' '[:lower:]')

CLIENT				= bin/client_$(PLATFORM)
SERVER				= bin/server_$(PLATFORM)

DEBUG_SERVER		= bin/debug_server_$(PLATFORM)

COMMON_TARGETS		= $(CLIENT)

#FreeBSD = freebsd
ifeq ("freebsd", "$(OPLATFORM)")
TARGETS			= $(COMMON_TARGETS)
endif

ifeq ("linux_i386", "$(PLATFORM)")
TARGETS			= $(COMMON_TARGETS)
endif

#Linux = linux
ifeq ("linux_x86_64", "$(PLATFORM)")
TARGETS			= $(SERVER) $(COMMON_TARGETS) $(DEBUG_SERVER)
endif

INCLUDES			= -I${CUR_DIR}/inc
VPATH     			= ${CUR_DIR}/src
SOURCES				= ${CUR_DIR}/src

CPP					= g++
DEBUG			    = -DDEBUG -g
PREPROCESSORS		=
OPTIMIZE			= -O2
LIBS      			= -lpthread -lrt

CPPFLAGS			= -Wall $(PREPROCESSORS) -c $(INCLUDES) $(OPTIMIZE)
LDFLAGS 			=  $(LIBS)

OBJ_SUFFIX			= obj

COMMON_SOURCES		= ${SOURCES}/log.cpp

CLIENT_SOURCES		= $(SOURCES)/client.cpp

SERVER_SOURCES		= $(COMMON_SOURCES) \
						$(SOURCES)/server.cpp \
						$(SOURCES)/queue.cpp \
						$(SOURCES)/main.cpp

SERVER_SOURCES_BARE	= $(notdir $(SERVER_SOURCES))
CLIENT_SOURCES_BARE	= $(notdir $(CLIENT_SOURCES))

SERVER_OBJECTS 		= ${SERVER_SOURCES_BARE:%.cpp=.server_objects_$(PLATFORM)/%.$(OBJ_SUFFIX)}
DSERVER_OBJECTS 	= ${SERVER_SOURCES_BARE:%.cpp=.dserver_objects_$(PLATFORM)/%.$(OBJ_SUFFIX)}
CLIENT_OBJECTS 		= ${CLIENT_SOURCES_BARE:%.cpp=.client_objects_$(PLATFORM)/%.$(OBJ_SUFFIX)}

all: $(TARGETS)

.dserver_objects_$(PLATFORM)/%.$(OBJ_SUFFIX): %.cpp
	@mkdir -p $(dir $@)
	@echo compile phase: $<
	${CPP} ${DEBUG} ${CPPFLAGS} ${PSQL_FLAGS} -o $@ $<

.server_objects_$(PLATFORM)/%.$(OBJ_SUFFIX): %.cpp
	@mkdir -p $(dir $@)
	@echo compile phase: $<
	${CPP} ${CPPFLAGS} ${PSQL_FLAGS} -o $@ $<

.client_objects_$(PLATFORM)/%.$(OBJ_SUFFIX): %.cpp
	@mkdir -p $(dir $@)
	@echo compile phase: $<
	${CPP} ${CPPFLAGS} -o $@ $<

${SERVER}: ${SERVER_OBJECTS}
	@echo $@ link phase ...
	$(CPP) -o $@ $(SERVER_OBJECTS) $(LDFLAGS)

${CLIENT}: ${CLIENT_OBJECTS}
	@echo $@ link phase ...
	@$(CPP) -o $@ $(CLIENT_OBJECTS) $(LDFLAGS)

${DEBUG_SERVER}: ${DSERVER_OBJECTS}
	@echo $@ link phase ...
	@$(CPP) $(DEBUG) -o $@ $(DSERVER_OBJECTS) $(LDFLAGS)

clean:
	@echo clean up ...
	rm -f $(TARGETS) .server_objects_$(PLATFORM)/* .dserver_objects_$(PLATFORM)/* .sserver_objects_$(PLATFORM)/* .client_objects_$(PLATFORM)/*
