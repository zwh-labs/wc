#ifndef _WC_CONFIGURATION_ARGUMENTPARSER_H_
#define _WC_CONFIGURATION_ARGUMENTPARSER_H_


#include "wc_export.h"
#include <wc/Configuration.h>


WC_EXPORT wcConfiguration * wcConfiguration_newFromArguments( int argc, const char ** argv );


#endif
