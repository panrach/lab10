#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int do_or_die(const char *em, int ret)
{
  if (ret == -1) {
    perror(em);
    exit(1);
  }
  return ret;
}

int main(int argc, char **argv)
{
  int fdinp, fdout;

  if (0) {  // change 0 to 1 to print out arguments so you can double-check
    printf("foo arguments:");
    for (int i = 1; i < argc; i++) {
      printf(" %s", argv[i]);
    }
    printf("\n");
  }
  if (argc < 3
      || sscanf(argv[1], "%d", &fdinp) != 1
      || sscanf(argv[2], "%d", &fdout) != 1)
  {
    fprintf(stderr, "foo: invalid argument\n");
    exit(1);
  }

  char buf[80];
  int r;

  printf("foo sends msg 1\n");
  do_or_die("foo: write", write(fdout, "msg 1\n", 6));

  sleep(2);

  printf("foo reads, next line is what I get\n");
  do {
    r = do_or_die("foo: read", read(fdinp, buf, 80));
    if (r == 0) {
      printf("foo: unexpected EOF\n");
      exit(1);
    }
    printf("%.*s", r, buf);
    fflush(stdout);
  } while (buf[r-1] != '\n');

  printf("foo closes write end and waits for bar to do the same\n");
  close(fdout);
  r = do_or_die("foo: final read", read(fdinp, buf, 80));
  if (r != 0) {
    printf("foo: unexpected data\n");
    exit(1);
  } else {
    printf("foo is satisfied\n");
  }
  close(fdinp);

  return 0;
}
