# macos-free

A `free` command replacement for macOS that displays memory usage information similar to the Linux `free` command.

## Overview

macOS doesn't include a `free` command like Linux. This utility provides a familiar interface for checking memory usage, using native macOS APIs to gather accurate memory statistics.

## Features

- Display total, used, free, and available memory
- Show swap usage
- Multiple output formats (bytes, KiB, MiB, GiB, human-readable)
- Wide mode showing all macOS memory categories (active, inactive, wired, compressed)
- Continuous monitoring with configurable refresh interval
- Compatible with common `free` command options

## Building

### Requirements

- macOS 10.12 or later
- Xcode Command Line Tools (`xcode-select --install`)

### Compile

```shell
make
```

### Debug Build

```shell
make debug
```

## Installation

```shell
sudo make install
```

This installs the free command to `/usr/local/bin/`. You can specify a different prefix:

```shell
sudo make install PREFIX=/opt/local
```

## Usage

```shell
free [options]
```

### Options

| Option  | Long Form | Description                             |
| ------- | --------- | --------------------------------------- |
| -b      | --bytes   | Display output in bytes                 |
| -k      | --kibi    | Display output in kibibytes (default)   |
| -m      | --mebi    | Display output in mebibytes             |
| -g      | --gibi    | Display output in gibibytes             |
| -h      | --human   | Display output in human-readable format |
| -w      | --wide    | Wide output (show all memory categories)|
| -t      | --total   | Show total for RAM + swap               |
| -s N    | --seconds N | Repeat printing every N seconds       |
| -c N    | --count N | Repeat printing N times, then exit      |
| -V      | --version | Output version information and exit     |
|         | --help    | Display help and exit                   |

### Examples

#### Basic usage

```txt
$ free
              total        used        free      shared  buff/cache   available
Mem:       16777216     8388608     2097152      524288     6291456     8388608
Swap:       2097152      524288     1572864
```

**Human-readable format:**

```shell
$ free -h
               total         used         free       shared   buff/cache    available
Mem:          16.0Gi        8.0Gi        2.0Gi      512.0Mi        6.0Gi        8.0Gi
Swap:          2.0Gi      512.0Mi        1.5Gi
```

**Wide output with all memory categories:**

```shell
$ free -w -h
              total        used        free      active    inactive       wired  compressed   available
Mem:         16.0Gi       8.0Gi       2.0Gi       4.0Gi       2.0Gi       2.0Gi       1.0Gi       8.0Gi
Swap:         2.0Gi     512.0Mi       1.5Gi
```

**Monitor memory every 2 seconds:**

```shell
free -h -s 2
```

**Monitor memory 5 times with 1 second interval:**

```shell
free -h -s 1 -c 5
```

**Show totals:**

```txt
$ free -h -t
              total        used        free      shared  buff/cache   available
Mem:         16.0Gi       8.0Gi       2.0Gi     512.0Mi       6.0Gi       8.0Gi
Swap:         2.0Gi     512.0Mi       1.5Gi
Total:       18.0Gi       8.5Gi       3.5Gi
```

## Memory Categories

### Standard Output

| **Column**     | **Description**                                                     |
| -------------- | ------------------------------------------------------------------- |
| **total**      | Total installed physical memory                                     |
| **used**       | Used memory(Active + Wired + Compressed)                            |
| **free**       | Completely unused memory                                            |
| **shared**     | Memory used by shared mappings                                      |
| **buff/cache** | Memory used for file buffers and cache(mapped to Inactive on macOS) |
| **available**  | Estimate of memory available for starting new applications          |

### Wide Output(macOS - specific)

| **Column**     | **Description**                                  |
| -------------- | ------------------------------------------------ |
| **active**     | Memory currently in use or recently used         |
| **inactive**   | Memory marked as not recently used               |
| **wired**      | Memory that cannot be paged out(kernel, drivers) |
| **compressed** | Memory that has been compressed to save space    |

## How It Works

This utility uses macOS - specific APIs to gather memory information:

- **`host_statistics64()`** - Retrieves VM statistics including page counts for different memory states
- **`sysctl()`** - Gets total physical memory and swap usage
- **`vm_page_size`** - System page size for converting page counts to bytes

### Memory Calculation Notes

- **Used Memory** = Active + Wired + Compressed (speculative pages excluded)
- **Available Memory** = Free + Inactive (approximation, as macOS can reclaim inactive pages)
- **Shared Memory** = Pages marked as purgeable/shared in VM statistics

## Differences from Linux `free`

1. **buff/cache**: On Linux, this shows buffer and cache memory separately. On macOS, we map this to inactive memory, which serves a similar purpose.

2. **shared**: Linux tracks shared memory segments. On macOS, we report purgeable/shared page statistics.

3. **available**: Both systems estimate available memory, but the calculation differs due to different memory management strategies.

4. **Wide mode**: Shows macOS-specific categories (compressed memory is unique to macOS).

## Uninstall

```bash
sudo make uninstall
```

Or manually:

```bash
sudo rm /usr/local/bin/free
```

## Troubleshooting

### "Permission denied" when installing

Use sudo for installation:

```shell
sudo make install
```

### Values don't match Activity Monitor exactly

This is expected.Activity Monitor may use slightly different calculations or update at different intervals.The values should be close but not identical.

### Swap shows 0 total

macOS uses dynamic swap. If no swap file has been created yet, total swap may show as 0. Swap files are created on - demand in `/private/var/vm/`.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

### Building for Development

```shell
# Build with debug symbols
make debug

# Run tests
make test

# Clean build artifacts
make clean
```

## License

MIT License - See [LICENSE](LICENSE) file for details.

---

### Made with ❤️ and C
