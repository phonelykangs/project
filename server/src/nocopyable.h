#ifndef __NOCOPYABLE_H__
#define __NOCOPYABLE_H__

namespace server{

class Nocopyable{
public:
    Nocopyable() = default;
    ~Nocopyable() = default;
    Nocopyable(const Nocopyable&) = delete;
    Nocopyable& operator=(const Nocopyable&) = delete;
};

}

#endif