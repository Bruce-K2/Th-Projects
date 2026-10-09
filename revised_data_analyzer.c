/**
 * @file data_analyzer.c
 * @brief Analyze byte data, character frequencies, encoding indicators, and entropy.
 */

#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE ((size_t)1048576)
#define INITIAL_CAPACITY ((size_t)4096)
#define HEX_DUMP_LIMIT ((size_t)256)

typedef struct {
    size_t total_length;
    size_t printable_chars;
    size_t non_printable_chars;
    size_t alpha_chars;
    size_t digit_chars;
    size_t whitespace_chars;
    size_t punctuation_chars;
    size_t special_chars;
    size_t extended_ascii_chars;
    size_t utf8_multibyte_chars;
    double entropy;
    char encoding_type[40];
} DataStatistics;

typedef struct {
    size_t frequency[256];
    unsigned int most_common_char;
    size_t most_common_count;
} CharFrequency;

static DataStatistics analyze_data(const uint8_t *data, size_t length,
                                   CharFrequency *frequency);
static void calculate_entropy(DataStatistics *stats,
                              const CharFrequency *frequency);
static void detect_encoding(DataStatistics *stats, const uint8_t *data,
                            size_t length, bool has_null, bool valid_utf8);
static bool validate_utf8(const uint8_t *data, size_t length,
                          size_t *multibyte_count);
static size_t get_utf8_char_length(uint8_t first_byte);
static bool is_ascii_whitespace(uint8_t byte);
static bool is_ascii_punctuation(uint8_t byte);
static uint8_t *read_input_data(size_t *length, const char *filename,
                                bool *truncated);
static void display_statistics(const DataStatistics *stats);
static void display_character_frequency(const CharFrequency *frequency,
                                        bool detailed);
static void display_hex_dump(const uint8_t *data, size_t length,
                             size_t max_bytes);
static double percentage(size_t count, size_t total);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           ADVANCED DATA ANALYZER v1.1.0                      ║\n");
    printf("║     Character Detection & Length Analysis System             ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");

    if (argc == 1) {
        printf("📝 Enter data to analyze (Ctrl+D to finish on Unix, Ctrl+Z on Windows):\n");
        printf("────────────────────────────────────────────────────────────────\n");
    }

    size_t data_length = 0;
    bool truncated = false;
    const char *filename = argc == 2 ? argv[1] : NULL;
    uint8_t *data = read_input_data(&data_length, filename, &truncated);
    if (!data) {
        return EXIT_FAILURE;
    }

    if (filename) {
        printf("📁 Analyzing file: %s\n\n", filename);
    } else {
        printf("\n");
    }
    if (truncated) {
        fprintf(stderr, "Warning: input exceeds the %zu-byte limit; analyzing the first %zu bytes.\n",
                MAX_INPUT_SIZE, MAX_INPUT_SIZE);
    }

    printf("🔍 Performing comprehensive data analysis...\n\n");

    CharFrequency frequency = {0};
    DataStatistics stats = analyze_data(data, data_length, &frequency);
    display_statistics(&stats);

    printf("\n");
    display_character_frequency(&frequency, data_length < HEX_DUMP_LIMIT);

    if (data_length <= HEX_DUMP_LIMIT) {
        printf("\n");
        display_hex_dump(data, data_length, HEX_DUMP_LIMIT);
    }

    free(data);
    printf("\n✅ Analysis complete.\n");
    return EXIT_SUCCESS;
}

