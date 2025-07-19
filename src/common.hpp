#ifndef COMMON_HPP
#define COMMON_HPP

// helpers to set directory of main.cpp at compile time
// usage: STRING(CURRENT_DIR)
// based on https://stackoverflow.com/a/196093
#define QUOTE(var) #var
#define STRING(var) QUOTE(var)
#ifndef CURRENT_DIR
#   error "missing definition of CURRENT_DIR"
#endif


#endif
