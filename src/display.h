/*
 * display.h - Output formatting and display functions
 *
 * Part of mac-free: A 'free' command replacement for macOS
 * License: MIT
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "memory.h"
#include "utils.h"

/* ============================================================================
 * Constants
 * ============================================================================ */

/* Column widths for formatting */
#define COL_WIDTH_LABEL     5
#define COL_WIDTH_VALUE     12
#define COL_WIDTH_HUMAN     10

/* ANSI color codes (optional) */
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"

/* ============================================================================
 * Function Prototypes
 * ============================================================================ */

/**
 * Print memory information header
 *
 * @param opts  Display options
 */
void print_header(const options_t *opts);

/**
 * Print memory information in numeric format
 *
 * @param mem   Physical memory information
 * @param swap  Swap memory information
 * @param opts  Display options
 */
void print_numeric(const mem_info_t *mem, const swap_info_t *swap,
                   const options_t *opts);

/**
 * Print memory information in human-readable format
 *
 * @param mem   Physical memory information
 * @param swap  Swap memory information
 * @param opts  Display options
 */
void print_human(const mem_info_t *mem, const swap_info_t *swap,
                 const options_t *opts);

/**
 * Print all memory information
 *
 * @param sys_mem   System memory information
 * @param opts      Display options
 */
void print_memory_info(const system_memory_t *sys_mem, const options_t *opts);

/**
 * Print a separator line
 *
 * @param opts  Display options (determines width)
 */
void print_separator(const options_t *opts);

/**
 * Print totals row
 *
 * @param mem   Physical memory information
 * @param swap  Swap memory information
 * @param opts  Display options
 */
void print_totals(const mem_info_t *mem, const swap_info_t *swap,
                  const options_t *opts);

#endif /* DISPLAY_H */
