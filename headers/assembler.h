
#ifndef M14_ASSEMBLER_H
#define M14_ASSEMBLER_H

#include "macro_list.h"
#include "symbols.h"

/*sends files to pre processor and returns an am file*/
void manage_files(int num_files ,char ** file_list, macro_table_t*, symbol_table_t*);

#endif /*M14_ASSEMBLER_H*/
