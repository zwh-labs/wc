
#ifndef WC_EXPORT_H
#define WC_EXPORT_H

#define WC_EXPORT
#define WC_NO_EXPORT

#ifndef WC_DEPRECATED
#  define WC_DEPRECATED __attribute__ ((__deprecated__))
#  define WC_DEPRECATED_EXPORT WC_EXPORT __attribute__ ((__deprecated__))
#  define WC_DEPRECATED_NO_EXPORT WC_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define WC_NO_DEPRECATED
#endif

#endif
