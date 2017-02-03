#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "../core/secure.h"
#include "test_toolbox.h"

bool lecture() {
    bool r = true;
    
    r = r && test_equality_bool(true, read("test 1", 10), "test 1 : chaine courte");
    r = r && test_equality_bool(true, read("test 2\n", 10), "test 2 : chaine courte avec retour");
    r = r && test_equality_bool(false, read("test 3\n", 3), "test 3 : chaine longue");
    
    return r;
}

int main ()
{
   printf("%d\n", lecture());
   return EXIT_SUCCESS;;
}
