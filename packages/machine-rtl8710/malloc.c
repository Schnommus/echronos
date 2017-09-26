#define MALLOC_BUF_SIZE 1024

char malloc_buf[MALLOC_BUF_SIZE];
char *malloc_ptr = &malloc_buf;

void *malloc(int size) {
  void *ret;
  ret = (void*)malloc_ptr;
  malloc_ptr += size;

  if(&malloc_buf + MALLOC_BUF_SIZE > malloc_ptr) {
      return ret;
  } else {
      while(1); //OOM in dumb malloc!
  }
}

void free(void *buf) {
}
