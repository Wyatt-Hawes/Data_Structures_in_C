#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

typedef struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
} Parser;

Parser *parser_create(FILE *f) {

  Parser *p = (Parser *)malloc(sizeof(Parser));
  p->f = f;
  p->line_offset = MAX_PARSER_LINE_LENGTH + 1;

  return p;
}

void parser_delete(Parser **p) {
  fclose((*p)->f);
  free(*p);
  *p = NULL;
  return;
}

static bool is_valid_letter(char l) {

  // printf("Check: %c |%d%d%d\n", l,isalnum(l), (l == '\'') , (l == '-'));
  return (isalnum(l) != 0 || (l == '\'') || (l == '-'));
}

bool next_word(Parser *p, char *word) {

  if (p->line_offset > MAX_PARSER_LINE_LENGTH) {

    if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) {
      return false;
    }
    p->line_offset = 0;
  }
  // printf("%s",p->current_line);

  while (!is_valid_letter(p->current_line[p->line_offset])) {
    // printf("invalid\n");
    if (p->current_line[p->line_offset] == '\n') {
      p->line_offset = MAX_PARSER_LINE_LENGTH + 1;
      return next_word(p, word);
    }

    p->line_offset += 1;
  }

  int i = 0;
  while (is_valid_letter(p->current_line[p->line_offset])) {
    // printf("\n");
    word[i] = tolower(p->current_line[p->line_offset]);
    p->line_offset += 1;
    i++;
  }
  word[i] = 0;
  return true;
}
