#include "tokenize.hpp"
#include <cctype>

TokenSequenceType tokenize(std::istream& seq) {
    TokenSequenceType tokens;
    std::string line;

    while (std::getline(seq, line)) {
        size_t pos = 0;

        while (pos < line.length()) {
            char current = line[pos];

            // Skip leading whitespace
            while (pos < line.length() && (std::isspace(current) != 0 || current == '\n')) {
                pos++;
                current = line[pos];
            }

            if (pos < line.length()) {
                // Check for COMMENT character
                if (current == COMMENT) {
                    break; // Ignore the rest of the line as it's a comment
                }

                if (current == OPEN || current == CLOSE) {
                    // Parentheses are individual tokens
                    tokens.push_back(std::string(1, current));
                    pos++;
                }
                else {
                    // Parse a space-delimited string
                    size_t start = pos;
                    while (pos < line.length() && std::isspace(line[pos]) == 0 && line[pos] != OPEN && line[pos] != CLOSE) {
                        pos++;
                    }
                    tokens.push_back(line.substr(start, pos - start));
                }

                // Skip trailing whitespace
                while (pos < line.length() && (std::isspace(line[pos]) != 0 || line[pos] == '\n')) {
                    pos++;
                }
            }
        }
    }

    return tokens;
}
