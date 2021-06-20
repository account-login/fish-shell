#pragma once

#include <string>
#include "./pinyin_data.h"


uint8_t pinyin_match(const std::wstring &pattern, const std::wstring &str);
