CC       = cc
AR		 = ar
CPPFLAGS += -I slow5lib/include/ -I include/
CFLAGS   += -g -Wall -O2  -std=c99
LDFLAGS  += $(LIBS) -lz -lm -lpthread -lcurl
BUILD_DIR = build
SLOW5LIB  = slow5lib/lib/libslow5.a

ifeq ($(zstd),1)
LDFLAGS		+= -lzstd
endif

BINARY = slow5curl
LIBRARY = libslow5curl.a

OBJ_BIN = $(BUILD_DIR)/main.o \
			$(BUILD_DIR)/misc.o \
			$(BUILD_DIR)/get.o \

OBJ_LIB = $(BUILD_DIR)/fetch.o \
			$(BUILD_DIR)/index.o \
			$(BUILD_DIR)/s5curl.o \
			$(BUILD_DIR)/curlget.o \

VERSION = `git describe --tags`

# make asan=1 enables address sanitiser
ifdef asan
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif


.PHONY: clean distclean test

$(BINARY): $(OBJ_BIN) $(LIBRARY)
	$(CC) $(CFLAGS) $(OBJ_BIN) $(LIBRARY) $(LDFLAGS) -o $@

$(LIBRARY): $(OBJ_LIB) $(SLOW5LIB)
	cp $(SLOW5LIB) $@
	$(AR) rcs $@ $(OBJ_LIB)

# library
$(BUILD_DIR)/fetch.o: src/fetch.c src/fetch.h include/slow5curl/s5curl.h src/fetch.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/index.o: src/index.c include/slow5curl/s5curl.h slow5lib/src/slow5_idx.h slow5lib/src/slow5_extra.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/s5curl.o: src/s5curl.c include/slow5curl/s5curl.h src/fetch.h slow5lib/src/slow5_idx.h slow5lib/src/slow5_extra.h slow5lib/src/slow5_misc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/curlget.o: src/curlget.c include/slow5curl/s5curl.h src/fetch.h slow5lib/src/slow5_idx.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

# tool
$(BUILD_DIR)/misc.o: src_tool/misc.c src_tool/error.h src_tool/cmd.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/get.o: src_tool/get.c src_tool/thread.h src_tool/misc.h src_tool/cmd.h include/slow5curl/s5curl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/main.o: src_tool/main.c src_tool/error.h src_tool/cmd.h src_tool/misc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

slow5lib/lib/libslow5.a:
	$(MAKE) -C slow5lib zstd=$(zstd) no_simd=$(no_simd) zstd_local=$(zstd_local) lib/libslow5.a

clean:
	rm -rf $(BINARY) $(BUILD_DIR)/*.o
	make -C slow5lib clean

# Delete all gitignored files (but not directories)
distclean: clean
	git clean -f -X
	rm -rf $(BUILD_DIR)/* autom4te.cache

# make test with run a simple test
test: $(BINARY)
	./test/test.sh

# make test with run a simple memory test using valgrind
mem: $(BINARY)
	./test/test.sh mem

examples: $(BINARY)
	./examples/build.sh