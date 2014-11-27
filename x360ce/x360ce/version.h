#pragma once

#include "globals.h"
#include "svnrev.h"

#define VERSION_MAJOR 3
#define VERSION_MINOR 4
#define VERSION_PATCH 1
#define VERSION_REVISION SVN_REV

#define VERSION_CONFIG 2

#define PRODUCT_VERSION TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "." TOSTRING(VERSION_PATCH) "." SVN_REVSTR
#define FILE_VERSION TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "." TOSTRING(VERSION_PATCH) "." TOSTRING(VERSION_REVISION)

