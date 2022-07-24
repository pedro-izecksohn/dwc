#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct KeyValue
{
  char *key;
  unsigned int value;
} KeyValue;

typedef struct Vector
{
  size_t size;
  KeyValue **array;
} Vector;

KeyValue *newKeyValue (char *key)
{
  KeyValue *ret = malloc (sizeof (KeyValue)); 
  ret->key = malloc (strlen (key)+1);
  strcpy (ret->key, key);
  ret->value = 1;
  return ret;          
}

char *newString (void)
{
  char *ret = malloc (1);
  ret[0] = 0;
  return ret;
}

char *stringApend (char *str, const char c)
{
  const size_t size = strlen (str);
  str = realloc (str, size+2);
  if (str==NULL)
  {
    printf ("stringApend: realloc para %lu falhou.",size+2);
    exit (EXIT_FAILURE);
  }
  str[size] = c;
  str[size+1] = 0;
  return str;
}

void vectorApend (Vector *vector,KeyValue *kv)
{
  vector->array = realloc (vector->array, (vector->size+1) * sizeof (KeyValue*));
  vector->array[vector->size] = kv;
  vector->size++;
}

Vector *newVector (void)
{
  Vector *ret = malloc (sizeof (Vector));
  ret->size = 0;
  ret->array = malloc (0);
  return ret;

}
KeyValue *searchStringInVector (Vector *vector, char *str)
{
  for (size_t i = 0;i<vector->size;i++)
  {
    KeyValue *kv = vector->array[i];
    if (strcmp (kv->key, str)==0)
    {
      return kv;
    }      
  }
  return NULL;
}

int compareKeyValues (const void *p,const void *q)
{
  const KeyValue *const*ppkv1 = p;
  const KeyValue *const*ppkv2 = q;
  const KeyValue *pkv1 = *ppkv1; 
  const KeyValue *pkv2 = *ppkv2;
  if (pkv1->value>pkv2->value)
  {
    return 1;
  }
  else if (pkv2->value>pkv1->value)
  {
    return -1;
  }
  return strcmp (pkv1->key, pkv2->key);
}
		
int main()
{
  char *word = newString();
  char c = 0;
  Vector *vector = newVector();
  while (1)
  {
    c = getchar();
    if (ferror (stdin))
    {
      printf ("Error in stdin.\n");
      return EXIT_FAILURE;
    }
    else if ((' ' == c) ||
    ('\t' == c) ||
    ('\n' == c) ||
    ('\r' == c) ||
    ('\\' == c) ||
    ('/' == c) ||
    ('|' == c) ||
    ('!' == c) ||
    ('?' == c) ||
    ('.' == c) ||
    (',' == c) ||
    (':' == c) ||
    (';' == c) ||
    ('~' == c) ||
    ('^' == c) ||
    ('`' == c) ||
    ('*' == c) ||
    ('+' == c) ||
    ('(' == c) ||
    (')' == c) ||
    ('[' == c) ||
    (']' == c) ||
    ('{' == c) ||
    ('}' == c) ||
    ('"' == c) ||
    ('\'' == c) ||
    ('$' == c) ||
    ('#' == c) ||
    (0 == c))   
    {
      if (word[0])
      {
        KeyValue *kv = searchStringInVector (vector,word);
        if (kv)
        {
          kv->value = kv->value+1;
        }
        else
        {
          kv = newKeyValue (word);
          vectorApend (vector,kv);
        }
        free (word);
        word = newString();
      }
      continue;
    }
    else if (c == EOF)
    {
      if (word[0])
      {
        KeyValue *kv = searchStringInVector (vector,word);
        if (kv)
        {
          kv->value = kv->value+1;
        }
        else
        {
          kv = newKeyValue (word);
          vectorApend (vector, kv);
        }
      }
      free (word);
      break;
    }
    else
    {
      word = stringApend (word, tolower(c));
      //printf ("Inseri %c em %s\n", c, word);
    }
  }
  qsort (vector->array, vector->size, sizeof(KeyValue *), compareKeyValues);
  for (unsigned int i = 0; i<vector->size; i++)
  {
    KeyValue *kv = vector->array[i];
    printf ("%s\t%u\n", kv->key, kv->value);
  }
  return EXIT_SUCCESS;
}
