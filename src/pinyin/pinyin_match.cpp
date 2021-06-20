#include "./pinyin_match.h"


static const pinyin_t *lookup(uint16_t u) {
    const pinyin_t *lo = g_pinyin_begin;
    const pinyin_t *hi = g_pinyin_end;
    const pinyin_t *mid = NULL;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (mid->code > u) {
            hi = mid;
        } else if (mid->code < u) {
            lo = mid + 1;
        } else {
            break;
        }
    }
    while (mid > g_pinyin_begin && mid[-1].code == u) {
        mid--;
    }
    return mid->code == u ? mid : g_pinyin_end;
}

static const wchar_t k_first_hanzi = 0x3007;

static size_t match_py(const wchar_t *p, const wchar_t *p_end, const wchar_t *s) {
    if (*p == *s) {
        return 1;   // direct match
    }
    if ('a' <= *p && *p <= 'z' && 'A' <= *s && *s <= 'Z') {
        // NOTE: casefold only when the user input is lower case
        if (*p == *s - ('A' - 'a')) {
            return 1;   // icase match
        }
    }

    if (*s < k_first_hanzi || *s > 0xffff) {
        return 0;   // can not be pinyin
    }

    // try to match each pinyin by prefix, return the prefix length
    size_t matched = 0;
    const pinyin_t *py = lookup(*s);
    for (; py < g_pinyin_end && py->code == *s; ++py) {
        size_t i = 0;
        for (i = 0; i < 6 && py->pinyin[i]; ++i) {
            if (p + i >= p_end) {
                break;
            }
            // NOTE: no casefold here, pinyin can only matched by lower case
            if (p[i] != (wchar_t)py->pinyin[i]) {
                break;
            }
        }
        if (i > matched) {
            matched = i;
        }
    }
    return matched;
}

// string_fuzzy_match_t::contain_type_t plus one
enum pinyin_match_cls {
    miss = 0,
    exact,   // exact match: foobar matches foo
    prefix,  // prefix match: foo matches foobar
    substr,  // substring match: ooba matches foobar
    subseq,  // subsequence match: fbr matches foobar
};

// FIXME: greedy pinyin match can cause mismatch later
// FIXME: greedy subsequence match may not detect substring match
uint8_t pinyin_match(const std::wstring &pattern, const std::wstring &str)
{
    if (pattern.empty() || str.empty()) {
        return miss;    // ???
    }

    uint8_t cls = 0;
    const wchar_t *p = pattern.data();
    const wchar_t *p_end = pattern.data() + pattern.size();
    const wchar_t *s = str.data();
    const wchar_t *s_end = str.data() + str.size();
    while (p < p_end && s < s_end) {
        size_t matched = match_py(p, p_end, s);
        if (matched && cls == 0) {
            cls = (s == str.data()) ? prefix : substr;
        } else if (!matched && cls != 0) {
            cls = subseq;   // downgrade
        }
        p += matched;
        s += 1;
    }
    if (p != p_end) {
        return miss;
    }

    if (cls == prefix && s == s_end) {
        cls = exact;    // upgrade
    }
    return cls;
}
