#include <ctype.h>
#include <string.h>
#include "strhelpers.h"

void to_lowercase(char *string)
{
    int string_length = strlen(string);

    for (int i = 0; i < string_length; i++)
    {
        string[i] = tolower(string[i]);
    }
}
