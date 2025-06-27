#ifndef LOG_H
#define LOG_H

typedef enum e_log_level
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_SUCCESS
} t_log_level;

void log_msg(t_log_level level, const char *fmt, ...);

#endif
