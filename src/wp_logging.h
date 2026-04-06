#ifndef WP_LOGGING_H
#define WP_LOGGING_H

#include "wp_constants.h"

#define wp_log_info(format, ...)           fprintf(stdout, WP_NAME ": [info] " format "\n", ##__VA_ARGS__)
#define wp_log_warning(format, ...)        fprintf(stdout, WP_NAME ": \033[33m[warning]\033[0m " format "\n", ##__VA_ARGS__)
#define wp_log_error(format, ...)          fprintf(stderr, WP_NAME ": \033[31m[error]\033[0m " format "\n", ##__VA_ARGS__)
#define wp_log_info_overwrite(format, ...) fprintf(stdout, "\033[1G\033[1K" WP_NAME ": [info] " format, ##__VA_ARGS__)

#endif