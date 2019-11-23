#include <stdio.h>
#include <assert.h>

int main() {
  printf("begin\n");
  FILE *fp = fopen("/share/texts/num", "r+");
  printf("dd\n");
  assert(fp);
  printf("after assertfp\n");
  fseek(fp, 0, SEEK_END);
  printf("after fseek\n");
  long size = ftell(fp);
  printf("size is %d\n",size);
  assert(size == 5000);
  printf("after assert1\n");
  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  printf("aab\n");
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  printf("bb\n");
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  printf("ee\n");
  /*for (i = 500; i < 1000; i ++) {
    printf("i is %d\n",i);
    fscanf(fp, "%d", &n);
    printf("n is %d\n",n);
    assert(n == i + 1);
  }*/
  printf("ff\n");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    printf("i is %d,n is %d\n",i,n);
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }
  printf("gg\n");
  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
