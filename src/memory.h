/*
 * memory.h - Memory information retrieval for macOS
 *
 * Part of macos-free: A 'free' command replacement for macOS
 * License: MIT
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <mach/mach.h>

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

/* Physical memory information */
typedef struct {
    uint64_t total;         /* Total physical memory */
    uint64_t used;          /* Used memory (active + wired + compressed) */
    uint64_t free;          /* Free memory */
    uint64_t active;        /* Active pages */
    uint64_t inactive;      /* Inactive pages */
    uint64_t wired;         /* Wired (non-pageable) pages */
    uint64_t compressed;    /* Compressed pages */
    uint64_t cached;        /* Cached/purgeable pages */
    uint64_t app_memory;    /* Memory used by applications */
    uint64_t available;     /* Available memory (free + inactive + cached) */
} mem_info_t;

/* Swap/virtual memory information */
typedef struct {
    uint64_t total;         /* Total swap space */
    uint64_t used;          /* Used swap space */
    uint64_t free;          /* Free swap space */
} swap_info_t;

/* Combined system memory information */
typedef struct {
    mem_info_t  mem;        /* Physical memory */
    swap_info_t swap;       /* Swap memory */
    vm_size_t   page_size;  /* System page size */
} system_memory_t;

/* ============================================================================
 * Function Prototypes
 * ============================================================================ */

/**
 * Get system page size
 *
 * @return  Page size in bytes
 */
vm_size_t get_page_size(void);

/**
 * Get total physical memory
 *
 * @return  Total memory in bytes, or 0 on error
 */
uint64_t get_total_memory(void);

/**
 * Get VM statistics from Mach kernel
 *
 * @param vm_stats  Pointer to vm_statistics64_data_t structure
 * @return          0 on success, -1 on error
 */
int get_vm_stats(vm_statistics64_data_t *vm_stats);

/**
 * Get physical memory information
 *
 * @param mem   Pointer to mem_info_t structure to fill
 * @return      0 on success, -1 on error
 */
int get_memory_info(mem_info_t *mem);

/**
 * Get swap memory information
 *
 * @param swap  Pointer to swap_info_t structure to fill
 * @return      0 on success, -1 on error
 */
int get_swap_info(swap_info_t *swap);

/**
 * Get all system memory information
 *
 * @param sys_mem   Pointer to system_memory_t structure to fill
 * @return          0 on success, -1 on error
 */
int get_system_memory(system_memory_t *sys_mem);

/**
 * Calculate memory pressure (0.0 - 1.0)
 *
 * @param mem   Pointer to mem_info_t structure
 * @return      Memory pressure as a ratio (0.0 = no pressure, 1.0 = critical)
 */
double calculate_memory_pressure(const mem_info_t *mem);

#endif /* MEMORY_H */
