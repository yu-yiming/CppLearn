#include <iostream>
#include <list>
#include <vector>

void general_container_usage();

void vector_usage();
void list_usage();

/**
 * a general container C.
 * 
 * There are some shared interface among containers in the C++ standard library.
 * I'd like to go over these before discussing each container respectively.
 * 
 * In general, we can divide the containers into almost two groups:
 * - Sequence containers are structures that can be considered as a sequence of
 * data, including `std::array`, `std::vector`, `std::list`, etc.
 * - Associative containers are structures that put elements in certain place,
 * either to make the data sorted (ordered), like `std::set` and `std::map`, or 
 * just easy to find (hashed), like `std::unordered_set`.
 * 
 * There are some other containers such as the container adaptors, like stacks,
 * and a non-owning structure `std::span`.
 */
void general_container_usage() {
    // Constructors and assignments
    {
        // Except for `std::array`, all other containers have constructors.
        // Usually there are constructors of the following signature:
        // * C()
        // * C(other)
        // * C(&&other)
        // * C(ct, val)
        // * C(first, last)
        // * C(init-list)
        // The overloaded operator = is also defined for all except for arrays.
        // * operator =(other)
        // * operator =(&other)
        // * operator =(init-list)
        // The `assign` operation is defined for sequence containers e.f.a.
        // * assign(ct, val)
        // * assign(first, last)
        // * assign(init-list)
    }

    // Iterators
    {
        // Almost all containers implements the four containers.
        // Reverse containers are not supported by hashed containers and
        // forward lists.
        // * begin() -> iterator
        // * end() -> iterator
        // * cbegin() -> const_iterator
        // * cend() -> const_iterator
        // * rbegin() -> reverse_iterator
        // * rend() -> reverse_iterator
        // * crbegin() -> const_reverse_iterator
        // * crend() -> const_reverse_iterator
    }

    // Element access
    {
        // Size information are supported by almost all containers.
        // Forward lists don't provide `size` function.
        // * empty() -> bool
        // * size() -> size_type
        // * max_size() -> size_type
        // Containers that support O(1) access provides `at` and `operator []`.
        // * at() -> reference
        // * at() -> const_reference
        // * operator [] -> reference
        // * operator [] -> const_reference
    }

    // Modifiers
    {
        
    }
}

/**
 * std::vector<T, Alloc=std::allocator<T>>
 * is a sequence container that encapsulates a dynamic size array.
 * ? `T`: Element type, type of the elements stored in the vector.
 * ? `Allocator`: Allocator type, type of the memory manager that can acquire/
 * ? release memory and construct/destroy the elements in that memory.
 * 
 * - All elements are stored in a sequence, so a pointer to an element in an
 * array can be treated the same as an array.
 * - Heap memory is allocated, whose size is called "capacity" of the vector.
 * - Every time the allocated memory is used up, yet new element(s) are
 * given, a reallocation will take place, where all elements will possibly
 * copied to another heap memory chunk with bigger size.
 * - Cache-friendly, O(1) element access.
 * - Amortized O(1) for putting or removing an element at the back.
 * 
 * * We will seldom talk about allocators in the examples below.
 */
