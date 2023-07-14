CC       = cc
CPPFLAGS += -I slow5lib/include/
CFLAGS   += -g -Wall -O2
LDFLAGS  += $(LIBS) -lz
BUILD_DIR = build

ifeq ($(zstd),1)
LDFLAGS		+= -lzstd
endif

# change the tool name to what you want
BINARY = tool

OBJ = $(BUILD_DIR)/main.o \
# add more objects here
#      $(BUILD_DIR)/subtool1.o \
#      $(BUILD_DIR)/subtool2.o \

ifdef asan
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

.PHONY: clean test

$(BINARY): $(OBJ) slow5lib/lib/libslow5.a
	$(CC) $(CFLAGS) $(OBJ) slow5lib/lib/libslow5.a $(LDFLAGS) -o $@

$(BUILD_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LANGFLAG) $< -c -o $@

# follow the main.o above and add more objects here if needed

slow5lib/lib/libslow5.a:
	$(MAKE) -C slow5lib zstd=$(zstd) no_simd=$(no_simd) zstd_local=$(zstd_local) lib/libslow5.a

clean:
	rm -rf $(BINARY) $(BUILD_DIR)/*.o
	make -C slow5lib clean

test: $(BINARY)
	./test/test.sh

