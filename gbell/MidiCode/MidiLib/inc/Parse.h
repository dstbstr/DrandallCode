#ifndef __PARSE_H__
#define __PARSE_H__

int parselin(char *line, char parsechr, char *&arg1, char *&arg2);
int preparselin(char *&buf);

#endif // __PARSE_H__