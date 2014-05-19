#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tokenizer.h"

static const int INPUT_SIZE = 2048;
static const int TRUNCATED_INPUT = 1024;
/**
 * Main program execution
 */
int main( int argc, char *argv[] )
{
  TOKENIZER *tokenizer;
  char buffer [INPUT_SIZE] = "";
  char* currentString;
  int bytesRead;

  string[255] = '\0';	   /* ensure that string is always null-terminated */
  printf( "\n\nGive me a string to parse or press ctrl-d to stop:\n" );
  while ((br = read( STDIN_FILENO, string, 255 )) > 0) {
    if (br <= 1)
      continue;
    string[br-1] = '\0';   /* remove trailing \n */
    /* tokenize string */
    printf( "Parsing '%s'\n", string );
    tokenizer = init_tokenizer( string );
    while( (tok = get_next_token( tokenizer )) != NULL ) {
      printf( "Got token '%s'\n", tok );
      free( tok );    /* free the token now that we're done with it */
    }
    free_tokenizer( tokenizer ); /* free memory */
    printf( "\n\nGive me a string to parse or press ctrl-d to stop:\n" );
  }

  printf( "\nBye!\n" );
  return 0;			/* all's well that end's well */
}
