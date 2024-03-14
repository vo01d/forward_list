#ifndef _TASK_H_
#define _TASK_H_

#include <memory>

// Visual Studio 2022

namespace task {
    class Node_base {
    public:
        std::unique_ptr<Node_base> m_next;

        Node_base(std::unique_ptr<Node_base> next = nullptr) noexcept : m_next(std::move(next)) {};
        virtual ~Node_base() noexcept = default;
    };

    template<typename _Ty>
    class Node : public Node_base {
    public:
        _Ty m_value;

        Node(std::unique_ptr<Node_base> next, _Ty value) : Node_base(std::move(next)), m_value(value) {}
        template<typename... Args>
        Node(std::unique_ptr<Node_base> next, Args&&... args) : Node_base(std::move(next)), m_value(std::forward<Args>(args)...) {}
        ~Node() noexcept = default;
    };

    template<typename _Ty>
    class forward_iterator_base {
    public:
        Node_base* m_node;

        forward_iterator_base(Node_base* node) : m_node(node) {}
        bool operator==(const forward_iterator_base& other) const {
            return this->m_node == other.m_node;
        }

        bool operator!=(const forward_iterator_base& other) const {
            return this->m_node != other.m_node;
        }
    };

    template<typename _Ty>
    class forward_list_iterator : public forward_iterator_base<_Ty> {
    public:
        using value_type = _Ty;
        using reference = value_type&;
        using pointer = value_type*;

        forward_list_iterator(Node_base* node) : forward_iterator_base<_Ty>(node) {}

        forward_list_iterator& operator++() {
            this->m_node = this->m_node->m_next.get();

            return *this;
        }

        forward_list_iterator operator++(int) {
            forward_list_iterator tmp = *this;
            this->m_node = this->m_node->m_next.get();

            return tmp;
        }

        reference operator*() const {
            return static_cast<Node<_Ty>*>(this->m_node)->m_value;
        }

        pointer operator->() const {
            return **this;
        }
    };

    template<typename _Ty>
    class forward_list_const_iterator : public forward_iterator_base<_Ty> {
    public:
        using value_type = _Ty;
        using reference = const value_type&;
        using pointer = const value_type*;

        forward_list_const_iterator(Node_base* node) : forward_iterator_base<_Ty>(node) {}

        forward_list_const_iterator(forward_list_iterator<_Ty> other) : forward_iterator_base<_Ty>(other.m_node) {}

        forward_list_const_iterator& operator++() {
            this->m_node = this->m_node->m_next.get();

            return *this;
        }

        forward_list_const_iterator operator++(int) {
            forward_list_iterator tmp = *this;
            this->m_node = this->m_node->m_next.get();

            return tmp;
        }

        reference operator*() const {
            return static_cast<Node<_Ty>*>(this->m_node)->m_value;
        }

        pointer operator->() const {
            return **this;
        }
    };

	template<typename _Ty>
	class forward_list {
	public:
		using value_type = _Ty;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
        using iterator = forward_list_iterator<_Ty>;
        using const_iterator = forward_list_const_iterator<_Ty>;
    public:
        forward_list() noexcept = default;

        forward_list(size_type count, const_reference value) {
            insert_after(before_begin(), count, value);
        }

        explicit forward_list(size_type count) {
            insert_after(before_begin(), count, _Ty());
        }

        template< class InputIt >
        forward_list(InputIt first, InputIt last) {
            insert_after(before_begin(), first, last);
        }

        forward_list(const forward_list& right) {
            insert_after(before_begin(), right.begin(), right.end());
        }

        forward_list(forward_list&& right) noexcept {
            m_head.m_next = std::move(right.m_head.m_next);
        }

        forward_list(std::initializer_list<_Ty> ilist) {
            insert_after(before_begin(), ilist.begin(), ilist.end());
        }

        ~forward_list() noexcept {}

        forward_list& operator=(const forward_list& right) {
            if (this != &right) {
                clear();
                insert_after(before_begin(), right.begin(), right.end());
            }

            return *this;
        }

        forward_list& operator=(forward_list&& right) noexcept {
            if (this != &right) {
                clear();
                m_head.m_next = std::move(right.m_head.m_next);
            }

            return *this;
        }

        forward_list& operator=(std::initializer_list<_Ty> ilist) {
            clear();
            insert_after(before_begin(), ilist.begin(), ilist.end());

            return *this;
        }

        void assign(size_type count, const_reference value) {
            clear();
            insert_after(before_begin(), count, value);
        }

        template< class InputIt >
        void assign(InputIt first, InputIt last) {
            clear();
            insert_after(before_begin(), first, last);
        }

        void assign(std::initializer_list<_Ty> ilist) {
            clear();
            insert_after(before_begin(), ilist.begin(), ilist.end());
        }

        reference front() {
            return *begin();
        }

        const_reference front() const {
            return *begin();
        }

        iterator before_begin() {
            return iterator(&m_head);
        }

        const_iterator before_begin() const {
            return const_iterator(&m_head);
        }

        iterator begin() {
            return iterator(m_head.m_next.get());
        }

        const_iterator begin() const {
            return const_iterator(m_head.m_next.get());
        }

        iterator end() {
            return iterator(nullptr);
        }

        const_iterator end() const {
            return const_iterator(nullptr);
        }

        const_iterator cbefore_begin() {
            return before_begin();
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        bool empty() const {
            return static_cast<bool>(!m_head.m_next);
        }

        void clear() {
            m_head.m_next.reset();
        }

        iterator insert_after(iterator pos, const_reference value) {
            return emplace_after(pos, value);
        }

        iterator insert_after(iterator pos, _Ty&& value) {
            return emplace_after(pos, std::move(value));
        }
        
        iterator insert_after(iterator pos, size_type count, const_reference value) {
            for (size_type i = 0; i < count; ++i) {
                pos = emplace_after(pos, value);
            }

            return pos;
        }

        template<class InputIt>
        iterator insert_after(iterator pos, InputIt first, InputIt last) {
            for (; first != last; ++first) {
                pos = emplace_after(pos, *first);
            }

            return pos;
        }

        iterator insert_after(iterator pos, std::initializer_list<_Ty> ilist) {
            return insert_after(pos, ilist.begin(), ilist.end());
        }

        template<typename... Args>
        iterator emplace_after(iterator pos, Args&&... args) {
            std::unique_ptr<Node<_Ty>> newNode = std::make_unique<Node<_Ty>>(std::move(pos.m_node->m_next), std::forward<Args>(args)...);
            pos.m_node->m_next = std::move(newNode);

            return iterator(pos.m_node->m_next.get());
        }

        iterator erase_after(iterator pos) {
            std::unique_ptr<Node_base> tmp = std::move(pos.m_node->m_next->m_next);
            pos.m_node->m_next.reset();
            pos.m_node->m_next = std::move(tmp);

            return iterator(pos.m_node->m_next.get());
        }

        void push_front(const_reference value) {
            emplace_front(value);
        }

        void push_front(_Ty&& value) {
            emplace_front(std::move(value));
        }

        template<class... Args>
        reference emplace_front(Args&&... args) {
            auto it = emplace_after(before_begin(), std::forward<Args>(args)...);

            return *it;
        }

        void pop_front() {
            erase_after(before_begin());
        }

        void swap(forward_list& other) {
            m_head.m_next.swap(other.m_head.m_next);
        }
	private:
        Node_base m_head;
	};
};

#endif 