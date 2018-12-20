#ifndef _LWS_LOG_H_
#define _LWS_LOG_H_

typedef enum {
	LOG_LEVLE_SYS = 1,
	LOG_LEVEL_ERR = 2,
	LOG_LEVEL_WARN = 3,
	LOG_LEVEL_INFO = 4
} log_level_t;

extern void lws_set_log_level(log_level_t level);
extern log_level_t lws_get_log_level(void);

/**
 * @func    lws_logger
 * @brief   output print information interface
 *
 * @param   level[in] debug print level
 * @param   filename[in] debug information filename
 * @param   line[in] debug information line
 * @param   format[in] debug information format
 * @return  print message size.
 */
extern int lws_logger(log_level_t level, const char *filename, int line, const char *format, ...);

#define lws_log(level, ...)   lws_logger(level, __FILE__, __LINE__, ##__VA_ARGS__)
#define lws_err(...)    lws_logger(LOG_LEVEL_ERR, __FILE__, __LINE__, ##__VA_ARGS__)
#define lws_wrn(...)    lws_logger(LOG_LEVEL_WARN, __FILE__, __LINE__, ##__VA_ARGS__)
#define lws_dbg(...)    lws_logger(LOG_LEVEL_INFO, __FILE__, __LINE__, ##__VA_ARGS__)

#endif // _LWS_LOG_H_

