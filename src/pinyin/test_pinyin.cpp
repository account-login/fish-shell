// system
#include <assert.h>
// proj
#include "./pinyin_data.cpp"
#include "./pinyin_match.cpp"


int main() {
    assert(miss == pinyin_match(L"p", L"qasdf"));
    assert(exact == pinyin_match(L"p", L"p"));
    assert(prefix == pinyin_match(L"p", L"pp"));
    assert(substr == pinyin_match(L"p", L"qp"));
    assert(subseq == pinyin_match(L"pq", L"apaq"));

    assert(miss == pinyin_match(L"chu", L"𬺓"));
    assert(miss == pinyin_match(L"x", L"〈"));  // U+3008

    assert(miss == pinyin_match(L"a", L"吧"));
    assert(exact == pinyin_match(L"a", L"啊"));
    assert(prefix == pinyin_match(L"a", L"啊哦"));
    assert(prefix == pinyin_match(L"wn", L"吾能吞琉璃而不傷"));
    assert(prefix == pinyin_match(L"wuneng", L"吾能吞琉璃而不傷"));
    assert(prefix == pinyin_match(L"wneng", L"吾能吞琉璃而不傷"));
    assert(prefix == pinyin_match(L"wun", L"吾能吞琉璃而不傷"));
    assert(prefix == pinyin_match(L"wunen", L"吾能吞琉璃而不傷"));
    assert(exact == pinyin_match(L"wnt琉lebs", L"吾能吞琉璃而不傷"));
    assert(exact == pinyin_match(L"wntllebs", L"吾能吞琉璃而不傷"));
    assert(substr == pinyin_match(L"ll", L"吾能吞琉璃而不傷"));
    assert(substr == pinyin_match(L"lli", L"吾能吞琉璃而不傷"));
    assert(substr == pinyin_match(L"liul", L"吾能吞琉璃而不傷"));
    assert(substr == pinyin_match(L"liuli", L"吾能吞琉璃而不傷"));
    assert(subseq == pinyin_match(L"llbs", L"吾能吞琉璃而不傷"));

    return 0;
}
