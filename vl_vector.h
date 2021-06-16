//
// Created by idopinto12 on 16/06/2021.
//

#ifndef EX6_IDOPINTO12_VL_VECTOR_H
#define EX6_IDOPINTO12_VL_VECTOR_H
template <class T,size_t tatic_capcity = 16>
class vl_vector{
    public:
    /* Life-Span operations*/
    // TODO DefaultConstructor
    vl_vector():
    {
        _size = 0;
        _capacity = Static_capcity;
        dynamic_vector = nullptr;
        for (int i = 0; i < sta; ++i) {

        }




    }

    // TODO CopyConstructor
    // TODO SequencebasedConstructor
    template<class InputIterator>vl_vector(InputIterator first, InputIterator last);
    // TODO Single-valueinitializedconstructor
    // TODO Destructor
    /*--------------------*/
    /* Methods*/
    // TODO size
    /**
     * this function returns the current size of the vector(# of elements)
     * return type size_t
     **/

    // TODO capacity
    /**
     * this function the current capacity of the vector
     * return type size_t. (NO NEED to calculate capC)
     **/

    // TODO empty
    /**
     * this function returns true if the vector is empty. false otherwise
     * return type bool
     */
    private:
    T static_vector[StaticCapacity];
    T* dynamic_vector;
    size_t _size;
    size_t _capacity;
};

#endif //EX6_IDOPINTO12_VL_VECTOR_H
