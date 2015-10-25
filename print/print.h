#define error(format, ...) \
        fprintf(stderr, "ERROR(%s:%d): " format, \
                __FILE__, __LINE__, ##__VA_ARGS__)

#define info(format, ...) \
        fprintf(stdout, "INFO(%s:%d): " format, \
                __FILE__, __LINE__, ##__VA_ARGS__)
