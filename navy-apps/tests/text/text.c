#include <stdio.h>
#include <assert.h>

int main() {
  FILE *fp = fopen("/share/texts/num", "r+");
  assert(fp);
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  printf("size is %d\n",size);
  assert(size == 5000);
  fseek(fp, 5*500, SEEK_SET);//5*500
  int i, n;
  for (i = 500; i < 1000; i ++) {
printf("currt pos is %d\n",ftell(fp));
    fscanf(fp, "%d", &n);//没读一次前进5
    assert(n == i + 1);
  }
  //此时n=1000
  fseek(fp, 0, SEEK_SET);//指向了文件开头
  printf("now pos is %d\n",ftell(fp));
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  //fseek(fp,0,SEEK_END);printf("after insert size is %d\n",ftell(fp));
  //此时文件头部应写入了1001-1500，n为1000,移动到2500,文件为1001-1500,501-1000
  /*for (i = 500; i < 1000; i ++) {
    printf("i is %d\n",i);
    printf("curr pos is %d\n",ftell(fp));
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }*/
  fseek(fp, 2495, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    printf("n is %d\n",n);
    printf("curr pos is %d\n",ftell(fp));
    //assert(n == i + 1 + 1000);
  }
  printf("gg\n");
  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
