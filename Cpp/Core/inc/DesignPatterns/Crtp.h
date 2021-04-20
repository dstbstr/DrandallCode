#ifndef __CRTP_H__
#define __CRTP_H__

template<class T, template<typename> class CrtpType>
class Crtp {
public:
    T& Self() {
        return static_cast<T&>(*this);
    }
    const T& Self() const {
        return static_cast<const T&>(*this);
    }

private:
    Crtp() = default;
    friend CrtpType<T>;
};
#endif // __CRTP_H__