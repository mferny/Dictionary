#include <unordered_map>

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
    explicit NotFoundExceptionImpl(const TKey &key) : key_(key) {}
    virtual const TKey& GetKey() const noexcept override {
        return key_;
    }
};

template<class TKey, class TValue>
class DerivedDictionary : public Dictionary<TKey, TValue>
{
private:
    std::unordered_map<TKey, TValue> dict;
public:
    virtual ~DerivedDictionary() = default;

    virtual const TValue& Get(const TKey& key) const {
        if (dict.count(key) == 0) {
            throw NotFoundExceptionImpl<TKey>{key};
        }
        return dict.at(key);
    }
    virtual void Set(const TKey& key, const TValue& value) {
        dict[key] = value;
    }
    virtual bool IsSet(const TKey& key) const {
        return dict.count(key) != 0;
    }
};
