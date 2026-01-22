#ifndef HSCOPT_DEFS_H
#define HSCOPT_DEFS_H

#if defined(__GNUC__) || defined(__clang__)

#define HSCOPT_INLINE static inline __attribute__((always_inline))
#define HSCOPT_UNUSED __attribute__((unused))
#define HSCOPT_LIKELY(x) __builtin_expect(!!(x), 1)
#define HSCOPT_UNLIKELY(x) __builtin_expect(!!(x), 0)

#else

#define HSCOPT_INLINE static inline
#define HSCOPT_UNUSED
#define HSCOPT_LIKELY(x) (x)
#define HSCOPT_UNLIKELY(x) (x)

#endif

#define HSCOPT_CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define HSCOPT_SWAP(type, a, b) \
  do {                          \
    type _tmp = (a);            \
    (a) = (b);                  \
    (b) = _tmp;                 \
  } while (0)

#endif /* HSCOPT_DEFS_H */
