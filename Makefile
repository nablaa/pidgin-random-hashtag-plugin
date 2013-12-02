CC = gcc
CFLAGS += -O2 -Werror -Wall -Wextra -Wno-missing-braces \
          -Wno-missing-field-initializers -Wformat=2 -Wswitch-default \
          -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast \
          -Wstrict-overflow=5 -Winline -Wundef -Wnested-externs -Wcast-qual \
          -Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal \
          -Wstrict-aliasing=2 -Wredundant-decls \
          -Wold-style-definition -Wno-unused-parameter -std=c99

PIDGIN_CFLAGS += `pkg-config --cflags pidgin`
PIDGIN_LIBS += `pkg-config --libs pidgin`

PLUGIN_DIR = ~/.purple/plugins/

random_hashtag.so: random_hashtag.o
	$(CC) $(LDFLAGS) -shared $(CFLAGS) $? -o $@ $(PIDGIN_LIBS)

random_hashtag.o: random_hashtag.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(PIDGIN_CFLAGS)

clean:
	rm -f random_hashtag.so
	rm -f *.o

install: random_hashtag.so
	@echo "Installing random_hashtag.so"
	mkdir -p $(PLUGIN_DIR)
	cp random_hashtag.so $(PLUGIN_DIR)
	@echo "Plugin installed to ~/.purple/plugins/"
