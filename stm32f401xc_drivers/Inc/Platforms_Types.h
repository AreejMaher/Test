/*
 * Platforms_Types.h
 *
 * Created on: Aug 18, 2025
 * Author: Ahmed Ezzat
 */

#ifndef PLATFORMS_TYPES_H_
#define PLATFORMS_TYPES_H_

/*-------------------------------- Types -------------------------------------*/

#define NULL 							(void*)(0)

typedef _Bool							boolean;

typedef char							char_t;
typedef signed char						sint8_t;
typedef unsigned char					uint8_t;

typedef signed short					sint16_t;
typedef unsigned short					uint16_t;

typedef signed int						sint32_t;
typedef unsigned int					uint32_t;

typedef signed long long				sint64_t;
typedef unsigned long long				uint64_t;

typedef volatile char					vchar_t;
typedef volatile signed char			vsint8_t;
typedef volatile unsigned char			vuint8_t;

typedef volatile signed short			vsint16_t;
typedef volatile unsigned short			vuint16_t;

typedef volatile signed int				vsint32_t;
typedef volatile unsigned int			vuint32_t;

typedef volatile signed long long		vsint64_t;
typedef volatile unsigned long long		vuint64_t;

typedef float							float32;
typedef double							float64;

/*-------------------------------- Defines -------------------------------------*/

#ifndef TRUE
#define TRUE    ((boolean) 1)
#endif

#ifndef FALSE
#define FALSE   ((boolean) 0)
#endif

#endif /* PLATFORMS_TYPES_H_ */
