/*
 * utils.c - Utility function implementations
 *
 * Part of macos-free: A 'free' command replacement for macOS
 * License: MIT
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

/* ============================================================================
 * Conversion Functions
 * ============================================================================ */

double convert_unit(uint64_t bytes, unit_type_t unit) {
    switch (unit) {
        case UNIT_KIBI:
            return (double)bytes / BYTES_PER_KB;
        case UNIT_MEBI:
            return (double)bytes / BYTES_PER_MB;
        case UNIT_GIBI:
            return (double)bytes / BYTES_PER_GB;
        case UNIT_TEBI:
            return (double)bytes / (BYTES_PER_GB * 1024ULL);
        case UNIT_KILO:
            return (double)bytes / 1000.0;
        case UNIT_MEGA:
            return (double)bytes / 1000000.0;
        case UNIT_GIGA:
            return (double)bytes / 1000000000.0;
        case UNIT_TERA:
            return (double)bytes / 1000000000000.0;
        case UNIT_BYTES:
        case UNIT_HUMAN:
        default:
            return (double)bytes;
    }
}

void format_human(uint64_t bytes, char *buf, size_t bufsize) {
    if (buf == NULL || bufsize == 0) {
        return;
    }

    if (bytes >= BYTES_PER_GB) {
        snprintf(buf, bufsize, "%.1fGi", (double)bytes / BYTES_PER_GB);
    } else if (bytes >= BYTES_PER_MB) {
        snprintf(buf, bufsize, "%.1fMi", (double)bytes / BYTES_PER_MB);
    } else if (bytes >= BYTES_PER_KB) {
        snprintf(buf, bufsize, "%.1fKi", (double)bytes / BYTES_PER_KB);
    } else {
        snprintf(buf, bufsize, "%lluB", (unsigned long long)bytes);
    }
}

const char* get_unit_suffix(unit_type_t unit) {
    switch (unit) {
        case UNIT_BYTES: return "B";
        case UNIT_KIBI:  return "Ki";
        case UNIT_MEBI:  return "Mi";
        case UNIT_GIBI:  return "Gi";
        case UNIT_TEBI:  return "Ti";
        case UNIT_KILO:  return "kB";
        case UNIT_MEGA:  return "MB";
        case UNIT_GIGA:  return "GB";
        case UNIT_TERA:  return "TB";
        case UNIT_HUMAN: return "";
        default:         return "";
    }
}

/* ============================================================================
 * Options Functions
 * ============================================================================ */

void init_options(options_t *opts) {
    if (opts == NULL) {
        return;
    }

    opts->unit    = UNIT_KIBI;
    opts->wide    = 0;
    opts->seconds = 0;
    opts->count   = -1;
    opts->totals  = 0;
    opts->lohi    = 0;
}

/* ============================================================================
 * Help and Version Functions
 * ============================================================================ */

void print_usage(const char *prog_name) {
    const char *name = prog_name ? prog_name : PROGRAM_NAME;

    printf("Usage: %s [options]\n", name);
    printf("\n");
    printf("Display memory usage information (macOS version of 'free')\n");
    printf("\n");
    printf("Options:\n");
    printf("  -b, --bytes         Display output in bytes\n");
    printf("  -k, --kibi          Display output in kibibytes (default)\n");
    printf("  -m, --mebi          Display output in mebibytes\n");
    printf("  -g, --gibi          Display output in gibibytes\n");
    printf("  -h, --human         Display human-readable output\n");
    printf("  -w, --wide          Wide output (show all memory categories)\n");
    printf("  -t, --total         Show total for RAM + swap\n");
    printf("  -s N, --seconds N   Repeat printing every N seconds\n");
    printf("  -c N, --count N     Repeat printing N times, then exit\n");
    printf("      --help          Display this help message\n");
    printf("  -V, --version       Display version information\n");
    printf("\n");
    printf("Memory Categories:\n");
    printf("  total       Total installed memory\n");
    printf("  used        Used memory (active + wired + compressed)\n");
    printf("  free        Unused memory\n");
    printf("  active      Memory currently in use or recently used\n");
    printf("  inactive    Memory marked as not recently used\n");
    printf("  wired       Memory that cannot be paged out\n");
    printf("  compressed  Memory that has been compressed\n");
    printf("  cached      Cached files and purgeable memory\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -h          Human-readable output\n", name);
    printf("  %s -m          Output in mebibytes\n", name);
    printf("  %s -hw         Human-readable, wide format\n", name);
    printf("  %s -s 2        Refresh every 2 seconds\n", name);
    printf("  %s -s 1 -c 5   Refresh 5 times, 1 second apart\n", name);
    printf("\n");
}

void print_version(void) {
    printf("%s %d.%d.%d\n", PROGRAM_NAME,
           VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("A 'free' command replacement for macOS\n");
    printf("\n");
    printf("Copyright (c) 2024\n");
    printf("License: MIT\n");
}
