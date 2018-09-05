#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>

#include "growablechar.h"

unsigned int smallestPowerOf2(unsigned int val);

GrowableChar *GC_new() {
    GrowableChar *gc;

    gc = malloc(sizeof(GrowableChar));
    if(gc == NULL) {
        return(NULL);
    }

    gc->c = NULL;
    gc->len = 0;
    gc->mem = 0;

    return(gc);
}

char *GC_finish(GrowableChar *gc) {
    char *temp;

    if(gc->len > 0) {
        temp = realloc(gc->c, gc->len);
        if(temp == NULL) {
            return(NULL);
        }
        free(gc);
        return(temp);
    }

    return(NULL);
}

void GC_free(GrowableChar *gc) {
    if(gc->mem > 0) {
        free(gc->c);
    }

    free(gc);
}

int GC_appendf(GrowableChar *gc, const char *format, ...) {
    va_list ap;
    unsigned int size, totalsize, newsize;
    char *temp;

    va_start(ap, format);
    size = vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    totalsize = gc->len + size;
    if(gc->c == NULL) {
        newsize = smallestPowerOf2(totalsize + 1); /* add null terminator */
        gc->c = malloc(newsize);
        if(gc->c == NULL) {
            return(-ENOMEM);
        }
        gc->mem = newsize;
        gc->len = 1; /* no null terminator yet */
    }
    if(totalsize > gc->mem) {
        /* make sure a null terminator is accounted for before first append */
        newsize = smallestPowerOf2(totalsize);
        temp = realloc(gc->c, newsize);
        if(temp == NULL) {
            return(-ENOMEM);
        }
        gc->c = temp;
        gc->mem = newsize;
    }

    va_start(ap, format);
    /* start printing over null terminator */
    size = vsnprintf(&(gc->c[gc->len - 1]),
                     gc->mem - (gc->len - 1),
                     format, ap);
    va_end(ap);

    gc->len += size;         

    return(size);
}

/* probably a better way to do this */
unsigned int smallestPowerOf2(unsigned int val) {
    unsigned int i, val2;

    val2 = 1;
    for(i = 0; i < (sizeof(unsigned int) * 8) - 1; i++) {
        if(val2 > val) {
            return(val2);
        }
        val2 *= 2;
    }

    return(UINT_MAX); /* overflow */
}
