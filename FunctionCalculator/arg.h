#pragma once

enum ARG_TYPE {
    ARG_NUMBER, ARG_NAME
};

struct ARG {
    enum ARG_TYPE type;
    void** value;
};

int argcmp(const struct ARG_TYPE*, const struct ARG_TYPE*);