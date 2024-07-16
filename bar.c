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
    printf("bar arguments:");
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
    return 1;
  }

  char buf[80];
  int r;

  sleep(1);

  printf("bar reads, next line is what I get\n");
  do {
    r = do_or_die("bar: read", read(fdinp, buf, 80));
    if (r == 0) {
      printf("bar: unexpected EOF\n");
      exit(1);
    }
    printf("%.*s", r, buf);
    fflush(stdout);
  } while (buf[r-1] != '\n');

  printf("bar sends reply 1\n");
  do_or_die("bar: write", write(fdout, "reply 1\n", 8));

  sleep(2);

  printf("bar closes write end and waits for foo to do the same\n");
  close(fdout);
  r = do_or_die("bar: final read", read(fdinp, buf, 80));
  if (r != 0) {
    printf("bar: unexpected data\n");
    exit(1);
  } else {
    printf("bar is satisfied\n");
  }

  return 0;
}
