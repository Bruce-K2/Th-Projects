/**
 * @file data_analyzer.c
 * @brief Advanced data analysis tool for character and length detection
 * @author System Architect
 * @version 1.0.0
 * 
 * This program provides comprehensive data analysis capabilities including:
 * - Character frequency analysis
 * - String/data length detection
 * - Encoding detection
 * - Statistical analysis
 * 
 * Designed with modularity and extensibility in mind for AI-assisted enhancements.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// ============================================================================
// CONFIGURATION AND CONSTANTS
// ============================================================================

#define MAX_INPUT_SIZE 1048576  // 1MB maximum input
#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 126
#define EXTENDED_ASCII_START 128
#define EXTENDED_ASCII_END 255

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * @brief Comprehensive statistics for analyzed data
 */
typedef struct {
    size_t total_length;           // Total number of bytes
    size_t printable_chars;        // Count of printable characters
    size_t non_printable_chars;    // Count of non-printable characters
    size_t alpha_chars;            // Alphabetic characters
    size_t digit_chars;            // Numeric characters
    size_t whitespace_chars;       // Whitespace characters
    size_t punctuation_chars;      // Punctuation marks
    size_t special_chars;          // Special/control characters
    size_t extended_ascii_chars;   // Extended ASCII (128-255)
    size_t utf8_multibyte_chars;   // UTF-8 multibyte sequences detected
    double entropy;                // Shannon entropy of the data
    char encoding_type[32];        // Detected encoding type
} DataStatistics;

/**
 * @brief Character frequency tracking structure
 */
typedef struct {
    uint32_t frequency[256];       // Frequency for each byte value
    uint32_t most_common_char;     // Most frequent character
    uint32_t most_common_count;    // Count of most frequent character
} CharFrequency;

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

// Core analysis functions
DataStatistics analyze_data(const uint8_t *data, size_t length);
void calculate_entropy(DataStatistics *stats, const uint8_t *data, size_t length);
void detect_encoding(DataStatistics *stats, const uint8_t *data, size_t length);

// Character analysis functions
CharFrequency analyze_character_frequency(const uint8_t *data, size_t length);
void display_character_frequency(const CharFrequency *freq, bool detailed);

// Input/Output functions
uint8_t* read_input_data(size_t *length, const char *filename);
void display_statistics(const DataStatistics *stats);
void display_hex_dump(const uint8_t *data, size_t length, size_t max_bytes);

