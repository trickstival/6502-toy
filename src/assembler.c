#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * readSource (char *file, long* filesize) {
  FILE *fp = fopen(file, "rb");
  if (!fp) {
    printf("Cannot read file %s", file);
    return NULL;
  }

  // move to the end, get size and go back to the beginning of the file
  fseek(fp, 0, SEEK_END);
  *filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buffer = (char *) malloc(*filesize + 1);
  if (!buffer) {
    perror("Get outta here");
    fclose(fp);
    return NULL;
  }

  fread(buffer, sizeof(char), *filesize, fp);
  buffer[*filesize] = '\0';

  fclose(fp);

  return buffer;
}

enum EToken {
  INSTRUCTION,
  VALUE
};

struct Token {
  enum EToken type;
  int value;
};

struct Token *runTokenizer (char *source, long filesize) {
  // TODO: allow more tokens
  struct Token* tokens = malloc(sizeof(struct Token) * 200);

  // split
  for (int i = 0; i < filesize; i++) {
    if (source[i] == ' ' || source[i] == '\n') {
      source[i] = '\0';
    }
  }

  int tokenIdx = 0;
  int sourceIdx = 0;
  for (int i = 0; i < filesize; i++) {
    char currentChar = source[i];

    if (currentChar != '\0') {
      continue;
    }

    char *strToken = &source[sourceIdx];

    if (strcmp(strToken, "LDA") == 0) {
      tokens[tokenIdx] = (struct Token){ INSTRUCTION, 0xA9 };
    } else {
      // value
      tokens[tokenIdx] = (struct Token){ VALUE, atoi(strToken) };
    }

    tokenIdx++;
    sourceIdx = i + 1;
  }

  return tokens;
}

int run (int argc, char *argv[]) {
  long *filesize = 0;
  char *source = readSource(argv[1], filesize);

  struct Token *tokens = runTokenizer(source, *filesize);

  printf("%s\n", source);
  free(source);
  return 0;
}
