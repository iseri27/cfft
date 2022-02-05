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

static CF_Integer PADDING_TOP;
static CF_Integer PADDING_BOTTOM;
static CF_Integer PADDING_LEFT;
static CF_Integer PADDING_RIGHT;
static CF_Integer MARGIN_INTERVAL;

static CF_Integer COLOR_BG = COLOR_BLACK;

static CF_Integer COLOR_PAIR_SUCCESS = 1;
static CF_Integer COLOR_PAIR_ERROR   = 2;
static CF_Integer COLOR_PAIR_INFO    = 3;
static CF_Integer COLOR_PAIR_WARNING = 4;
