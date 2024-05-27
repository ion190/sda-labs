#include <stdio.h>
#include <stdlib.h>
#include "version1.h"
#include "version2.h"


int main() {
    int version;
    printf("Type 1 for 1st version with struct pointers, or 2 for 2nd version with union: ");
    scanf("%d", &version); 
    
    if (version == 1) {
        version1();
    } else if (version == 2) {
        version2();
    } else {
        printf("Undefined version");
    }

    return 0;
}