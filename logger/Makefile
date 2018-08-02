UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
CXX   = g++
LD    = g++
else ifeq ($(UNAME_S), Linux)
CXX   = g++
LD    = g++
endif

ifeq ($(DEBUG), 1)
CXXFLAGS = -g -O0 -DDEBUG -std=c++11
else ifeq ($(UNIT_TEST), 1)
CXXFLAGS = -g -O0 -DUNIT_TEST -DDEBUG -std=c++11
else
CXXFLAGS = -O3 -std=c++11
endif

OBJS_DIR = obj
TARGET = logger_test 

DEPEND_FILE = $(OBJS_DIR)/depend_file

SRCS      = $(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard */*/*.cpp)
OBJS      = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:%.cpp=%.o)))
LDFLAGS   = -L/usr/local/lib
LIBS      =
INCLUDES  = 

.SUFFIXES : .cpp .o

#-------------------------------------------------------------------------------
# Make Rules
#-------------------------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo -e "=\033[0;36m Creating \033[0;0m  $@"
	@$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
	@ctags `find . -name '*.h' -or -name '*.cpp' -or -name '*.hpp'`

$(OBJS):
	@echo -e "=\033[0;32m Compiling \033[0;0m $<"
	@`[ -d $(OBJS_DIR) ] || mkdir $(OBJS_DIR)`
	@$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

depend:
	@`[ -d $(OBJS_DIR) ] || mkdir $(OBJS_DIR)`
	@rm -f $(DEPEND_FILE)
	@for FILE in $(SRCS:%.cpp=%); do \
		$(CXX) -MM -MT $(OBJS_DIR)/`basename $$FILE.o` $$FILE.cpp \
			$(CXXFLAGS) $(DEFINES) $(INCLUDES) >> $(DEPEND_FILE); \
	done

tags: $(SRCS) Makefile
	ctags `find . -name '*.h' -or -name '*.cpp' -or -name '*.hpp'`

clean:
	rm -f $(TARGET)
	rm -rf $(OBJS_DIR)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include $(DEPEND_FILE)
endif
endif
endif

