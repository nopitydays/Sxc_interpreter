#include "sxc_interface.h"

void simple_stm(FILE *fp) {
    long int pos;

    pos = ftell(fp);

    value_stm(fp);

    fseek(fp, -1, SEEK_CUR);
    if (fgetc(fp) == ';' && fgetc(fp) == '\n') {
        ;
    } else {
        fseek(fp, pos, SEEK_SET);
        error_sxc(SIMPLE_ExtraCharAS, fp);
    }
}
