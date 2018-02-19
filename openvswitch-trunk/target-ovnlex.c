#include "ovn/lex.h"
#include "openvswitch/dynamic-string.h"
#include <stdio.h>

static void
compare_token(const struct lex_token *a, const struct lex_token *b)
{
    if (a->type != b->type) {
        fprintf(stderr, "type differs: %d -> %d\n", a->type, b->type);
        return;
    }

    if (!((a->s && b->s && !strcmp(a->s, b->s))
          || (!a->s && !b->s))) {
        fprintf(stderr, "string differs: %s -> %s\n",
                a->s ? a->s : "(null)",
                b->s ? b->s : "(null)");
        return;
    }

    if (a->type == LEX_T_INTEGER || a->type == LEX_T_MASKED_INTEGER) {
        if (memcmp(&a->value, &b->value, sizeof a->value)) {
            fprintf(stderr, "value differs\n");
            return;
        }

        if (a->type == LEX_T_MASKED_INTEGER
            && memcmp(&a->mask, &b->mask, sizeof a->mask)) {
            fprintf(stderr, "mask differs\n");
            return;
        }

        if (a->format != b->format
            && !(a->format == LEX_F_HEXADECIMAL
                 && b->format == LEX_F_DECIMAL
                 && a->value.integer == 0)) {
            fprintf(stderr, "format differs: %d -> %d\n",
                    a->format, b->format);
        }
    }
}

#if 0
void refill_dstr(struct ds *dstr, char *str)
{
    ds_clear(dstr);
    ds_put_cstr(dstr, str);
}

int
preprocess_data(struct ds *out, char *data)
{
    char *s, *comment;
    s = strtok(data, "\n");
    // Get a line of data
    if (s == NULL) return 1;

    while (s != NULL) {
        refill_dstr(out, s);
	s = ds_cstr(out);
        if (*s == '#') {
            puts(s);
	    s = strtok(NULL, "\n");
            continue;
        }

        comment = strchr(s, '#');
        if (comment) {
            *comment = '\0';
        }
        if (s[strspn(s, " \t\n")] == '\0') {
            putchar('\n');
	    s = strtok(NULL, "\n");
            continue;
        }

        return 0;
    }
    return 1;
}
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

    if (size == 0 || data[size-1] != '\0')
      return 0;

    struct ds input;
    struct ds output;

    ds_init(&input);
    ds_init(&output);

    FILE *fp;
    fp = fopen("tmp123", "wb");
    fwrite(data, size, 1, fp);
    fclose(fp);

    while (!ds_get_test_line(&input, fp)) {
        struct lexer lexer;
    
        lexer_init(&lexer, ds_cstr(&input));
        ds_clear(&output);
        while (lexer_get(&lexer) != LEX_T_END) {
            size_t len = output.length;
            lex_token_format(&lexer.token, &output);
    
            /* Check that the formatted version can really be parsed back
             * losslessly. */
            if (lexer.token.type != LEX_T_ERROR) {
                const char *s = ds_cstr(&output) + len;
                struct lexer l2;
    
                lexer_init(&l2, s);
                lexer_get(&l2);
                compare_token(&lexer.token, &l2.token);
                lexer_destroy(&l2);
            }
            ds_put_char(&output, ' ');
        }
        lexer_destroy(&lexer);
    
        ds_chomp(&output, ' ');
        puts(ds_cstr(&output));
    }
    ds_destroy(&input);
    ds_destroy(&output);
    return 0;
}
