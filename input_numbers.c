#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

  int *mapped;
  int fd;
  int size = 101;

  fd = shm_open("/file1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if(fd == -1) {
    perror("shm_open");
    exit(-1);
  }

  mapped = mmap(0, size, PROT_WRITE, MAP_SHARED, fd, 0);

  if(mapped == MAP_FAILED) {
    perror("mapping");
    close(fd);
    exit(-1);
  }

  if(ftruncate(fd, size) == -1) {
    perror("ftruncate");
    exit(-1);
  }

  printf("Please enter up to 100 integers \n");

  char str[1024];
  int count = 1;
  int numbers;

  while(fgets(str, 1024, stdin)) {
    if(sscanf(str, "%d", &numbers) > 0 && count < size) {
      mapped[count++] = numbers;
    }
  }

  *mapped = count;

  if(msync(mapped, size, MS_SYNC) == -1) {
    perror("msync");
    exit(-1);
  }

  if(munmap(mapped, size) == -1) {
    perror("unmap");
    exit(-1);
  }

  close(fd);
  return 0;
}