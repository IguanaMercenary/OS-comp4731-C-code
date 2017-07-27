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
  int size = (int) sizeof(int);

  fd = shm_open("/file1", O_RDONLY, S_IRUSR | S_IWUSR);

  if(fd == -1) {
    perror("shm_open");
    exit(-1);
  }

  mapped = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

  if(mapped == MAP_FAILED) {
    perror("mapping");
    close(fd);
    exit(-1);
  }

  for(int i = 1; i < *mapped; i++) {
    printf("%d\n", mapped[i]);
  }

  if(shm_unlink("/file1") == -1) {
    perror("shm_unlink");
    exit(-1);
  }
  if(munmap(mapped, size) == -1) {
    perror("un-mapping");
    exit(-1);
  }

  close(fd);
  return 0;
}

