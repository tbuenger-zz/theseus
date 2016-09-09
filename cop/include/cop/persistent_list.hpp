#include <cassert>
#include <memory>
#include <functional>
#include <initializer_list>
#include <iterator>

template<class T>
class PersistentList
{
    struct Item
    {
        Item(T v, std::shared_ptr<const Item> tail) 
            : _val(v), _next(std::move(tail)) 
        {}
        // singleton
        explicit Item(T v) : _val(v) {}
        //~Item() { std::cout << "~" << _val << std::endl; }
        T _val;
        std::shared_ptr<const Item> _next;

        size_t size() const
        {
            if (_next)
            {
                return _next->size() + 1;
            }
            else
            {
                return 1;
            }
        }
    };
    friend Item;
    explicit PersistentList(std::shared_ptr<const Item> items) 
        : _head(std::move(items)) {}
public:
    // Empty list
    PersistentList() {}
   // Cons
    PersistentList(T v, PersistentList const & tail) 
        : _head(std::make_shared<Item>(v, tail._head)) {}
    // Singleton
    explicit PersistentList(T v) : _head(std::make_shared<Item>(v)) {}


    bool isEmpty() const { return !_head; } // conversion to bool
    T front() const
    {
        assert(!isEmpty());
        return _head->_val;
    }
    PersistentList popped_front() const
    {
        assert(!isEmpty());
        return PersistentList(_head->_next);
    }
    // Additional utilities
    PersistentList pushed_front(T v) const
    {
        return PersistentList(v, *this);
    }
    PersistentList take(int n)
    {
        if (n <= 0 || isEmpty()) return PersistentList();
        return popped_front().take(n - 1).pushed_front(front());
    }
    PersistentList insertedAt(int i, T v) const
    {
        if (i == 0)
            return pushed_front(v);
        else {
            assert(!isEmpty());
            return PersistentList<T>(front(), popped_front().insertedAt(i - 1, v));
        }
    }
    PersistentList removed(T v) const
    {
        if (isEmpty()) return PersistentList();
        if (v == front())
            return popped_front().removed(v);
        return PersistentList(front(), popped_front().removed(v));
    }
    PersistentList removed1(T v) const
    {
        if (isEmpty()) return PersistentList();
        if (v == front())
            return popped_front();
        return PersistentList(front(), popped_front().removed(v));
    }
    bool member(T v) const
    {
        if (isEmpty()) return false;
        if (v == front()) return true;
        return popped_front().member(v);
    }
    template<class F>
    void forEach(F f) const
    {
        Item const * it = _head.get();
        while (it != nullptr)
        {
            f(it->_val);
            it = it->_next.get();
        }
    }

    size_t size() const
    {
        if (isEmpty())
        {
            return 0;
        }
        else
        {
            return _head->size();
        }
    }
    
    //friend class FwdPersistentListIter<T>;
    // For debugging
    int headCount() const { return _head.use_count(); }
private:
    std::shared_ptr<const Item> _head;
};
