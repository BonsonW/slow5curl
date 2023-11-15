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

ifeq ($(zstd_local),)
else
LDFLAGS		+= zstd/lib/libzstd.a
endif

BINARY = slow5curl
LIBRARY = lib/libslow5curl.a

OBJ_BIN = $(BUILD_DIR)/main.o \
			$(BUILD_DIR)/misc.o \
			$(BUILD_DIR)/get.o \
			$(BUILD_DIR)/head.o \
			$(BUILD_DIR)/reads.o \

OBJ_LIB = $(BUILD_DIR)/fetch.o \
			$(BUILD_DIR)/index.o \
			$(BUILD_DIR)/s5curl.o \
			$(BUILD_DIR)/mt.o \

VERSION = `git describe --tags`

# make asan=1 enables address sanitiser
ifdef asan
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif


.PHONY: tool library clean distclean test

tool: $(BINARY)

$(BINARY): $(OBJ_BIN) $(LIBRARY)
	$(CC) $(CFLAGS) $(OBJ_BIN) $(LIBRARY) $(LDFLAGS) -o $@

library: $(LIBRARY)

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

$(BUILD_DIR)/mt.o: src/mt.c include/slow5curl/s5curl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

# tool
$(BUILD_DIR)/misc.o: src_tool/misc.c src_tool/error.h src_tool/cmd.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/get.o: src_tool/get.c src_tool/misc.h src_tool/cmd.h include/slow5curl/s5curl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/head.o: src_tool/head.c src_tool/misc.h src_tool/cmd.h include/slow5curl/s5curl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/reads.o: src_tool/reads.c src_tool/misc.h src_tool/cmd.h include/slow5curl/s5curl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

$(BUILD_DIR)/main.o: src_tool/main.c src_tool/error.h src_tool/cmd.h src_tool/misc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

slow5lib/lib/libslow5.a:
	$(MAKE) -C slow5lib slow5_mt=1 zstd=$(zstd) no_simd=$(no_simd) zstd_local=$(zstd_local) lib/libslow5.a

clean:
	rm -rf $(BINARY) $(BUILD_DIR)/*.o
	make -C slow5lib clean

# Delete all gitignored files (but not directories)
distclean: clean
	git clean -f -X
	rm -rf $(BUILD_DIR)/* autom4te.cache

release: distclean
# make the release
	mkdir -p slow5curl-$(VERSION)
	mkdir -p slow5curl-$(VERSION)/docs slow5curl-$(VERSION)/scripts slow5curl-$(VERSION)/slow5lib
	cp -r README.md LICENSE Makefile build lib src src_tool include slow5curl-$(VERSION)
	cp -r docs/* slow5curl-$(VERSION)/docs/
	cp scripts/install-zstd.sh slow5curl-$(VERSION)/scripts/
	cp -r slow5lib/lib slow5lib/include slow5lib/src  slow5lib/Makefile slow5lib/LICENSE slow5lib/thirdparty/ slow5curl-$(VERSION)/slow5lib
	tar -zcf slow5curl-$(VERSION)-release.tar.gz slow5curl-$(VERSION)
	rm -rf slow5curl-$(VERSION)
# make the binaries
	scripts/install-zstd.sh
	make -j8 zstd_local=../zstd/lib
	mkdir -p slow5curl-$(VERSION)
	mkdir slow5curl-$(VERSION)/docs
	mv slow5curl slow5curl-$(VERSION)/
	cp -r README.md LICENSE slow5curl-$(VERSION)/
	cp -r docs/commands.md slow5curl-$(VERSION)/docs/
	tar -zcf slow5curl-$(VERSION)-x86_64-linux-binaries.tar.gz slow5curl-$(VERSION)
	rm -rf slow5curl-$(VERSION)
# test release
	tar xf slow5curl-$(VERSION)-release.tar.gz
	make -C slow5curl-$(VERSION)
	slow5curl-$(VERSION)/slow5curl --version
	rm -rf slow5curl-$(VERSION)
# test binaries
	tar xf slow5curl-$(VERSION)-x86_64-linux-binaries.tar.gz
	mv slow5curl-$(VERSION)/slow5curl slow5curl
	test/test.sh

# make test with run a simple test
test: $(BINARY)
	./test/test.sh

# make test with run a simple memory test using valgrind
mem: $(BINARY)
	./test/test.sh mem

examples: $(BINARY)
	./examples/build.sh
