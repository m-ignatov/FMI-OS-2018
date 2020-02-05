#include "commons.h"

const enum SEG_TYPE SEG_ORDER[] = {
    text,
    text,
    numeric,
    byte,
    text};

const struct base parameters[] = {
    //parameter; segment; position; valid values

    {"device_name", 0, 0, "[a-zA-Z0-9_-]"},
    {"rom_revision", 0, 1, "[a-zA-Z0-9_-.]"},
    {"serial_number", 0, 2, "[A-Z0-9]"},

    {"bd_addr_part0", 1, 0, "[A-Z0-9:]"},
    {"bd_addr_part1", 1, 1, "[A-Z0-9:]"},
    {"bd_pass_part0", 1, 2, "[a-z0-9]"},

    {"serial_baudrate", 2, 0, "1200, 2400, 4800, 9600, 19200, 115200"},
    {"audio_bitrate", 2, 1, "32, 96, 128, 160, 192, 256, 320"},
    {"sleep_period", 2, 2, "100, 500, 1000, 5000, 10000"},

    {"serial_parity", 3, 0, "N, E, O"},
    {"serial_data_bit", 3, 1, "5, 6, 7, 8"},
    {"serail_stop_bit", 3, 2, "0, 1"},

    {"bd_pass_part1", 4, 0, "[a-z0-9]"},
    {"rom_checksum_part0", 4, 1, "[a-z0-9]"},
    {"rom_checksum_part1", 4, 2, "[a-z0-9]"}

};

int parametersSize = sizeof(parameters) / sizeof(struct base);
const int SEGMENT_SIZE = 64;