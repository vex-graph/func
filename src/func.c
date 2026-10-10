#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFINITION _Static_assert(1, "native opcode compiler");
#define OVERVIEW _Static_assert(1, "compiler module map");

;;DEFINITION
/* Cold, standalone compile-time decoder. Both input spellings produce flat
 * literal-only opcode rows, then C23 source. The emitted program contains no
 * parser or VM. The Python launcher owns compiler deadlines and publication.
 * No ecosystem headers, allocation ABI, or interpreted execution are borrowed. */
;;OVERVIEW
/**
 * ============================================================================
 * MODULE: Func native source emitter (src/func.c)
 * ============================================================================
 * No public class or header is owned by this procedural CLI module.
 * Both spellings decode into the same flat, compiler-owned instruction rows.
 *
 * PRIVATE HELPERS (file-local behaviorless decoded instruction record):
 * ----------------------------------------------------------------------------
 *   Row {
 *     int opcode;       // canonical operation ID: add=1, mul=2, print=3
 *     int64_t left;     // first signed decimal literal operand
 *     int64_t right;    // second literal; zero/unused for print
 *   }
 *
 * FUNCTION REGISTRY:
 * ----------------------------------------------------------------------------
 * Public CLI entry:
 *   - main(argc, argv)
 *       decoder numeric|surface < source; emits native C23, never runs it
 *
 * Private cold validation:
 *   - reject(message)                     : one diagnostic, false result
 *   - skip(cursor)                        : whitespace at token boundaries
 *   - integer(cursor, dest)               : checked signed decimal int64
 *   - operation(cursor, surface, dest)    : shared ID/name + arity selection
 *   - decode(source, surface, dest, count): complete flat-row admission
 *
 * Private code generation:
 *   - literal(value)                      : INT64_MIN-safe native constant
 *   - emit(rows, count)                   : checked add/mul, ordered output
 *
 * OWNERSHIP / FAILURE CONTRACT:
 * ----------------------------------------------------------------------------
 *   Input and rows are owned only during main; every exit frees both.
 *   Row storage grows geometrically, with allocation/size failure observable.
 *   add/mul take two literals; print takes one. Each prints its native result.
 *   Empty, unknown, wrong-arity, overflowing literal, embedded-NUL and malformed
 *   sources reject before any generated source is emitted. Native arithmetic
 *   overflow exits nonzero instead of wrapping or invoking undefined behavior.
 *   The 1 MiB source ceiling is an external-input safety bound. No threads,
 *   public object constructors, reference operands, control flow or VM exist.
 * ============================================================================
 */

/* Fixed external-input safety ceiling, not a total runtime entity capacity. */
#define SOURCE_CAP (1024u * 1024u)
#define INITIAL_ROWS 16u
#define ADD_ID 1
#define MUL_ID 2
#define PRINT_ID 3

typedef struct Row {
    int opcode;
    int64_t left;
    int64_t right;
} Row;

/* One cold rejection diagnostic; callers propagate failure without more logs. */
static bool reject(const char *message) {
    fprintf(stderr, "[vex] %s:%d: func: %s\n", __FILE__, __LINE__, message);
    return false;
}

/* Whitespace is permitted at token boundaries, never silently inside tokens. */
static void skip(const char **cursor) {
    while (isspace((unsigned char) **cursor))
        ++*cursor;
}

/* Parse a complete signed decimal integer without wrapping. */
static bool integer(const char **cursor, int64_t *dest) {
    skip(cursor);
    const char *start = *cursor;
    const char *digits = start;
    if (*digits == '-' || *digits == '+')
        ++digits;
    if (!isdigit((unsigned char) *digits))
        return reject("expected decimal integer");
    errno = 0;
    char *end = nullptr;
    intmax_t value = strtoimax(start, &end, 10);
    if (errno == ERANGE || value < INT64_MIN || value > INT64_MAX)
        return reject("integer out of range");
    *cursor = end;
    *dest = (int64_t) value;
    return true;
}

/* Surface names and numeric IDs share the same canonical opcode identities. */
static bool operation(const char **cursor, bool surface, int *dest) {
    skip(cursor);
    if (!surface) {
        int64_t id;
        if (!integer(cursor, &id))
            return false;
        if (id != ADD_ID && id != MUL_ID && id != PRINT_ID)
            return reject("unknown opcode");
        *dest = (int) id;
        return true;
    }
    const char *start = *cursor;
    while (isalpha((unsigned char) **cursor))
        ++*cursor;
    size_t length = (size_t) (*cursor - start);
    if (length == 3 && memcmp(start, "add", 3) == 0)
        *dest = ADD_ID;
    else if (length == 3 && memcmp(start, "mul", 3) == 0)
        *dest = MUL_ID;
    else if (length == 5 && memcmp(start, "print", 5) == 0)
        *dest = PRINT_ID;
    else
        return reject("unknown surface operation");
    skip(cursor);
    if (**cursor != '(')
        return reject("expected opening parenthesis");
    ++*cursor;
    return true;
}

