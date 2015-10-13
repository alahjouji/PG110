#ifndef MISC_H_
#define MISC_H_


#include <stdio.h>
#include <stdlib.h>

#include <assert.h>


#include "SDL/SDL_image.h"


// Print an error message on stderr and exit
extern void error(const char *s, ...);

// Load an image, raise an error in case of failure
extern SDL_Surface *load_image(const char *filename);

#endif /* MISC_H_ */
