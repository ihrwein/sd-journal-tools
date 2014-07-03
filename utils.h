#ifndef UTILS_H
#define UTILS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define KEY_LENGTH 256
#define VALUE_LENGTH 2048

void
split_data(const char* data, size_t data_length, char* key, char* value)
{
  int i, j;

  for (i = 0; data[i] != '=' && i < data_length; i++)
    key[i] = data[i];

  key[i++] = '\0';

  for (j = i; j < data_length; j++)
    value[j - i] = data[j];

  value[j - i] = '\0';
}

void
lookup_catalog(sd_journal* journal)
{
  char *data;
  int ret;

  ret = sd_journal_get_catalog(journal, &data);

  if (ret == 0)
    {
      printf("%s\n", data);
      free(data);
    }
  else if (ret == (-1* ENOENT))
    {
      // no catalog entry
    }
}

void
print_fields(sd_journal *j)
{
  const void *data;
  size_t length;

  char key[KEY_LENGTH];
  char value[VALUE_LENGTH];

  lookup_catalog(j);

  SD_JOURNAL_FOREACH_DATA(j, data, length)
  {
    if (length < KEY_LENGTH + VALUE_LENGTH)
      {
        split_data(data, length, key, value);
        printf("%s %s\n", key, value);
      }
    else
      {
        printf("%.*s\n", (int) length, (char*) data);
      }
  }
}


int
print_err(char* errmsg, int errcode)
{
  if (errcode < 0)
    {
      fprintf(stderr, errmsg, strerror(-errcode));
      return 1;
    }
  return 0;
}

#endif
