#ifndef COMMON_HPP
#define COMMON_HPP

// helpers to set directory of main.cpp at compile time
// based on https://stackoverflow.com/a/196093
#define QUOTE(var) #var
#define STRING(var) QUOTE(var)

// usage: STRING(SOURCE_DIR)
#ifndef SOURCE_DIR
#   error "missing definition of SOURCE_DIR"
#endif

// usage: STRING(ASSETS_DIR)
#ifndef ASSETS_DIR
#   error "missing definition of ASSETS_DIR"
#endif

#endif