// Utility functions
bool is_utf8_continuation(uint8_t byte);
size_t get_utf8_char_length(uint8_t first_byte);
const char* get_char_category(uint8_t byte);

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(int argc, char *argv[]) {
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           ADVANCED DATA ANALYZER v1.0.0                      ║\n");
    printf("║     Character Detection & Length Analysis System             ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    uint8_t *data = NULL;
    size_t data_length = 0;
    
    // Read input based on arguments or stdin
    if (argc > 1) {
        // Read from file
        data = read_input_data(&data_length, argv[1]);
        if (!data) {
            fprintf(stderr, "Error: Failed to read input file '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
        printf("📁 Analyzing file: %s\n\n", argv[1]);
    } else {
        // Read from stdin
        printf("📝 Enter data to analyze (Ctrl+D to finish on Unix, Ctrl+Z on Windows):\n");
        printf("────────────────────────────────────────────────────────────────\n");
        data = read_input_data(&data_length, NULL);
        if (!data) {
            fprintf(stderr, "Error: Failed to read input\n");
            return EXIT_FAILURE;
        }
        printf("\n");
    }
    
    // Perform analysis
    printf("🔍 Performing comprehensive data analysis...\n\n");
    
    DataStatistics stats = analyze_data(data, data_length);
    CharFrequency freq = analyze_character_frequency(data, data_length);
    
    // Display results
    display_statistics(&stats);
    
    printf("\n");
    display_character_frequency(&freq, data_length < 256);
    
    // Show hex dump for small inputs
    if (data_length <= 256) {
        printf("\n");
        display_hex_dump(data, data_length, 256);
    }
    
    // Cleanup
    free(data);
    
    printf("\n✅ Analysis complete.\n");
    return EXIT_SUCCESS;
}

// ============================================================================
// CORE ANALYSIS FUNCTIONS
// ============================================================================

/**
 * @brief Performs comprehensive statistical analysis on input data
 * @param data Pointer to the data buffer
 * @param length Length of the data buffer
 * @return DataStatistics structure containing all computed metrics
 */
DataStatistics analyze_data(const uint8_t *data, size_t length) {
    DataStatistics stats = {0};
    
    if (!data || length == 0) {
        return stats;
    }
    
    stats.total_length = length;
    
    // Analyze each byte
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        
        // Categorize the character
        if (byte >= ASCII_PRINTABLE_START && byte <= ASCII_PRINTABLE_END) {
            stats.printable_chars++;
            
            if (isalpha(byte)) {
                stats.alpha_chars++;
            } else if (isdigit(byte)) {
                stats.digit_chars++;
            } else if (ispunct(byte)) {
                stats.punctuation_chars++;
            }
        } else if (byte == ' ' || byte == '\t' || byte == '\n' || byte == '\r') {
            stats.whitespace_chars++;
            stats.printable_chars++;
        } else if (byte >= EXTENDED_ASCII_START) {
            stats.extended_ascii_chars++;
        } else {
            stats.non_printable_chars++;
            stats.special_chars++;
        }
    }
    
    // Detect UTF-8 multibyte sequences
    for (size_t i = 0; i < length; i++) {
        if ((data[i] & 0x80) != 0) {  // High bit set
            size_t char_len = get_utf8_char_length(data[i]);
            if (char_len > 1 && i + char_len <= length) {
                bool valid = true;
                for (size_t j = 1; j < char_len; j++) {
                    if (!is_utf8_continuation(data[i + j])) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    stats.utf8_multibyte_chars++;
                    i += char_len - 1;
                }
            }
        }
    }
    
    // Calculate entropy and detect encoding
    calculate_entropy(&stats, data, length);
    detect_encoding(&stats, data, length);
    
    return stats;
}

/**
 * @brief Calculates Shannon entropy of the data
 * @param stats Pointer to statistics structure to update
 * @param data Pointer to data buffer
 * @param length Length of data
 */
void calculate_entropy(DataStatistics *stats, const uint8_t *data, size_t length) {
    if (!stats || !data || length == 0) return;
    
    uint32_t frequency[256] = {0};
    
    // Count byte frequencies
    for (size_t i = 0; i < length; i++) {
        frequency[data[i]]++;
    }
    
    // Calculate Shannon entropy
    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            double p = (double)frequency[i] / length;
            entropy -= p * log2(p);
        }
    }
    
    stats->entropy = entropy;
}

/**
 * @brief Attempts to detect the encoding type of the data
 * @param stats Pointer to statistics structure to update
 * @param data Pointer to data buffer
 * @param length Length of data
 */
void detect_encoding(DataStatistics *stats, const uint8_t *data, size_t length) {
    if (!stats || !data || length == 0) return;
    
    // Check for BOM markers
    if (length >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
        strcpy(stats->encoding_type, "UTF-8 (BOM)");
        return;
    }
    if (length >= 2 && data[0] == 0xFF && data[1] == 0xFE) {
        strcpy(stats->encoding_type, "UTF-16 LE");
        return;
    }
    if (length >= 2 && data[0] == 0xFE && data[1] == 0xFF) {
        strcpy(stats->encoding_type, "UTF-16 BE");
        return;
    }
    
    // Check for null bytes (likely binary)
    bool has_null = false;
    for (size_t i = 0; i < length && i < 1024; i++) {
        if (data[i] == 0) {
            has_null = true;
            break;
        }
    }
    
    if (has_null) {
        strcpy(stats->encoding_type, "Binary");
        return;
    }
    
    // Check if mostly ASCII
    if (stats->extended_ascii_chars == 0 && stats->utf8_multibyte_chars == 0) {
        strcpy(stats->encoding_type, "ASCII");
    } else if (stats->utf8_multibyte_chars > 0) {
        strcpy(stats->encoding_type, "UTF-8 (no BOM)");
    } else {
        strcpy(stats->encoding_type, "Extended ASCII/Latin-1");
    }
}

// ============================================================================
// CHARACTER ANALYSIS FUNCTIONS
// ============================================================================

/**
 * @brief Analyzes character frequency distribution
 * @param data Pointer to data buffer
 * @param length Length of data
 * @return CharFrequency structure with frequency data
 */
CharFrequency analyze_character_frequency(const uint8_t *data, size_t length) {
    CharFrequency freq = {0};
    
    if (!data || length == 0) return freq;
    
    // Count frequencies
    for (size_t i = 0; i < length; i++) {
        freq.frequency[data[i]]++;
    }
    
    // Find most common character
    for (int i = 0; i < 256; i++) {
        if (freq.frequency[i] > freq.most_common_count) {
            freq.most_common_count = freq.frequency[i];
            freq.most_common_char = i;
        }
    }
    
    return freq;
}

