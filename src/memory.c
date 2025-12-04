/*
 * memory.c - Memory information retrieval implementation
 *
 * Part of macos-free: A 'free' command replacement for macOS
 * License: MIT
 */

#include <stdio.h>
#include <string.h>
#include <sys/sysctl.h>
#include <mach/mach_host.h>
#include "memory.h"

/* ============================================================================
 * System Information Functions
 * ============================================================================ */

vm_size_t get_page_size(void) {
    vm_size_t page_size;
    host_page_size(mach_host_self(), &page_size);
    return page_size;
}

uint64_t get_total_memory(void) {
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    uint64_t total_memory = 0;
    size_t length = sizeof(total_memory);

    if (sysctl(mib, 2, &total_memory, &length, NULL, 0) != 0) {
        perror("sysctl HW_MEMSIZE");
        return 0;
    }

    return total_memory;
}

int get_vm_stats(vm_statistics64_data_t *vm_stats) {
    if (vm_stats == NULL) {
        return -1;
    }

    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    kern_return_t kr;

    kr = host_statistics64(mach_host_self(), HOST_VM_INFO64,
                           (host_info64_t)vm_stats, &count);

    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "host_statistics64 failed: %s\n",
                mach_error_string(kr));
        return -1;
    }

    return 0;
}

/* ============================================================================
 * Memory Information Functions
 * ============================================================================ */

int get_memory_info(mem_info_t *mem) {
    if (mem == NULL) {
        return -1;
    }

    memset(mem, 0, sizeof(mem_info_t));

    /* Get total physical memory */
    mem->total = get_total_memory();
    if (mem->total == 0) {
        return -1;
    }

    /* Get VM statistics */
    vm_statistics64_data_t vm_stats;
    if (get_vm_stats(&vm_stats) != 0) {
        return -1;
    }

    vm_size_t page_size = get_page_size();

    /* Calculate memory values in bytes */
    mem->free       = (uint64_t)vm_stats.free_count * page_size;
    mem->active     = (uint64_t)vm_stats.active_count * page_size;
    mem->inactive   = (uint64_t)vm_stats.inactive_count * page_size;
    mem->wired      = (uint64_t)vm_stats.wire_count * page_size;
    mem->compressed = (uint64_t)vm_stats.compressor_page_count * page_size;

    /* Purgeable memory (cached) */
    mem->cached = (uint64_t)vm_stats.purgeable_count * page_size;

    /*
     * Calculate used memory
     * Used = Active + Wired + Compressed
     * (Inactive and cached are considered "available")
     */
    mem->used = mem->active + mem->wired + mem->compressed;

    /*
     * Calculate available memory
     * Available = Free + Inactive + Cached
     * This represents memory that can be reclaimed if needed
     */
    mem->available = mem->free + mem->inactive + mem->cached;

    /*
     * App memory is roughly the active memory minus cached files
     * This is an approximation since macOS doesn't expose this directly
     */
    mem->app_memory = mem->active;

    return 0;
}

int get_swap_info(swap_info_t *swap) {
    if (swap == NULL) {
        return -1;
    }

    memset(swap, 0, sizeof(swap_info_t));

    struct xsw_usage swap_usage;
    size_t length = sizeof(swap_usage);
    int mib[2] = {CTL_VM, VM_SWAPUSAGE};

    if (sysctl(mib, 2, &swap_usage, &length, NULL, 0) != 0) {
        perror("sysctl VM_SWAPUSAGE");
        return -1;
    }

    swap->total = swap_usage.xsu_total;
    swap->used  = swap_usage.xsu_used;
    swap->free  = swap_usage.xsu_avail;

    return 0;
}

int get_system_memory(system_memory_t *sys_mem) {
    if (sys_mem == NULL) {
        return -1;
    }

    memset(sys_mem, 0, sizeof(system_memory_t));

    sys_mem->page_size = get_page_size();

    if (get_memory_info(&sys_mem->mem) != 0) {
        return -1;
    }

    if (get_swap_info(&sys_mem->swap) != 0) {
        /* Swap info failure is non-fatal */
        fprintf(stderr, "Warning: Could not retrieve swap information\n");
    }

    return 0;
}

double calculate_memory_pressure(const mem_info_t *mem) {
    if (mem == NULL || mem->total == 0) {
        return 0.0;
    }

    /*
     * Memory pressure calculation:
     * Based on how much memory is under pressure (used vs available)
     */
    double used_ratio = (double)mem->used / (double)mem->total;
    double compressed_ratio = (double)mem->compressed / (double)mem->total;

    /* Weight compressed memory more heavily as it indicates pressure */
    double pressure = used_ratio + (compressed_ratio * 0.5);

    /* Clamp to 0.0 - 1.0 range */
    if (pressure > 1.0) pressure = 1.0;
    if (pressure < 0.0) pressure = 0.0;

    return pressure;
}