/* Decode transactionally: output source is emitted only after full validation. */
static bool decode(const char *source, bool surface, Row **dest, size_t *count) {
    const char *cursor = source;
    size_t capacity = 0;
    *dest = nullptr;
    *count = 0;
    skip(&cursor);
    while (*cursor != '\0') {
        Row row = {0};
        if (!operation(&cursor, surface, &row.opcode))
            return false;
        if (!surface) {
            skip(&cursor);
            if (*cursor++ != ',')
                return reject("expected operand separator");
        }
        if (!integer(&cursor, &row.left))
            return false;
        if (row.opcode != PRINT_ID) {
            skip(&cursor);
            if (*cursor++ != ',')
                return reject("missing second operand");
            if (!integer(&cursor, &row.right))
                return false;
        }
        skip(&cursor);
        if (surface) {
            if (*cursor++ != ')')
                return reject("wrong arity or missing closing parenthesis");
            skip(&cursor);
            if (*cursor == ';')
                ++cursor;
            else if (*cursor != '\0')
                return reject("expected semicolon between calls");
        } else if (*cursor != '\0') {
            if (*cursor++ != ',')
                return reject("expected instruction separator");
            skip(&cursor);
            if (*cursor == '\0')
                return reject("trailing instruction separator");
        }
        if (*count == capacity) {
            size_t next = capacity == 0 ? INITIAL_ROWS : capacity * 2;
            if (next < capacity || next > SIZE_MAX / sizeof(Row))
                return reject("row capacity overflow");
            Row *grown = realloc(*dest, next * sizeof(Row));
            if (grown == nullptr)
                return reject("row allocation failed");
            *dest = grown;
            capacity = next;
        }
        (*dest)[(*count)++] = row;
        skip(&cursor);
    }
    return *count != 0 || reject("empty program");
}

/* Emit INT64_MIN safely; native arithmetic checks reject before overflowing. */
static void literal(int64_t value) {
    if (value == INT64_MIN)
        fputs("INT64_MIN", stdout);
    else
        printf("INT64_C(%" PRId64 ")", value);
}

static bool emit(const Row *rows, size_t count) {
    fputs("#include <stdint.h>\n#include <inttypes.h>\n#include <stdio.h>\nint main(void) {\n", stdout);
    for (size_t i = 0; i < count; ++i) {
        const Row *row = &rows[i];
        fputs("{ int64_t a = ", stdout);
        literal((*row).left);
        fputs(", b = ", stdout);
        literal((*row).right);
        fputs(", result = a; (void)b;\n", stdout);
        if ((*row).opcode != PRINT_ID)
            printf("if (__builtin_%s_overflow(a, b, &result)) { fputs(\"func: arithmetic overflow\\n\", stderr); return 1; }\n",
                (*row).opcode == ADD_ID ? "add" : "mul");
        fputs("printf(\"%\" PRId64 \"\\n\", result); }\n", stdout);
    }
    fputs("return 0; }\n", stdout);
    return !ferror(stdout) || reject("source output failed");
}

/* Read a bounded stdin unit, emit C, free all temporary compiler state. */
int main(int argc, char **argv) {
    if (argc != 2 || (strcmp(argv[1], "numeric") != 0 && strcmp(argv[1], "surface") != 0)) {
        reject("usage: decoder numeric|surface < source");
        return 1;
    }
    char *source = malloc(SOURCE_CAP + 1);
    if (source == nullptr) {
        reject("source allocation failed");
        return 1;
    }
    size_t length = fread(source, 1, SOURCE_CAP, stdin);
    bool valid = !ferror(stdin) && fgetc(stdin) == EOF && memchr(source, 0, length) == nullptr;
    source[length] = '\0';
    Row *rows = nullptr;
    size_t count = 0;
    bool ok = valid ? decode(source, strcmp(argv[1], "surface") == 0, &rows, &count) : reject("invalid or oversized source");
    if (ok)
        ok = emit(rows, count);
    free(rows);
    free(source);
    return ok ? 0 : 1;
}