/**
 * @brief Displays character frequency analysis
 * @param freq Pointer to frequency structure
 * @param detailed Whether to show detailed per-character breakdown
 */
void display_character_frequency(const CharFrequency *freq, bool detailed) {
    if (!freq) return;
    
    printf("📊 CHARACTER FREQUENCY ANALYSIS\n");
    printf("────────────────────────────────────────────────────────────────\n");
    
    if (freq->most_common_count > 0) {
        char display_char = (freq->most_common_char >= 32 && freq->most_common_char <= 126) 
                           ? (char)freq->most_common_char : '?';
        printf("Most common character: '%c' (0x%02X) - appears %u times\n",
               display_char, freq->most_common_char, freq->most_common_count);
    }
    
    if (detailed) {
        printf("\nDetailed frequency breakdown:\n");
        printf("%-8s %-8s %-10s %s\n", "Char", "Hex", "Count", "Bar");
        printf("────────────────────────────────────────────────────────────────\n");
        
        for (int i = 0; i < 256; i++) {
            if (freq->frequency[i] > 0) {
                char display_char = (i >= 32 && i <= 126) ? (char)i : '.';
                printf("'%c'      0x%02X    %-10u ", display_char, i, freq->frequency[i]);
                
                // Simple bar graph (max 40 chars)
                int bar_length = (int)((double)freq->frequency[i] / freq->most_common_count * 40);
                for (int j = 0; j < bar_length; j++) {
                    printf("█");
                }
                printf("\n");
            }
        }
    }
}

// ============================================================================
// INPUT/OUTPUT FUNCTIONS
// ============================================================================

/**
 * @brief Reads input data from file or stdin
 * @param length Pointer to store the length of read data
 * @param filename Filename to read from, or NULL for stdin
 * @return Pointer to allocated buffer containing data, or NULL on error
 */
uint8_t* read_input_data(size_t *length, const char *filename) {
    FILE *input = stdin;
    
    if (filename) {
        input = fopen(filename, "rb");
        if (!input) return NULL;
    }
    
    // Allocate initial buffer
    size_t capacity = 4096;
    uint8_t *buffer = malloc(capacity);
    if (!buffer) {
        if (filename) fclose(input);
        return NULL;
    }
    
    size_t total_read = 0;
    size_t bytes_read;
    
    // Read in chunks
    while ((bytes_read = fread(buffer + total_read, 1, capacity - total_read, input)) > 0) {
        total_read += bytes_read;
        
        // Check if we need to grow the buffer
        if (total_read >= capacity) {
            if (capacity >= MAX_INPUT_SIZE) {
                fprintf(stderr, "Warning: Input truncated at %d bytes\n", MAX_INPUT_SIZE);
                break;
            }
            
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) {
                capacity = MAX_INPUT_SIZE;
            }
            
            uint8_t *new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                if (filename) fclose(input);
                return NULL;
            }
            buffer = new_buffer;
        }
    }
    
    if (filename) fclose(input);
    
    *length = total_read;
    return buffer;
}

/**
 * @brief Displays comprehensive statistics
 * @param stats Pointer to statistics structure
 */
void display_statistics(const DataStatistics *stats) {
    if (!stats) return;
    
    printf("📈 COMPREHENSIVE DATA STATISTICS\n");
    printf("════════════════════════════════════════════════════════════════\n");
    
    printf("📏 Total Length:           %zu bytes\n", stats->total_length);
    printf("🔤 Encoding Detected:      %s\n", stats->encoding_type);
    printf("📊 Shannon Entropy:        %.4f bits/byte\n", stats->entropy);
    printf("\n");
    
    printf("📋 CHARACTER BREAKDOWN\n");
    printf("────────────────────────────────────────────────────────────────\n");
    
    double total = (double)stats->total_length;
    
    printf("✅ Printable Characters:   %8zu (%.2f%%)\n", 
           stats->printable_chars, 
           stats->total_length ? (stats->printable_chars / total * 100) : 0);
    
    printf("   ├─ Alphabetic:          %8zu (%.2f%%)\n", 
           stats->alpha_chars,
           stats->total_length ? (stats->alpha_chars / total * 100) : 0);
    
    printf("   ├─ Numeric:             %8zu (%.2f%%)\n", 
           stats->digit_chars,
           stats->total_length ? (stats->digit_chars / total * 100) : 0);
    
    printf("   ├─ Punctuation:         %8zu (%.2f%%)\n", 
           stats->punctuation_chars,
           stats->total_length ? (stats->punctuation_chars / total * 100) : 0);
    
    printf("   └─ Whitespace:          %8zu (%.2f%%)\n", 
           stats->whitespace_chars,
           stats->total_length ? (stats->whitespace_chars / total * 100) : 0);
    
    printf("\n");
    printf("⚠️  Non-Printable:          %8zu (%.2f%%)\n", 
           stats->non_printable_chars,
           stats->total_length ? (stats->non_printable_chars / total * 100) : 0);
    
    printf("🔣 Extended ASCII:         %8zu (%.2f%%)\n", 
           stats->extended_ascii_chars,
           stats->total_length ? (stats->extended_ascii_chars / total * 100) : 0);
    
    printf("🌐 UTF-8 Multibyte:        %8zu sequences\n", 
           stats->utf8_multibyte_chars);
    
    // Entropy interpretation
    printf("\n💡 ENTROPY INTERPRETATION\n");
    printf("────────────────────────────────────────────────────────────────\n");
    
    if (stats->entropy < 1.0) {
        printf("   Low entropy - Highly repetitive or structured data\n");
    } else if (stats->entropy < 3.0) {
        printf("   Medium entropy - Natural language or structured text\n");
    } else if (stats->entropy < 6.0) {
        printf("   High entropy - Mixed content or compressed data\n");
    } else {
        printf("   Very high entropy - Encrypted or random data\n");
    }
}

