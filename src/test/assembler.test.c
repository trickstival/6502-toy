#include <assert.h>
#include "../assembler.c"

void tokenizes () {
  char val[] = "LDA 10";
  struct Token *tokens = runTokenizer(val, 7);

  assert(tokens[0].type == INSTRUCTION);
  assert(tokens[0].value == 0xA9);
  assert(tokens[1].type == VALUE);
  assert(tokens[1].value == 10);
}

int main () {
  tokenizes();
}
