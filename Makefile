all: foo bar bridge
.PHONE: all

bridge: bridge.c
	gcc $< -o $@

foo: foo.c
	gcc $< -o $@

bar: bar.c
	gcc $< -o $@
