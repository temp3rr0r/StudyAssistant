#include <stdio.h>
#include "Bmp085.h"

int main(void) {
	Bmp085 myBmp;
	printf ("temp: %2.2f", myBmp.getTemp());

	return 0;
}

