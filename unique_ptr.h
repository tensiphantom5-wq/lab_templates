// #pragma once

// #include <cstddef>
// #include <memory>
// #include <utility>

// template <typename T, typename Deleter = std::default_delete<T>>
// class UniquePtr {
// public:
//     // ======================== Constructors ========================

//     UniquePtr();
//     explicit UniquePtr(T* ptr);
//     UniquePtr(T* ptr, const Deleter& deleter);
//     UniquePtr(T* ptr, Deleter&& deleter);

//     // =================== No copy ==================================

//     UniquePtr(const UniquePtr&)            = delete;
//     UniquePtr& operator=(const UniquePtr&) = delete;

//     // =================== Move ====================================

//     UniquePtr(UniquePtr&& other);
//     UniquePtr& operator=(UniquePtr&& other);

//     // =================== Destructor ==============================

//     ~UniquePtr();

//     // =================== Observers ===============================

//     T*       get();
//     const T* get() const;

//     explicit operator bool() const;

//     T&       operator*();
//     const T& operator*() const;

//     T*       operator->();
//     const T* operator->() const;

//     Deleter&       get_deleter();
//     const Deleter& get_deleter() const;

//     // =================== Modifiers ===============================

//     T*   release();
//     void reset(T* ptr = nullptr);
//     void swap(UniquePtr& other);

// private:
//     T*      ptr_ = nullptr;
//     Deleter deleter_;
// };

// // =====================================================================
// //  Partial specialization for arrays: UniquePtr<T[]>
// //  Uses operator[] instead of operator*/operator->.
// //  Default deleter is std::default_delete<T[]> (calls delete[]).
// // =====================================================================

// template <typename T, typename Deleter>
// class UniquePtr<T[], Deleter> {
// public:
//     // ======================== Constructors ========================

//     UniquePtr();
//     explicit UniquePtr(T* ptr);
//     UniquePtr(T* ptr, const Deleter& deleter);
//     UniquePtr(T* ptr, Deleter&& deleter);

//     // =================== No copy ==================================

//     UniquePtr(const UniquePtr&)            = delete;
//     UniquePtr& operator=(const UniquePtr&) = delete;

//     // =================== Move ====================================

//     UniquePtr(UniquePtr&& other);
//     UniquePtr& operator=(UniquePtr&& other);

//     // =================== Destructor ==============================

//     ~UniquePtr();

//     // =================== Observers ===============================

//     T*       get();
//     const T* get() const;

//     explicit operator bool() const;

//     T&       operator[](size_t index);
//     const T& operator[](size_t index) const;

//     Deleter&       get_deleter();
//     const Deleter& get_deleter() const;

//     // =================== Modifiers ===============================

//     T*   release();
//     void reset(T* ptr = nullptr);
//     void swap(UniquePtr& other);

// private:
//     T*      ptr_ = nullptr;
//     Deleter deleter_;
// };

// // =================== Free function ===============================

// template <typename T, typename... Args>
// UniquePtr<T> make_unique(Args&&... args);



#pragma once

#include <cstddef>
#include <memory>
#include <utility>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    UniquePtr() = default;
    explicit UniquePtr(T* ptr) : ptr_(ptr) {}
    UniquePtr(T* ptr, const Deleter& deleter) : ptr_(ptr), deleter_(deleter) {}
    UniquePtr(T* ptr, Deleter&& deleter) : ptr_(ptr), deleter_(std::move(deleter)) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        reset();
    }

    T* get() {
        return ptr_;
    }

    const T* get() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T& operator*() {
        return *ptr_;
    }

    const T& operator*() const {
        return *ptr_;
    }

    T* operator->() {
        return ptr_;
    }

    const T* operator->() const {
        return ptr_;
    }

    Deleter& get_deleter() {
        return deleter_;
    }

    const Deleter& get_deleter() const {
        return deleter_;
    }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            if (ptr_) {
                deleter_(ptr_);
            }
            ptr_ = ptr;
        }
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

private:
    T* ptr_ = nullptr;
    [[no_unique_address]] Deleter deleter_;
};

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    UniquePtr() = default;
    explicit UniquePtr(T* ptr) : ptr_(ptr) {}
    UniquePtr(T* ptr, const Deleter& deleter) : ptr_(ptr), deleter_(deleter) {}
    UniquePtr(T* ptr, Deleter&& deleter) : ptr_(ptr), deleter_(std::move(deleter)) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        reset();
    }

    T* get() {
        return ptr_;
    }

    const T* get() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T& operator[](size_t index) {
        return ptr_[index];
    }

    const T& operator[](size_t index) const {
        return ptr_[index];
    }

    Deleter& get_deleter() {
        return deleter_;
    }

    const Deleter& get_deleter() const {
        return deleter_;
    }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            if (ptr_) {
                deleter_(ptr_);
            }
            ptr_ = ptr;
        }
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

private:
    T* ptr_ = nullptr;
    [[no_unique_address]] Deleter deleter_;
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}