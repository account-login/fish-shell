
# from https://github.com/mozillazg/pinyin-data/blob/master/pinyin.txt

CHAR_MAP = {
    'à': 'a',
    'á': 'a',
    'è': 'e',
    'é': 'e',
    'ê': 'e',
    'ì': 'i',
    'í': 'i',
    'ò': 'o',
    'ó': 'o',
    'ù': 'u',
    'ú': 'u',
    'ü': 'v',
    'ā': 'a',
    'ē': 'e',
    'ě': 'e',
    'ī': 'i',
    'ń': 'n',
    'ň': 'n',
    'ō': 'o',
    'ū': 'u',
    'ǎ': 'a',
    'ǐ': 'i',
    'ǒ': 'o',
    'ǔ': 'u',
    'ǘ': 'v',
    'ǚ': 'v',
    'ǜ': 'v',
    'ǹ': 'n',
    '̀': '',
    '̄': '',
    '̌': '',
    'ḿ': 'm',
    'ế': 'e',
    'ề': 'e',
}


def normalize(py):
    s = ''
    for ch in py:
        ch = CHAR_MAP.get(ch, ch)
        if len(ch) == 0:
            continue
        assert 'a' <= ch <= 'z'
        s += ch
    return s


def gen(u, py):
    assert len(py) <= 6
    if len(py) < 6:
        code = '"%s"' % py
    else:
        code = ', '.join(repr(x) for x in py)
        code = '{%s}' % code
    print('    {0x%04x, %s},' % (u, code))


import sys
u_list = list()
py_list = list()
for line in sys.stdin:
    line, _, _ = line.partition('#')
    line = line.strip()
    if not line:
        continue

    # U+3007: líng,yuán,xīng  # 〇
    u, _, pylist = line.partition(': ')
    u = int(u[2:], 16)
    pylist = pylist.split(',')
    pylist = sorted(set(normalize(x) for x in pylist))

    if u > 0xffff:
        continue

    for py in pylist:
        u_list.append(u)
        py_list.append(py)


assert sorted(u_list) == u_list


print(rf'''
#include "./pinyin_data.h"

static pinyin_t g_data[{len(u_list)}] = {{''')

for u, py in zip(u_list, py_list):
    gen(u, py)

print(rf'''}};

const pinyin_t *const g_pinyin_begin = &g_data[0];
const pinyin_t *const g_pinyin_end = &g_data[{len(u_list)}];
''')
