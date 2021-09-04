// LZ4 trivial example : print Library version number
// by Takayuki Matsuoka


#include <stdio.h>
#include "lz4.h"



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      lz4_version_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	(void)argc; (void)argv;
    printf("Hello World ! LZ4 Library version = %d\n", LZ4_versionNumber());
    return 0;
}
