#include "csv_ops.h"

inline string GetField(string row, int field_number) {
    int field_count = field_number;
    size_t field_start_pos = 0;
    size_t field_end_pos = row.find(',');
    while (field_count != 0 && field_end_pos != string::npos) {
        field_start_pos = field_end_pos + 1;
        field_end_pos = row.find(',', field_start_pos);
        field_count--;
    }
    return row.substr(field_start_pos, field_end_pos);
}