void vector_usage() {
    using std::vector;
    using std::cout;

    // Some preparation
    std::initializer_list<int> list = { 1, 2, 3 };
    int arr[] = { 4, 5, 6 };

    // Member types
    {
        using ValueType = vector<int>::value_type;
        using AllocatorType = vector<int>::allocator_type;
        using SizeType = vector<int>::size_type;
        using DifferenceType = vector<int>::difference_type;
        using Reference = vector<int>::reference;
        using ConstReference = vector<int>::const_reference;
        using Pointer = vector<int>::pointer;
        using ConstPointer = vector<int>::const_pointer;
        using Iterator = vector<int>::iterator;
        using ConstIterator = vector<int>::const_iterator;
        using ReverseIterator = vector<int>::reverse_iterator;
        using ConstReverseIterator = vector<int>::const_reverse_iterator;
    }

    // Constructors and assignments
    {
        vector<int> v1;                 // * vector()
        vector<int> v2{};               // * vector()
        vector<int> v3(3, 42);          // * vector(ct, value, ?alloc)
                                        // Make a vector with certain count of
                                        // given value.
        vector<int> v4(10);             // * vector(ct, ?alloc)
                                        // Make a vector of certain size.
                                        // Each entry is default initialized.
        vector<int> v5(list.begin(), list.end());
                                        // * vector(first, last, ?alloc)
                                        // Make a vector by given pair of input
                                        // iterators [begin, end).
        vector<int> v6(v1);             // * vector(other)
                                        // Make a copy of another vector.
        vector<int> v7(std::move(v1));  // * vector(other)
                                        // Make a vector by a moved object.
        vector<int> v8({});             // * vector(init-list, ?alloc)
                                        // Make a vector from an empty list.
        vector<int> v9 = { 1, 2, 3 };   // * vector(init-list, ?alloc)
                                        // Make a vector from a list.
        vector v10 = { 1, 2, 3 };       // Template parameter deduction.

        v2 = v3;                        // * operator =(other)
                                        // When the allocators are different,
                                        // deallocate the current vector and
                                        // allocate new memory for copied data.
        v2 = std::move(v4);             // * operator =(other)
                                        // Move the data of the other vector
                                        // to the current one using move
                                        // semantics.
        v2 = { 1, 2, 3 };               // * operator =(list)
                                        // Copy the contents of a list.
        v2.assign(3, 42);               // * assign(ct, value)
        v2.assign(arr, arr + 3);        // * assign(first, last)
        v2.assign(list);                // * assign(list)
    }

    // Element access
    {
        vector<int> v = { 1, 2, 3, 4, 5 };
        auto const& cv = v;
        vector<int> v_empty{};

        cout << v.at(0);                // * at(idx) -> reference
                                        // Get the reference of an element at
                                        // a given index.
        v.at(0) = -1;                   // Use `at` to modify an element.
        cout << cv.at(0);               // * at(idx) -> const_reference
                                        // Get the reference to const of an
                                        // element at a given index.
        try {
            (void)cv.at(100);           // Throw `std::out_of_range` exception.
        }
        catch (...) {}

        cout << v[3];                   // * operator [](idx) -> reference
                                        // Almost the same as `at`, but without
                                        // boundary check.
        cout << cv[3];                  // * operator [](idx) -> const_reference

        cout << v.front();              // * front() -> reference
                                        // Equivalent to *v.begin().
        cout << cv.front();             // * front() -> const_reference
                                        // Equivalent to *v.cbegin().
        cout << v.back();               // * back() -> reference
                                        // Equivalent to *std::prev(v.end()).
        cout << cv.back();              // * back() -> const_reference
                                        // Equivalent to *std::prev(v.cend()).
        (void)v_empty.front();          // Undefined behavior using `front`
                                        // and `back` to empty vectors.

        int* borrowed_array = v.data(); // * data() -> T*
                                        // Get the underlying array.
        for (int i = 0; i < v.size(); ++i) {
            cout << borrowed_array[i] << " ";
        }
        (void)cv.data();                // * data() -> T const*
        (void)v_empty.data();           // This might be nullptr.

    }

    // Iterators
    {
        vector<int> v = { 1, 2, 3, 4, 5 };
        auto const& cv = v;

        auto it = v.begin();            // * begin() -> iterator
                                        // Get the iterator to the first entry.
                                        // If empty, this will be equivalent
                                        // to `end`.
        (void)v.cbegin();               // * cbegin() -> const_iterator
        auto end = v.end();             // * end() -> iterator
                                        // Get the iterator to the place after
                                        // the last element.
        (void)v.cend();                 // * cend() -> const_iterator
        auto rit = v.rbegin();          // * rbegin() -> reverse_iterator
                                        // Get the reverse iterator to the last
                                        // entry. If empty, this will be
                                        // equivalent to `rend`.
        (void)v.crbegin();              // * crbegin() -> const_reverse_iterator
        auto rend = v.rend();           // * rend() -> reverse_iterator
                                        // Get the reverse iterator to the
                                        // place after the first element.
        (void) v.crend();               // * crend() -> const_reverse_iterator

        // Traverse a vector.
        while (it != end) {
            cout << *it;
            ++it;
        }
        // Traverse a vector in reverse.
        while (rit != rend) {
            cout << *rit;
            ++rit;
        }
        
    }

    // Capacity
    {
        vector<int> v = { 1, 2, 3, 4, 5 };
        vector<int> v_empty{};

        cout << v.empty();              // * empty() -> bool
                                        // Return whether the vector is empty.
        cout << v.size();               // * size() -> size_type
                                        // Return the size of a vector.
        cout << v.max_size();           // * max_size() -> size_type
                                        // Return max possible size of vector.
        v_empty.reserve(10);            // * reserve(size)
                                        // Make sure the capacity of vector
                                        // is no less than the given size.
                                        // Reallocation takes place if capacity
                                        // is smaller than the given size.
        cout << v_empty.capacity();     // * capacity() -> size_type
                                        // Get the capacity, i.e. the size of
                                        // the dynamically alloacted memory.
        v_empty.shrink_to_fit();        // * shrink_to_fit()
                                        // Remove the unused capacity.
                                        // Reallocation might happen.
        
    }

    // Modifiers
    {
        vector<int> v = { 1, 2, 3, 4, 5 };
        int arr[] = { 5, 6, 7 };
        vector<std::string> v_str;

        // v = []
        v.clear();                      // * clear()
                                        // Erase all elements in the vector,
                                        // yet the capacity stays unchanged.
        // v = [1]
        v.insert(v.begin(), 1);         // * insert(pos, val) -> iterator
                                        // Insert an element at the given pos.
                                        // Return an iterator to that position.
        // v = [3, 2, 1]
        v.insert(v.insert(v.begin(), 2), 3);
                                        // `insert` can be nested.
        // v = [4, 4, 4, 3, 2, 1]
        v.insert(v.begin(), 3, 4);      // * insert(pos, ct, val) -> iterator
                                        // Insert certain count of value to the
                                        // given position.
        // v = [4, 4, 4, 3, 2, 1, 5, 6, 7]
        v.insert(v.end(), arr, arr + 3);// * insert(pos, first, last) -> iterator
                                        // Insert elements between a pair of
                                        // iterators [begin, end) to the given
                                        // position.
        // v = [4, 4, 4, 3, 2, 1, 5, 6, 7, 8, 9, 10]
        v.insert(v.end(), { 8, 9, 10 });// * insert(pos, init-list) -> iterator
                                        // Insert an initializer list ot the
                                        // given position.
        // v = [10, 4, 4, 4, 3, 2, 1, 5, 6, 7, 8, 9, 10]
        v.emplace(v.begin(), 10);       // * emplace(pos, args...) -> iterator
                                        // Insert an element constructed from
                                        // `args...` to the given position.
        // v_str = ["aaa"]
        v_str.emplace(v_str.begin(), 3, 'a');
                                        // Almost the same as inserting
                                        // `std::string(3, 'a')`
        // v = [4, 4, 4, 3, 2, 1, 5, 6, 7, 8, 9, 10]
        v.erase(v.begin());             // * erase(pos) -> iterator
                                        // Erase the element at given position.
                                        // Return iterator following that pos.
        // v = []
        v.erase(v.begin(), v.end());    // * erase(first last) -> iterator
                                        // Erase elements between a pair of
                                        // iterators [begin, end).
                                        // Return iterator at `end`.
        // v = [42]
        v.push_back(42);                // * push_back(val)
                                        // Insert an element at the back.
        // v_str = ["aaa", "abc"]
        v_str.emplace_back("abc");      // * emplace_back(args...)
                                        // Almost the same as `push_back`,
                                        // but construct the object first.
        // v = []
        v.pop_back();                   // * pop_back()
                                        // Erase the last element.
        // v = [0, 0, 0, 0, 0]
        v.resize(5);                    // * resize(ct)
                                        // Resize the vector to certain size.
                                        // Extra elements will be abandoned.
                                        // New elements will be default init.
        // v = [0, 0, 0, 0, 0, 42, 42, 42, 42, 42]
        v.resize(10, 42);               // * resize(ct, val)
                                        // Same as `resize(ct)` except that
                                        // new elements will be initialized
                                        // to `val`.
        vector<int> tmp;
        // v = []
        // tmp = [0, 0, 0, 0, 0, 42, 42, 42, 42, 42]
        tmp.swap(v);                    // * swap(other)
                                        // Swap with another vector.
                                        // Only exchange the underlying ptr,
                                        // so this is O(1) time.
    }

    // Non-memmber functions
    {
        vector<int> v1 = { 1, 2, 3 };
        vector<int> v2 = { 1, 2, 4 };

        cout << (v1 == v2);
        cout << (v1 < v2);              // * ::operator <(v1, v2)
                                        // Lexicographic comparison.
        
        std::erase(v1, 3);              // * std::erase(v, val)
                                        // Erase elements in a vector that
                                        // equal to the given value.
        std::erase_if(v2, [](auto i) { return i > 3; });
                                        // * std::erase_if(v, pred)
                                        // Erase elements in a vector upto
                                        // certain predicate.
        std::swap(v1, v2);              // * std::swap(v1, v2)
                                        // Swap two vectors.
                                        // Same as v1.swap(v2)
    }

    // Deduction guides
    {
        // vector(first:It, last:It, ?alloc) -> vector<std::iterator_traits<It>::value_type>
    }
}

