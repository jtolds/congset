congset.so: congset.c
	gcc -shared -fPIC -o congset.so congset.c -ldl

clean:
	rm -f *.so

.PHONY: clean
