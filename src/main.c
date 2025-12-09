/*
 * main.c - Entry point for mac-free
 *
 * Part of mac-free: A 'free' command replacement for macOS
 * License: MIT
 */

#include "display.h"
#include "memory.h"
#include "utils.h"

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * ============================================================================
 * Global Variables
 * ============================================================================
 */

static volatile int g_running = 1;

/*
 * ============================================================================
 * Signal Handlers
 * ============================================================================
 */

static void signal_handler(int sig)
{
    (void)sig; /* Unused */
    g_running = 0;
}

static void setup_signals(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

/*
 * ============================================================================
 * Argument Parsing
 * ============================================================================
 */

static int parse_args(int argc, char *argv[], options_t *opts)
{
    static struct option long_options[] = {
        {"bytes", no_argument, NULL, 'b'},
        {"kibi", no_argument, NULL, 'k'},
        {"mebi", no_argument, NULL, 'm'},
        {"gibi", no_argument, NULL, 'g'},
        {"human", no_argument, NULL, 'h'},
        {"wide", no_argument, NULL, 'w'},
        {"total", no_argument, NULL, 't'},
        {"seconds", required_argument, NULL, 's'},
        {"count", required_argument, NULL, 'c'},
        {"lohi", no_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'H'},
        {"version", no_argument, NULL, 'V'},
        {NULL, 0, NULL, 0}};

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "bkmghwts:c:lV", long_options,
                              &option_index)) != -1)
    {
        switch (opt)
        {
            case 'b':
                opts->unit = UNIT_BYTES;
                break;
            case 'k':
                opts->unit = UNIT_KIBI;
                break;
            case 'm':
                opts->unit = UNIT_MEBI;
                break;
            case 'g':
                opts->unit = UNIT_GIBI;
                break;
            case 'h':
                opts->unit = UNIT_HUMAN;
                break;
            case 'w':
                opts->wide = 1;
                break;
            case 't':
                opts->totals = 1;
                break;
            case 's':
                opts->seconds = atoi(optarg);
                if (opts->seconds < 1)
                {
                    fprintf(stderr, "Error: Invalid seconds value: %s\n",
                            optarg);
                    return -1;
                }
                break;
            case 'c':
                opts->count = atoi(optarg);
                if (opts->count < 1)
                {
                    fprintf(stderr, "Error: Invalid count value: %s\n", optarg);
                    return -1;
                }
                break;
            case 'l':
                opts->lohi = 1;
                break;
            case 'H':
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
            case 'V':
                print_version();
                exit(EXIT_SUCCESS);
            case '?':
            default:
                print_usage(argv[0]);
                return -1;
        }
    }

    return 0;
}

/*
 * ============================================================================
 * Main Function
 * ============================================================================
 */

int main(int argc, char *argv[])
{
    options_t       opts;
    system_memory_t sys_mem;
    int             iterations = 0;

    /* Initialize options with defaults */
    init_options(&opts);

    /* Parse command line arguments */
    if (parse_args(argc, argv, &opts) != 0)
    {
        return EXIT_FAILURE;
    }

    /* Set up signal handlers for clean exit */
    setup_signals();

    /* Main display loop */
    do
    {
        /* Get current memory information */
        if (get_system_memory(&sys_mem) != 0)
        {
            fprintf(stderr, "Error: Failed to retrieve memory information\n");
            return EXIT_FAILURE;
        }

        /* Display memory information */
        print_memory_info(&sys_mem, &opts);

        iterations++;

        /* Check if we should continue looping */
        if (opts.seconds > 0 && g_running)
        {
            /* Check count limit */
            if (opts.count > 0 && iterations >= opts.count)
            {
                break;
            }

            /* Sleep and print separator before next iteration */
            sleep(opts.seconds);

            if (g_running)
            {
                printf("\n");
            }
        }

    } while (opts.seconds > 0 && g_running &&
             (opts.count == 0 || iterations < opts.count));

    return EXIT_SUCCESS;
}
