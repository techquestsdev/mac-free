/*
 * display.c - Output formatting and display implementation
 *
 * Part of mac-free: A 'free' command replacement for macOS
 * License: MIT
 */

#include "display.h"

#include <stdio.h>
#include <string.h>

/*
 * ============================================================================
 * Helper Functions
 * ============================================================================
 */

static void print_value(uint64_t bytes, const options_t *opts)
{
    if (opts->unit == UNIT_HUMAN)
    {
        char buf[32];
        format_human(bytes, buf, sizeof(buf));
        printf(" %11s", buf);
    }
    else
    {
        double val = convert_unit(bytes, opts->unit);
        if (opts->unit == UNIT_BYTES)
        {
            printf(" %12llu", (unsigned long long)bytes);
        }
        else
        {
            printf(" %12.0f", val);
        }
    }
}

/*
 * ============================================================================
 * Header Functions
 * ============================================================================
 */

void print_header(const options_t *opts)
{
    if (opts->unit == UNIT_HUMAN)
    {
        if (opts->wide)
        {
            printf("%-7s %11s %11s %11s %11s %11s %11s %11s %11s\n", "",
                   "total", "used", "free", "active", "inactive", "wired",
                   "compressed", "available");
        }
        else
        {
            printf("%-7s %11s %11s %11s %11s %11s %11s\n", "", "total", "used",
                   "free", "shared", "buff/cache", "available");
        }
    }
    else
    {
        if (opts->wide)
        {
            printf("%-7s %12s %12s %12s %12s %12s %12s %12s %12s\n", "",
                   "total", "used", "free", "active", "inactive", "wired",
                   "compressed", "available");
        }
        else
        {
            printf("%-7s %12s %12s %12s %12s %12s %12s\n", "", "total", "used",
                   "free", "shared", "buff/cache", "available");
        }
    }
}

/*
 * ============================================================================
 * Display Functions
 * ============================================================================
 */

void print_numeric(const mem_info_t *mem, const swap_info_t *swap,
                   const options_t *opts)
{
    /* Print memory row */
    printf("%-7s", "Mem:");
    print_value(mem->total, opts);
    print_value(mem->used, opts);
    print_value(mem->free, opts);

    if (opts->wide)
    {
        print_value(mem->active, opts);
        print_value(mem->inactive, opts);
        print_value(mem->wired, opts);
        print_value(mem->compressed, opts);
    }
    else
    {
        /* "shared" - using compressed as approximation */
        print_value(mem->compressed, opts);
        /* "buff/cache" */
        print_value(mem->cached + mem->inactive, opts);
    }
    print_value(mem->available, opts);
    printf("\n");

    /* Print swap row */
    printf("%-7s", "Swap:");
    print_value(swap->total, opts);
    print_value(swap->used, opts);
    print_value(swap->free, opts);

    if (opts->wide)
    {
        /* Pad remaining columns for alignment */
        int num_cols = 5;
        for (int i = 0; i < num_cols; i++)
        {
            if (opts->unit == UNIT_HUMAN)
            {
                printf(" %11s", "");
            }
            else
            {
                printf(" %12s", "");
            }
        }
    }
    else
    {
        /* Pad remaining columns */
        int num_cols = 3;
        for (int i = 0; i < num_cols; i++)
        {
            if (opts->unit == UNIT_HUMAN)
            {
                printf(" %11s", "");
            }
            else
            {
                printf(" %12s", "");
            }
        }
    }
    printf("\n");
}

void print_human(const mem_info_t *mem, const swap_info_t *swap,
                 const options_t *opts)
{
    /* Human format uses same function, just with UNIT_HUMAN */
    print_numeric(mem, swap, opts);
}

void print_totals(const mem_info_t *mem, const swap_info_t *swap,
                  const options_t *opts)
{
    uint64_t total_mem  = mem->total + swap->total;
    uint64_t total_used = mem->used + swap->used;
    uint64_t total_free = mem->free + swap->free;

    printf("%-7s", "Total:");
    print_value(total_mem, opts);
    print_value(total_used, opts);
    print_value(total_free, opts);
    printf("\n");
}

void print_separator(const options_t *opts)
{
    int width;
    if (opts->wide)
    {
        width = opts->unit == UNIT_HUMAN ? 97 : 115;
    }
    else
    {
        width = opts->unit == UNIT_HUMAN ? 67 : 79;
    }

    for (int i = 0; i < width; i++)
    {
        putchar('-');
    }
    putchar('\n');
}

void print_memory_info(const system_memory_t *sys_mem, const options_t *opts)
{
    print_header(opts);

    if (opts->unit == UNIT_HUMAN)
    {
        print_human(&sys_mem->mem, &sys_mem->swap, opts);
    }
    else
    {
        print_numeric(&sys_mem->mem, &sys_mem->swap, opts);
    }

    if (opts->totals)
    {
        print_totals(&sys_mem->mem, &sys_mem->swap, opts);
    }
}
