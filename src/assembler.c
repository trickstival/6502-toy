#include <stdio.h>
#include <stdlib.h>

char * readSource (char *file) {
  FILE *fp = fopen(file, "rb");
  if (!fp) {
    printf("Cannot read file %s", file);
    return NULL;
  }

  // move to the end, get size and go back to the beginning of the file
  fseek(fp, 0, SEEK_END);
  long filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buffer = (char *) malloc(filesize + 1);
  if (!buffer) {
    perror("Get outta here");
    fclose(fp);
    return NULL;
  }

  fread(buffer, sizeof(char), filesize, fp);
  buffer[filesize] = '\0';

  fclose(fp);

  return buffer;
}

int main (int argc, char *argv[]) {
  char *source = readSource(argv[1]);

  printf("%s\n", source);
  free(source);
  return 0;
}
