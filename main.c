#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
extern FILE* yyin;
extern const char* get_generated_program(void);
extern int has_semantic_errors(void);

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input.halal> [output.c]\n", argv[0]);
        return 1;
    }

    const char* input_path = argv[1];
    const char* output_path = (argc == 3) ? argv[2] : "out.c";

    yyin = fopen(input_path, "r");
    if (!yyin) {
        perror("Failed to open input file");
        return 1;
    }

    int parse_status = yyparse();
    fclose(yyin);

    if (parse_status != 0 || has_semantic_errors()) {
        fprintf(stderr, "Compilation failed: parse/semantic errors found.\n");
        return 1;
    }

    const char* generated = get_generated_program();
    if (!generated) {
        fprintf(stderr, "Compilation failed: no output generated.\n");
        return 1;
    }

    FILE* out = fopen(output_path, "w");
    if (!out) {
        perror("Failed to open output file");
        return 1;
    }

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "%s", generated);
    fclose(out);

    printf("Generated C code: %s\n", output_path);
    return 0;
}
