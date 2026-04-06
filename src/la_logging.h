#ifndef LA_LOGGING_H
#define LA_LOGGING_H

#define NAME "linux-audio"

#define log_info(format, ...)           fprintf(stdout, NAME ": [info] " format "\n", ##__VA_ARGS__)
#define log_warning(format, ...)        fprintf(stdout, NAME ": \033[33m[warning]\033[0m " format "\n", ##__VA_ARGS__)
#define log_error(format, ...)          fprintf(stderr, NAME ": \033[31m[error]\033[0m " format "\n", ##__VA_ARGS__)
#define log_info_overwrite(format, ...) fprintf(stdout, "\033[1G\033[1K" NAME ": [info] " format, ##__VA_ARGS__)

#endif