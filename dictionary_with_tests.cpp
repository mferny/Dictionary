#include <unordered_map>
#include "test_runner.h"

template<class TKey, class TValue>
class Dictionary
{
public:
    virtual ~Dictionary() = default;

    virtual const TValue& Get(const TKey& key) const = 0;
    virtual void Set(const TKey& key, const TValue& value) = 0;
    virtual bool IsSet(const TKey& key) const = 0;
};

template<class TKey>
class NotFoundException : public std::exception
{
public:
    virtual const TKey& GetKey() const noexcept = 0;
};

template<class TKey>
class NotFoundExceptionImpl : public NotFoundException<TKey>
{
private:
    TKey key_;
public:
    explicit NotFoundExceptionImpl(const TKey& key) : key_(key) {}
    const TKey& GetKey() const noexcept override {
        return key_;
    }
};

template<class TKey, class TValue>
class DictionaryImpl : public Dictionary<TKey, TValue>
{
private:
    std::unordered_map<TKey, TValue> dict;
public:
    ~DictionaryImpl() = default;

    const TValue& Get(const TKey& key) const {
        if (dict.count(key) == 0) {
            throw NotFoundExceptionImpl<TKey>{key};
        }
        return dict.at(key);
    }
    void Set(const TKey& key, const TValue& value) {
        dict[key] = value;
    }
    bool IsSet(const TKey& key) const {
        return dict.count(key) != 0;
    }
};

void TestGet() {
    DictionaryImpl<int, int> dict;
    dict.Set(1, 2);
    ASSERT_EQUAL(dict.Get(1), 2);
    dict.Set(100, 1000);
    ASSERT_EQUAL(dict.Get(100), 1000);
    ASSERT_EQUAL(dict.Get(1), 2);
    try {
        dict.Get(10);
    } catch(NotFoundExceptionImpl<int>& e) {
    } catch(...) {
        cerr << "TestGet: Exception doesn't work properly" << endl;
    }
}

void TestIsSet() {
    DictionaryImpl<char, int> dict;
    dict.Set('a', 97);
    ASSERT_EQUAL(dict.IsSet('a'), 1);
    ASSERT_EQUAL(dict.IsSet('b'), 0);
    dict.Set('b', 98);
    ASSERT_EQUAL(dict.IsSet('a'), 1);
    ASSERT_EQUAL(dict.IsSet('b'), 1);
    ASSERT_EQUAL(dict.IsSet('c'), 0);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestGet);
    RUN_TEST(tr, TestIsSet);
    return 0;
}
