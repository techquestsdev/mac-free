/*
 * utils.h - Utility definitions and macros
 *
 * Part of mac-free: A 'free' command replacement for macOS
 * License: MIT
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

/* ============================================================================
 * Constants
 * ============================================================================ */

#define BYTES_PER_KB    1024ULL
#define BYTES_PER_MB    (1024ULL * 1024ULL)
#define BYTES_PER_GB    (1024ULL * 1024ULL * 1024ULL)

#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_PATCH   0

#define PROGRAM_NAME    "free"

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

#include <stdint.h>

/* Output unit types */
typedef enum {
    UNIT_BYTES,
    UNIT_KIBI,      // KiB (1024)
    UNIT_MEBI,      // MiB (1024^2)
    UNIT_GIBI,      // GiB (1024^3)
    UNIT_TEBI,      // TiB (1024^4)
    UNIT_KILO,      // KB (1000)
    UNIT_MEGA,      // MB (1000^2)
    UNIT_GIGA,      // GB (1000^3)
    UNIT_TERA,      // TB (1000^4)
    UNIT_HUMAN      // Auto-select appropriate unit
} unit_type_t;

/* Command-line options */
typedef struct {
    unit_type_t unit;       /* Output unit */
    int         wide;       /* Wide output mode */
    int         seconds;    /* Refresh interval (0 = no refresh) */
    int         count;      /* Number of iterations (-1 = infinite) */
    int         totals;     /* Show totals line */
    int         lohi;       /* Show low/high memory stats */
} options_t;

/* ============================================================================
 * Function Prototypes
 * ============================================================================ */

/**
 * Convert bytes to specified unit
 *
 * @param bytes     Number of bytes to convert
 * @param unit      Target unit type
 * @return          Converted value as double
 */
double convert_unit(uint64_t bytes, unit_type_t unit);

/**
 * Format bytes as human-readable string
 *
 * @param bytes     Number of bytes to format
 * @param buf       Output buffer
 * @param bufsize   Size of output buffer
 */
void format_human(uint64_t bytes, char *buf, size_t bufsize);

/**
 * Get unit suffix string
 *
 * @param unit      Unit type
 * @return          Suffix string (e.g., "Ki", "Mi", "Gi")
 */
const char* get_unit_suffix(unit_type_t unit);

/**
 * Print usage information
 *
 * @param prog_name     Program name (argv[0])
 */
void print_usage(const char *prog_name);

/**
 * Print version information
 */
void print_version(void);

/**
 * Initialize default options
 *
 * @param opts      Pointer to options structure
 */
void init_options(options_t *opts);

#endif /* UTILS_H */
