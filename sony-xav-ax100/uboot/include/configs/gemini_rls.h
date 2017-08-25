#include "gemini.h"

/* To partially quiet u-boot (silent after loading NAND env), don't set CONFIG_EXTRA_ENV_SETTINGS below
 * but use "setenv silent 1 ; saveenv".
 *
 * To completely quiet u-boot, set prebuilt env "silent=1" via CONFIG_EXTRA_ENV_SETTINGS
 * because initial prints happen before loading NAND env.
 */
#define CONFIG_EXTRA_ENV_SETTINGS   "silent=1"
