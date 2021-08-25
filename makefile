COMPILER = gcc
FLAG = std=c11
SLIB = pthread rt
DLIB = $(shell cat ./shared)
TARGET = output

OBJECT_DIR = ./build
SOURCE_DIR = ./source
INCLUDE_DIR = ./include $(addsuffix /include,	\
	      $(shell cat ./shared))
DLIB_DIR = $(addsuffix /build,$(DLIB))

SOURCE = $(wildcard $(SOURCE_DIR)/*.c)
INCLUDE = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECT = $(addprefix $(OBJECT_DIR)/,\
	 $(patsubst %.c,%.o,$(notdir $(SOURCE))))
DOBJ = $(wildcard $(addsuffix /*, $(DLIB_DIR)))
DOBJ_MAIN = $(addsuffix /main.o,\
	    $(wildcard $(DLIB_DIR)))

all: $(TARGET)

debug: FLAG += g
debug: all

$(TARGET): $(OBJECT)
	$(COMPILER) -o $(TARGET) $(OBJECT)	\
	$(filter-out $(DOBJ_MAIN), $(DOBJ))	\
	$(addprefix -l,$(SLIB)) 		\
	

$(OBJECT): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(OBJECT_DIR)
	$(COMPILER) -c $? -o $@			\
	$(addprefix -I,$(INCLUDE_DIR)/) 	\
	$(addprefix -l,$(SLIB)) 		\
	$(addprefix -,$(FLAG))

buildall:
	$(foreach var,$(DLIB), make -C $(var) remake;)
	@make remake

debugall:
	$(foreach var,$(DLIB), make -C $(var) debug;)
	@make debug

clean:
	@rm -rf $(OBJECT_DIR)
	@rm -rf $(TARGET)

cleanall:
	$(foreach var,$(DLIB), make -C $(var) clean;)
	@make clean

remake: clean all

.PHONY: clean remake buildall
