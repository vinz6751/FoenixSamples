/* Ecrit une valeur 32bit non-signée à l'adresse indiquée */
/* Public domain */

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool parse_dec(const char * arg, uint32_t *out);
static bool parse_hex(const char * arg, uint32_t *out);
static bool parse_bin(const char * arg, uint32_t *out);
static bool parse_number(const char * arg, uint32_t *out);


int main(int argc, const char *argv[]) {
    if (argc == 3) {
        uint32_t address, value;

        if (parse_number(argv[1], &address)) {
            if (parse_number(argv[2], &value)) {
                *((uint32_t*)address) = value;
                return 0; /* Success, E_OK */
            }
            else {
                fputs("Valeur non reconnue.\n", stderr);
            }
        }
        else {
            fputs("Adresse non reconnue.\n", stderr);
        }
    } else {
        puts("UTILISATION: POKE32 <addresse> <valeur>\n");
        return -1;
    }
}

static bool parse_dec(const char * arg, uint32_t *out) {
    uint32_t n = 0;
    while (*arg) {
        if (*arg == ':' || *arg == '_') {
            arg++;
            continue;
        } else if (isdigit(*arg)) {
            n *= 10;
            n += *arg - '0';
        } else {
            return false;
        }

        arg++;
    }

    *out = n;
    return true;
}

static bool parse_hex(const char * arg, uint32_t *out) {
    uint32_t n = 0;

    while (*arg) {
        if (*arg == ':' || *arg == '_') {
            arg++;
            continue;
        } else if (isdigit(*arg)) {
            n *= 16;
            n += *arg - '0';
        } else if ((*arg >= 'a') && (*arg <= 'f')) {
            n *= 16;
            n += *arg - 'a' + 10;
        } else if ((*arg >= 'A') && (*arg <= 'F')) {
            n *= 16;
            n += *arg - 'A' + 10;
        } else {
            return false;
        }

        arg++;
    }

    *out = n;
    return true;

}

static bool parse_bin(const char * arg, uint32_t *out) {
    uint32_t n = 0;

    while (*arg) {
        if (*arg == ':' || *arg == '_') {
            arg++;
            continue;
        } else if ((*arg == '0') || (*arg == '1')) {
            n *= 2;
            n += *arg - '0';
        } else {
            return false;
        }

        arg++;
    }

    *out = n;
    return true;

}

/*
 * Evaluate an argument to a number
 */
static bool parse_number(const char * arg, uint32_t *out) {
    size_t len = strlen(arg);

    if (len > 2) {
        if (arg[0] == '0') {
            if (arg[1] == 'x') {
                return parse_hex(&arg[2], out);
            } else if (arg[1] == 'b') {
                return parse_bin(&arg[2], out);
            }
        }
    }

    if (len > 1) {
        if (arg[0] == '$') {
            return parse_hex(&arg[1], out);
        } else if (arg[0] == '%') {
            return parse_bin(&arg[1], out);
        }
    }

    return parse_dec(arg, out);
}