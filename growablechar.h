typedef struct {
    char *c;
    unsigned int len;
    unsigned int mem;
} GrowableChar;

GrowableChar *GC_new();
char *GC_finish(GrowableChar *gc);
void GC_free(GrowableChar *gc);
int GC_appendf(GrowableChar *gc, const char *format, ...);

