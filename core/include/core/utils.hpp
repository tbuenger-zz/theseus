#pragma once

#include <type_traits>
#include <vector>
#include <memory>

template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

};


template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


template< class T > struct like_pointer                     : std::is_pointer<T> {};
template< class T > struct like_pointer<std::shared_ptr<T>> : std::true_type {};
template< class T > struct like_pointer<std::unique_ptr<T>> : std::true_type {};


template <typename T, typename Enable = void>
struct Printer
{
    static std::ostream& print(std::ostream& stream, const T& t)
    {
        return stream << t;
    }  
};


template <typename T>
struct Printer<T, typename std::enable_if<like_pointer<T>::value>::type>
{
    static std::ostream& print(std::ostream& stream, const T& t)
    {
        return stream << *t;
    }  
};


template <typename T>
struct Joiner
{
    const std::vector<T>& elements;
    const std::string& divider;
};


template <typename T>
Joiner<T> join(const std::vector<T>& elements, const std::string& divider)
{
    return {elements, divider};
}


template <typename T>
inline std::ostream& operator<< (std::ostream& stream, const Joiner<T>& j)
{
    int index = 0;
    for (auto& e : j.elements)
    {
        if (index++ > 0)
            stream << j.divider;
        Printer<T>::print(stream, e);
    }
    return stream;
}

inline void hash_combine(std::size_t& seed, std::size_t h)
{
    seed ^= h + 0x9e3779b9 + (seed<<6) + (seed>>2);
}