/**
 * std::list<T, Alloc=std::allocator<T>>
 * is a linear linked structure that may not have space locality.
 * ? `T`: Element type, type of the elements stored in the vector.
 * ? `Allocator`: Allocator type, type of the memory manager that can acquire/
 * ? release memory and construct/destroy the elements in that memory.
 * 
 * - Implementation may be a doubly linked list, i.e. wrapper of collection
 * of nodes each of which stores an element, the address of the previous node,
 * and the address of the next node.
 * - Insertion/erasure of an element at a given position is O(1) time.
 * - Random access is *not* supported. Visit an element at certain position is
 * O(n) time.
 * - Way slower than std::vector, but somehow more general.
 */
void list_usage() {
    using std::list;
    using std::cout;

    int arr[] = { 1, 2, 3, 4, 5 };

    // Constructors and assignments.
    {
        list<int> l1{};                 // * list()
                                        // Default initialization.
        list<int> l2(3, 10);            // * !list(ct, val, ?alloc)
                                        // Initialize a list with certain count
                                        // of a certain value.
        list<int> l3(5);                // * !list(ct, ?alloc)
                                        // Initialize a list with certain count
                                        // of the default initialized value.
        list<int> l4(l1);               // * list(other)
                                        // Copy another list.
        list<int> l5(std::move(l2));    // * list(&&other)
                                        // Initialize the list with an moved
                                        // object. This will move all nodes
                                        // managed by `other`.
        list<int> l5({ 1, 2, 3 });      // * list(init-list, ?alloc)

        l5 = l3;                        // * operator =(other)
                                        // Copy another list by replacing the
                                        // current contents with a copy of
                                        // contents of `other`.
        l5 = std::move(l4);             // * operator =(&&other)
                                        // Move another list.
        l5 = { 1, 2, 3, 4, 5 };         // * operator =(init-list)
                                        // Replace the contents with a list.
        l5.assign(3, 42);               // * assign(ct, val)
        l5.assign(arr, arr + 5);        // * assign(first, last)
        l5.assign({ 1, 2, 3 });         // * assign(init-list)
    }

    // Element access
    {
        list<int> l = { 1, 2, 3 };
        auto const& cl = l;

        cout << l.front();              // * front() -> reference
                                        // Get the reference of the first elem.
        l.front() = 10;
        cout << cl.front();             // * front() -> const_reference
                                        // Get the reference to const of the
                                        // first elem.
        cout << l.back();               // * back() -> reference
                                        // Get the reference of the last elem.
        l.back() = 10;
        cout << cl.back();              // * back() -> const_reference
                                        // Get the reference to const of the
                                        // last elem.
    }


}