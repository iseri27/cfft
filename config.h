#include "cf.h"
#include <curses.h>
#include <string.h>

/**
 * Version Information
 */
static const char* VERSION = "0.0.2";

/**
 * Template file directory
 */
static const char* HOME_DIR = "/home/corona";
static const char* TEMPLATES_DIR = "Templates";

/**
 * Chinese character length
 */
static const CF_Integer ZH_CHAR_LEN = strlen("正");

/**
 * Invalid Characters
 */
static const char* INVALID_CHARS = "/\\!?()[]{}*@#$%& ><~`"; 

/**
 * Window Config
 */
static const CF_Integer MIN_WINDOW_ROW = 300;
static const CF_Integer MIN_WINDOW_COL = 600;

static CF_Integer PADDING_TOP     = 3;
static CF_Integer PADDING_LEFT    = 3;
static CF_Integer PADDING_RIGHT   = 3;
static CF_Integer PADDING_BOTTOM  = 5;
static CF_Integer MARGIN_INTERVAL = 1;


/**
 * Color Config
 */
static CF_Integer COLOR_BACKGROUND   = COLOR_BLACK;
static CF_Integer COLOR_PAIR_SUCCESS = 1;
static CF_Integer COLOR_PAIR_ERROR   = 2;
static CF_Integer COLOR_PAIR_INFO    = 3;
static CF_Integer COLOR_PAIR_WARNING = 4;

/**
 * Error Code
 */
static CF_Integer ERROR_INVALID_CHAR   = 501;
static CF_Integer ERROR_INVALID_CWD    = 502;
static CF_Integer ERROR_INVALID_HEIGHT = 503;
static CF_Integer ERROR_INVALID_WIDTH  = 504;