static DataStatistics analyze_data(const uint8_t *data, size_t length,
                                   CharFrequency *frequency) {
    DataStatistics stats = {0};
    bool has_null = false;

    if (frequency) {
        memset(frequency, 0, sizeof(*frequency));
    }
    if (!data && length != 0) {
        (void)snprintf(stats.encoding_type, sizeof(stats.encoding_type),
                       "Invalid input");
        return stats;
    }
    if (length == 0) {
        (void)snprintf(stats.encoding_type, sizeof(stats.encoding_type),
                       "Empty");
        return stats;
    }

    stats.total_length = length;

    for (size_t i = 0; i < length; ++i) {
        const uint8_t byte = data[i];

        if (frequency) {
            ++frequency->frequency[byte];
        }
        if (byte == 0) {
            has_null = true;
        }

        if (is_ascii_whitespace(byte)) {
            ++stats.whitespace_chars;
            ++stats.printable_chars;
        } else if (byte >= 0x21 && byte <= 0x7E) {
            ++stats.printable_chars;
            if ((byte >= 'A' && byte <= 'Z') ||
                (byte >= 'a' && byte <= 'z')) {
                ++stats.alpha_chars;
            } else if (byte >= '0' && byte <= '9') {
                ++stats.digit_chars;
            } else if (is_ascii_punctuation(byte)) {
                ++stats.punctuation_chars;
            }
        } else if (byte >= 0x80) {
            ++stats.extended_ascii_chars;
        } else {
            ++stats.non_printable_chars;
            ++stats.special_chars;
        }
    }

    if (frequency) {
        for (unsigned int byte = 0; byte < 256; ++byte) {
            if (frequency->frequency[byte] > frequency->most_common_count) {
                frequency->most_common_count = frequency->frequency[byte];
                frequency->most_common_char = byte;
            }
        }
        calculate_entropy(&stats, frequency);
    }

    const bool valid_utf8 = validate_utf8(
        data, length, &stats.utf8_multibyte_chars);
    detect_encoding(&stats, data, length, has_null, valid_utf8);
    return stats;
}

static void calculate_entropy(DataStatistics *stats,
                              const CharFrequency *frequency) {
    if (!stats || !frequency || stats->total_length == 0) {
        return;
    }

    double entropy = 0.0;
    for (size_t i = 0; i < 256; ++i) {
        const size_t count = frequency->frequency[i];
        if (count != 0) {
            const double probability =
                (double)count / (double)stats->total_length;
            entropy -= probability * log2(probability);
        }
    }
    stats->entropy = entropy;
}

static void detect_encoding(DataStatistics *stats, const uint8_t *data,
                            size_t length, bool has_null, bool valid_utf8) {
    const char *encoding;

    if (!stats || !data || length == 0) {
        return;
    }

    if (length >= 3 && data[0] == 0xEF && data[1] == 0xBB &&
        data[2] == 0xBF) {
        encoding = valid_utf8 ? "UTF-8 (BOM)" : "UTF-8 BOM (invalid data)";
    } else if (length >= 2 && data[0] == 0xFF && data[1] == 0xFE) {
        encoding = "UTF-16 LE (BOM)";
    } else if (length >= 2 && data[0] == 0xFE && data[1] == 0xFF) {
        encoding = "UTF-16 BE (BOM)";
    } else if (has_null) {
        encoding = "Binary (contains NUL)";
    } else if (valid_utf8 && stats->extended_ascii_chars != 0) {
        encoding = "UTF-8 (no BOM)";
    } else if (valid_utf8) {
        encoding = "ASCII";
    } else {
        encoding = "Extended ASCII/Latin-1 (heuristic)";
    }

    (void)snprintf(stats->encoding_type, sizeof(stats->encoding_type), "%s",
                   encoding);
}

static bool validate_utf8(const uint8_t *data, size_t length,
                          size_t *multibyte_count) {
    size_t count = 0;
    bool valid = true;
    if (multibyte_count) {
        *multibyte_count = 0;
    }
    if (!data && length != 0) {
        return false;
    }

    for (size_t i = 0; i < length;) {
        const uint8_t first = data[i];
        const size_t char_length = get_utf8_char_length(first);
        if (char_length == 0) {
            valid = false;
            ++i;
            continue;
        }
        if (char_length == 1) {
            ++i;
            continue;
        }
        if (char_length > length - i) {
            valid = false;
            ++i;
            continue;
        }

        bool sequence_valid = true;
        const uint8_t second = data[i + 1];
        if (!((second & 0xC0) == 0x80)) {
            valid = false;
            ++i;
            continue;
        }
        if ((first == 0xE0 && second < 0xA0) ||
            (first == 0xED && second > 0x9F) ||
            (first == 0xF0 && second < 0x90) ||
            (first == 0xF4 && second > 0x8F)) {
            valid = false;
            ++i;
            continue;
        }
        for (size_t j = 2; j < char_length; ++j) {
            if ((data[i + j] & 0xC0) != 0x80) {
                sequence_valid = false;
                break;
            }
        }
        if (!sequence_valid) {
            valid = false;
            ++i;
            continue;
        }

        ++count;
        i += char_length;
    }

    if (multibyte_count) {
        *multibyte_count = count;
    }
    return valid;
}

