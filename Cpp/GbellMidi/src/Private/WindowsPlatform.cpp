#include <winsock.h>
#include "Private/WindowsPlatform.h"

#include <share.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>
#include <string>

#if !(defined SSIZE_MAX)
#define SSIZE_MAX (SIZE_MAX >> 1)
#endif

char* index(char* str, char toFind) {
    std::string asString(str);
    auto offset = asString.find_first_of(toFind);
    if(offset == asString.npos) {
        return NULL;
    }
    return str + offset;
}

char* rindex(char* str, char toFind) {
    std::string asString(str);
    auto offset = asString.find_last_of(toFind);
    if(offset == asString.npos) {
        return NULL;
    }
    return str + offset;
}

//stolen from here: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
int getline(char** pline_buf, size_t* pn, FILE * fin) {
  const size_t INITALLOC = 16;
  const size_t ALLOCSTEP = 16;
  size_t num_read = 0;

  if ((NULL == pline_buf) || (NULL == pn) || (NULL == fin))
  {
    errno = EINVAL;
    return -1;
  }

  if (NULL == *pline_buf)
  {
    *pline_buf = (char*)malloc(INITALLOC);
    if (NULL == *pline_buf)
    {
      return -1;
    }
    else
    {
      *pn = INITALLOC;
    }
  }

  {
    int c;
    while (EOF != (c = getc(fin)))
    {
      /* Note we read a character. */
      num_read++;

      /* Reallocate the buffer if we need more room */
      if (num_read >= *pn)
      {
        size_t n_realloc = *pn + ALLOCSTEP;
        void * tmp = realloc(*pline_buf, n_realloc + 1); /* +1 for the trailing NUL. */
        if (NULL != tmp)
        {
          /* Use the new buffer and note the new buffer size. */
          *pline_buf = (char*) tmp;
          *pn = n_realloc;
        }
        else
        {
          /* Exit with error and let the caller free the buffer. */
          return -1;
        }

        /* Test for overflow. */
        if (SSIZE_MAX < *pn)
        {
          errno = ERANGE;
          return -1;
        }
      }

      /* Add the character to the buffer. */
      (*(char**)pline_buf)[num_read - 1] = (char) c;

      /* Break from the loop if we hit the ending character. */
      if (c == '\n')
      {
        break;
      }
    }

    /* Note if we hit EOF. */
    if (EOF == c)
    {
      errno = 0;
      return -1;
    }
  }

  /* Terminate the string by suffixing NUL. */
  (*(char**)pline_buf)[num_read] = '\0';

  return num_read;
}