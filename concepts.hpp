#ifndef LOOPR_CONCEPTS
#define LOOPR_CONCEPTS

///<summary> 
/// Any type that can be multiplied, divided by, added to,  subtracted from a float
///</summary>
template <typename T> 
concept Arithmetic = requires(T a, float b) {
    a * b;
    a / b;
    a + b;
    a - b;
};

#endif
