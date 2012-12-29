CC = gcc
FLAG = -g -Wall
#FLAG = -Wall
INCLUDEPATH = -I./include

all: libfdict test

fdict_test:
	$(CC) -o test_datrie $(FLAG) $(INCLUDEPATH) ./src/test_datrie.c -L. -lfdict
	$(CC) -o test_fdict $(FLAG) $(INCLUDEPATH) ./src/test_fdict.c -L. -lfdict

test_utf:
	$(CC) -o test_utf $(FLAG) $(INCLUDEPATH) ./src/test_utf8_utf16.c -L. -lfdict

test: clean_o clean_a clean_test libfdict fdict_test

tools: del_tools
	$(CC) -o fdbuild $(FLAG) $(INCLUDEPATH) ./src/fdbuild.c -L. -lfdict
	$(CC) -o fdsearch $(FLAG) $(INCLUDEPATH) ./src/fdsearch.c -L. -lfdict

del_tools:
	rm -rf fdbuild
	rm -rf fdsearch


libfdict: clean_o clean_a
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/util.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/check.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/wordbase.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/libtime.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/dictdatrie.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/wordimage.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/trie.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/libfdict.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/datrie.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/trie_out_text.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/utf.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/ConvertUTF.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/datafileprocess.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/record.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/config.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/json.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/build.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/search.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/fdict.c
	$(CC) -c $(INCLUDEPATH) $(FLAG) ./src/index.c
	ar csr libfdict.a *.o

clean_o:
	rm -rf *.o

clean_a:
	rm -rf *.a

clean_test:
	rm -rf fdict_test
	rm -rf test_utf
	rm -rf test_datrie
	rm -rf test_fdict
	rm -rf *.dSYM

clean_tools:
	rm -rf fdbuild
	rm -rf fdsearch

clean_data:
	rm -rf *.dump
	rm -rf *.datindex

clean: clean_o clean_a clean_test clean_tools clean_data