static size_t get_utf8_char_length(uint8_t first_byte) {
    if (first_byte <= 0x7F) {
        return 1;
    }
    if (first_byte >= 0xC2 && first_byte <= 0xDF) {
        return 2;
    }
    if (first_byte >= 0xE0 && first_byte <= 0xEF) {
        return 3;
    }
    if (first_byte >= 0xF0 && first_byte <= 0xF4) {
        return 4;
    }
    return 0;
}

static bool is_ascii_whitespace(uint8_t byte) {
    return byte == ' ' || byte == '\t' || byte == '\n' ||
           byte == '\v' || byte == '\f' || byte == '\r';
}

static bool is_ascii_punctuation(uint8_t byte) {
    return (byte >= 0x21 && byte <= 0x2F) ||
           (byte >= 0x3A && byte <= 0x40) ||
           (byte >= 0x5B && byte <= 0x60) ||
           (byte >= 0x7B && byte <= 0x7E);
}

static uint8_t *read_input_data(size_t *length, const char *filename,
                                bool *truncated) {
    FILE *input = stdin;
    uint8_t *buffer;
    size_t capacity = INITIAL_CAPACITY;
    size_t total_read = 0;
    bool close_input = filename != NULL;

    if (!length || !truncated) {
        fprintf(stderr, "Error: invalid input-reader arguments.\n");
        return NULL;
    }
    *length = 0;
    *truncated = false;

    if (filename) {
        input = fopen(filename, "rb");
        if (!input) {
            fprintf(stderr, "Error: cannot open '%s': ", filename);
            perror("fopen");
            return NULL;
        }
    }

    if (capacity > MAX_INPUT_SIZE) {
        capacity = MAX_INPUT_SIZE;
    }
    buffer = (uint8_t *)malloc(capacity);
    if (!buffer) {
        perror("Error: allocating input buffer");
        if (close_input) {
            (void)fclose(input);
        }
        return NULL;
    }

    while (total_read < MAX_INPUT_SIZE) {
        if (total_read == capacity) {
            const size_t new_capacity =
                capacity > MAX_INPUT_SIZE / 2 ? MAX_INPUT_SIZE : capacity * 2;
            uint8_t *new_buffer = (uint8_t *)realloc(buffer, new_capacity);
            if (!new_buffer) {
                perror("Error: growing input buffer");
                free(buffer);
                if (close_input) {
                    (void)fclose(input);
                }
                return NULL;
            }
            buffer = new_buffer;
            capacity = new_capacity;
        }

        const size_t bytes_read =
            fread(buffer + total_read, 1, capacity - total_read, input);
        total_read += bytes_read;

        if (bytes_read == 0) {
            if (ferror(input)) {
                perror("Error: reading input");
                free(buffer);
                if (close_input) {
                    (void)fclose(input);
                }
                return NULL;
            }
            break;
        }

        if (ferror(input)) {
            perror("Error: reading input");
            free(buffer);
            if (close_input) {
                (void)fclose(input);
            }
            return NULL;
        }
    }

    if (total_read == MAX_INPUT_SIZE) {
        const int extra_byte = fgetc(input);
        if (extra_byte != EOF) {
            *truncated = true;
        } else if (ferror(input)) {
            perror("Error: checking input limit");
            free(buffer);
            if (close_input) {
                (void)fclose(input);
            }
            return NULL;
        }
    }

    if (close_input && fclose(input) != 0) {
        perror("Error: closing input file");
        free(buffer);
        return NULL;
    }

    *length = total_read;
    return buffer;
}

static double percentage(size_t count, size_t total) {
    return total == 0 ? 0.0 : (double)count * 100.0 / (double)total;
}