/**
 * @brief Displays a hex dump of the data
 * @param data Pointer to data
 * @param length Length of data
 * @param max_bytes Maximum bytes to display
 */
void display_hex_dump(const uint8_t *data, size_t length, size_t max_bytes) {
    if (!data || length == 0) return;
    
    size_t display_length = (length < max_bytes) ? length : max_bytes;
    
    printf("🔍 HEX DUMP (first %zu bytes)\n", display_length);
    printf("────────────────────────────────────────────────────────────────\n");
    printf("%-8s  %-47s  %s\n", "Offset", "Hex Values", "ASCII");
    printf("────────────────────────────────────────────────────────────────\n");
    
    for (size_t i = 0; i < display_length; i += 16) {
        // Offset
        printf("%08zX  ", i);
        
        // Hex values
        for (size_t j = 0; j < 16; j++) {
            if (i + j < display_length) {
                printf("%02X ", data[i + j]);
            } else {
                printf("   ");
            }
            if (j == 7) printf(" ");
        }
        
        printf(" |");
        
        // ASCII representation
        for (size_t j = 0; j < 16 && i + j < display_length; j++) {
            uint8_t byte = data[i + j];
            printf("%c", (byte >= 32 && byte <= 126) ? byte : '.');
        }
        
        printf("|\n");
    }
    
    if (length > max_bytes) {
        printf("... (%zu more bytes not shown)\n", length - max_bytes);
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Checks if a byte is a UTF-8 continuation byte
 * @param byte The byte to check
 * @return true if it's a continuation byte (10xxxxxx pattern)
 */
bool is_utf8_continuation(uint8_t byte) {
    return (byte & 0xC0) == 0x80;
}

/**
 * @brief Determines the expected length of a UTF-8 character from its first byte
 * @param first_byte The first byte of a UTF-8 sequence
 * @return Expected character length (1-4), or 1 if invalid
 */
size_t get_utf8_char_length(uint8_t first_byte) {
    if ((first_byte & 0x80) == 0) return 1;       // 0xxxxxxx - ASCII
    if ((first_byte & 0xE0) == 0xC0) return 2;    // 110xxxxx - 2-byte
    if ((first_byte & 0xF0) == 0xE0) return 3;    // 1110xxxx - 3-byte
    if ((first_byte & 0xF8) == 0xF0) return 4;    // 11110xxx - 4-byte
    return 1;  // Invalid UTF-8
}

/**
 * @brief Gets a human-readable category for a character
 * @param byte The byte to categorize
 * @return String describing the character category
 */
const char* get_char_category(uint8_t byte) {
    if (byte >= 32 && byte <= 126) {
        if (isalpha(byte)) return "Alphabetic";
        if (isdigit(byte)) return "Digit";
        if (ispunct(byte)) return "Punctuation";
        if (isspace(byte)) return "Whitespace";
        return "Printable";
    }
    
    switch (byte) {
        case 0:  return "Null";
        case 7:  return "Bell";
        case 8:  return "Backspace";
        case 9:  return "Tab";
        case 10: return "Line Feed";
        case 13: return "Carriage Return";
        case 27: return "Escape";
        default:
            if (byte < 32) return "Control";
            return "Extended ASCII";
    }
}