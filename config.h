#include "cf.h"
#include <curses.h>

/**
 * Version Information
 */
static const char* VERSION = "0.0.1";

/**
 * Template file directory
 */
static const char* HOME_DIR = "/home/corona";
static const char* TEMPLATES_DIR = "Templates";

/**
 * Invalid Characters
 */
static const char* INVALID_CHARS = "/\\!?()[]{}*@#$%& ><~`"; 

/**
 * Window Config
 */
static CF_Integer PADDING_TOP;
static CF_Integer PADDING_BOTTOM;
static CF_Integer PADDING_LEFT;
static CF_Integer PADDING_RIGHT;
static CF_Integer MARGIN_INTERVAL;


/**
 * Color Config
 */
static CF_Integer COLOR_BG = COLOR_BLACK;
static CF_Integer COLOR_PAIR_SUCCESS = 1;
static CF_Integer COLOR_PAIR_ERROR   = 2;
static CF_Integer COLOR_PAIR_INFO    = 3;
static CF_Integer COLOR_PAIR_WARNING = 4;

/**
 * Error Code
 */
static CF_Integer ERROR_INVALID_CHAR = 401;
