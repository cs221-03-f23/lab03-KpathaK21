OBJS = passwords.o lab03.o sha256.o
PROG = lab03

%.o: %.c
	gcc -c -g -o $@ $<

$(PROG): $(OBJS)
	gcc -g -o $@ $^

clean:
	rm -rf $(OBJS) $(PROG) dictionary.txt