static void display_statistics(const DataStatistics *stats) {
    if (!stats) {
        return;
    }

    printf("📈 COMPREHENSIVE DATA STATISTICS\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("📏 Total Length:           %zu bytes\n", stats->total_length);
    printf("🔤 Encoding Detected:      %s\n", stats->encoding_type);
    printf("📊 Shannon Entropy:        %.4f bits/byte\n", stats->entropy);
    printf("\n");
    printf("📋 CHARACTER BREAKDOWN (byte counts)\n");
    printf("────────────────────────────────────────────────────────────────\n");
    printf("✅ Printable ASCII:        %8zu (%6.2f%%)\n",
           stats->printable_chars,
           percentage(stats->printable_chars, stats->total_length));
    printf("   ├─ Alphabetic:          %8zu (%6.2f%%)\n",
           stats->alpha_chars, percentage(stats->alpha_chars, stats->total_length));
    printf("   ├─ Numeric:             %8zu (%6.2f%%)\n",
           stats->digit_chars, percentage(stats->digit_chars, stats->total_length));
    printf("   ├─ Punctuation:         %8zu (%6.2f%%)\n",
           stats->punctuation_chars,
           percentage(stats->punctuation_chars, stats->total_length));
    printf("   └─ Whitespace:          %8zu (%6.2f%%)\n",
           stats->whitespace_chars,
           percentage(stats->whitespace_chars, stats->total_length));
    printf("\n");
    printf("⚠️  Non-Printable ASCII:    %8zu (%6.2f%%)\n",
           stats->non_printable_chars,
           percentage(stats->non_printable_chars, stats->total_length));
    printf("🔣 Extended bytes (80-FF): %8zu (%6.2f%%)\n",
           stats->extended_ascii_chars,
           percentage(stats->extended_ascii_chars, stats->total_length));
    printf("🌐 UTF-8 multibyte:        %8zu valid sequences\n",
           stats->utf8_multibyte_chars);
    printf("\n💡 ENTROPY INTERPRETATION\n");
    printf("────────────────────────────────────────────────────────────────\n");

    if (stats->entropy < 1.0) {
        printf("   Low entropy - Highly repetitive or structured data\n");
    } else if (stats->entropy < 3.0) {
        printf("   Medium entropy - Natural language or structured data\n");
    } else if (stats->entropy < 6.0) {
        printf("   High entropy - Mixed content or compressed data\n");
    } else {
        printf("   Very high entropy - Encrypted or random-looking data\n");
    }
}

static void display_character_frequency(const CharFrequency *frequency,
                                        bool detailed) {
    if (!frequency) {
        return;
    }

    printf("📊 CHARACTER FREQUENCY ANALYSIS\n");
    printf("────────────────────────────────────────────────────────────────\n");

    if (frequency->most_common_count != 0) {
        const unsigned int byte = frequency->most_common_char;
        const char display_char = byte >= 0x20 && byte <= 0x7E
                                      ? (char)byte
                                      : '.';
        printf("Most common byte: '%c' (0x%02X) - appears %zu times\n",
               display_char, byte, frequency->most_common_count);
    } else {
        printf("No bytes to report.\n");
    }

    if (!detailed) {
        return;
    }

    printf("\nDetailed frequency breakdown:\n");
    printf("%-8s %-8s %-10s %s\n", "Char", "Hex", "Count", "Bar");
    printf("────────────────────────────────────────────────────────────────\n");

    for (unsigned int byte = 0; byte < 256; ++byte) {
        const size_t count = frequency->frequency[byte];
        if (count != 0) {
            const char display_char =
                byte >= 0x20 && byte <= 0x7E ? (char)byte : '.';
            const size_t bar_length =
                frequency->most_common_count == 0
                    ? 0
                    : count * 40 / frequency->most_common_count;
            printf("'%c'      0x%02X    %-10zu ", display_char, byte, count);
            for (size_t j = 0; j < bar_length; ++j) {
                putchar('#');
            }
            putchar('\n');
        }
    }
}

static void display_hex_dump(const uint8_t *data, size_t length,
                             size_t max_bytes) {
    if (!data || length == 0) {
        return;
    }

    const size_t display_length = length < max_bytes ? length : max_bytes;
    printf("🔍 HEX DUMP (first %zu bytes)\n", display_length);
    printf("────────────────────────────────────────────────────────────────\n");
    printf("%-8s  %-47s  %s\n", "Offset", "Hex Values", "ASCII");
    printf("────────────────────────────────────────────────────────────────\n");

    for (size_t i = 0; i < display_length; i += 16) {
        printf("%08zX  ", i);
        for (size_t j = 0; j < 16; ++j) {
            if (j < display_length - i) {
                printf("%02X ", data[i + j]);
            } else {
                printf("   ");
            }
            if (j == 7) {
                putchar(' ');
            }
        }

        printf(" |");
        for (size_t j = 0; j < 16 && j < display_length - i; ++j) {
            const uint8_t byte = data[i + j];
            putchar(byte >= 0x20 && byte <= 0x7E ? (int)byte : '.');
        }
        printf("|\n");
    }

    if (length > max_bytes) {
        printf("... (%zu more bytes not shown)\n", length - max_bytes);
    }
}
