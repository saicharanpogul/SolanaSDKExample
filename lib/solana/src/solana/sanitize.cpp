#include "sanitize.h"

const char* SanitizeError::what() const throw() {
    switch (error) {
        case IndexOutOfBounds: return "index out of bounds";
        case ValueOutOfBounds: return "value out of bounds";
        case InvalidValue: return "invalid value";
    }
}

template <typename T>
void Sanitize<T>::sanitize() {
    // Default implementation does nothing
}

template <typename T>
void Vec<T>::sanitize() {
    for (auto& x : *this) {
        x.sanitize();
    }
}
