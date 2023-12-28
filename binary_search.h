#ifndef _BINARY_SEARCH_
#define _BINARY_SEARCH_

#include <stdint.h>

// Returns the index of the element which holds the provided value if found, or
// otherwise the insertion position to preserve ordering.
static inline uint8_t binary_search(const int8_t x, int8_t const* const xs, const uint8_t n)
{
  if(x > xs[n-1]) return n; 

  else if(x <= xs[0]) return 0;

  else {
    uint8_t ret=1;
    uint8_t diff=n-2;
    uint8_t mid;
    uint8_t hdiff;

    while(diff>0) {
      hdiff=diff>>1;
      mid=ret+hdiff;

      if(x>xs[mid]) {
	ret=++mid;
	diff-=hdiff+1;

      } else diff=hdiff;
    }

    return ret;
  }
}

#endif
