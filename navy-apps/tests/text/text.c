#include <stdio.h>
#include <assert.h>

int main() {
  FILE *fp = fopen("/share/texts/num", "r+");
  //printf("dd\n");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  //printf("cc\n");
  assert(size == 5000);

  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  //printf("aa\n");
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  //printf("bb\n");
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  printf("ee\n");
  for (i = 500; i < 1000; i ++) {
    printf("i is %d\n",i);
    fscanf(fp, "%d", &n);
    printf("n is %d\n",n);
    assert(n == i + 1);
  }
  printf("ff\n");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }
  printf("gg\n");
  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
