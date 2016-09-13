/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_LIST_H
#define __SGI_STL_INTERNAL_LIST_H

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

//list结构，list是双向的
template <class T>
struct __list_node {
  typedef void* void_pointer;
  void_pointer next;	//这里就是两个指针，一个next，一个prev
  void_pointer prev;
  T data;
};
//

//list是一个双向链表，所有需要迭代器，迭代器需要自身，引用，指针。
template<class T, class Ref, class Ptr>
struct __list_iterator {
  typedef __list_iterator<T, T&, T*>             iterator;	//支持begin和end
  typedef __list_iterator<T, const T&, const T*> const_iterator;	//支持cbegin和cend
  typedef __list_iterator<T, Ref, Ptr>           self;

  //类型重新定义
  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef __list_node<T>* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  link_type node; //指向实际的list结点

  //迭代器的构造函数
  __list_iterator(link_type x) : node(x) {}	//对迭代器赋值的构造函数
  __list_iterator() {}	//声明默认构造函数
  __list_iterator(const iterator& x) : node(x.node) {}	//拷贝构造函数

  //重载==判断
  bool operator==(const self& x) const { return node == x.node; }
  //重载!=判断
  bool operator!=(const self& x) const { return node != x.node; }
  //重载*，在这注意传递引用变为左值，这样就可以更改所返回的节点。
  reference operator*() const { return (*node).data; }


#ifndef __SGI_STL_NO_ARROW_OPERATOR
  //重载->，返回一个指针，这样就可以把迭代器当作一个指针进行使用。
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */

  //支持迭代器的前置++
  self& operator++() { 
    node = (link_type)((*node).next);
    return *this;
  }

  //支持迭代器的后置++
  self operator++(int) { 
    self tmp = *this;
    ++*this;
    return tmp;
  }

  //支持迭代器的前置--
  self& operator--() { 
    node = (link_type)((*node).prev);
    return *this;
  }

  //支持迭代器的后置--
  self operator--(int) { 
    self tmp = *this;
    --*this;
    return tmp;
  }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class T, class Ref, class Ptr>
inline bidirectional_iterator_tag

iterator_category(const __list_iterator<T, Ref, Ptr>&) {
  return bidirectional_iterator_tag();
}

template <class T, class Ref, class Ptr>
inline T*
value_type(const __list_iterator<T, Ref, Ptr>&) {
  return 0;
}

template <class T, class Ref, class Ptr>
inline ptrdiff_t*
distance_type(const __list_iterator<T, Ref, Ptr>&) {
  return 0;
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

//list的定义

template <class T, class Alloc = alloc>
class list {
protected:
  typedef void* void_pointer;
  typedef __list_node<T> list_node;

  //空间配置器，进行空间配置，
  typedef simple_alloc<list_node, Alloc> list_node_allocator;
public:      
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef list_node* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

public:
	/*在这的时候，我们定义一个迭代器list<T>::iterator，首先调用的是
	__list_iterator<T, T&, T*>的构造函数，这样就形成了联系，当需要const的
	时候，就会用第二个的构造函数
	*/

  typedef __list_iterator<T, T&, T*>             iterator;
  typedef __list_iterator<T, const T&, const T*> const_iterator;

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  typedef reverse_bidirectional_iterator<const_iterator, value_type,
  const_reference, difference_type>
  const_reverse_iterator;
  typedef reverse_bidirectional_iterator<iterator, value_type, reference,
  difference_type>
  reverse_iterator; 
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

protected:
 //创建一个节点，并且返回的是节点的指针。
  link_type get_node() { return list_node_allocator::allocate(); }
 //释放一个节点，
  void put_node(link_type p) { list_node_allocator::deallocate(p); }

  //创建一个节点，并且用x进行初始化，
  link_type create_node(const T& x) {
    link_type p = get_node();
    __STL_TRY {
      construct(&p->data, x);
    }
    __STL_UNWIND(put_node(p));
    return p;
  }

  //销毁一个节点
  void destroy_node(link_type p) {
    destroy(&p->data);	//调用对象的析构函数，对后续删除空间进行准备
    put_node(p);		//释放内存
  }

protected:
	//初始化一个空链表，开辟内存，然后首尾相连就可以了。
  void empty_initialize() { 
    node = get_node();
    node->next = node;
    node->prev = node;
  }
  //初始化n个值为value的的空链表。
  void fill_initialize(size_type n, const T& value) {
    empty_initialize();
    __STL_TRY {
      insert(begin(), n, value);
    }
    __STL_UNWIND(clear(); put_node(node));
  }

#ifdef __STL_MEMBER_TEMPLATES
  template <class InputIterator>
  void range_initialize(InputIterator first, InputIterator last) {
    empty_initialize();
    __STL_TRY {
      insert(begin(), first, last);
    }
    __STL_UNWIND(clear(); put_node(node));
  }
#else  /* __STL_MEMBER_TEMPLATES */
  
  //初始化一段指针区间的链表
  void range_initialize(const T* first, const T* last) {
    empty_initialize();
    __STL_TRY {
      insert(begin(), first, last);
    }
    __STL_UNWIND(clear(); put_node(node));
  }

  //初始化一段迭代器区间的链表
  void range_initialize(const_iterator first, const_iterator last) {
    empty_initialize();
    __STL_TRY {
      insert(begin(), first, last);
    }
    __STL_UNWIND(clear(); put_node(node));
  }
#endif /* __STL_MEMBER_TEMPLATES */

protected:
  /*哨兵卫节点，起到一个头的作用，并且起到维护迭代器的作用，
	在这里的begin就是哨兵卫节点的next，end就是哨兵卫节点
	*/
	link_type node;
  

public:
	//默认的构造函数，直接默认创建一个哨兵卫。
  list() { empty_initialize(); }

  //迭代器begin()，返回一个哨兵卫的next的节点的指针
  iterator begin() { return (link_type)((*node).next); }

  //返回迭代器const_iterator,
  const_iterator begin() const { return (link_type)((*node).next); }
  
  //返回迭代器end(),就是返回哨兵卫
  iterator end() { return node; }
  
  //返回const类型的迭代器，迭代器无法再次更改。
  const_iterator end() const { return node; }

  //反向迭代器，返回的就是第一个节点的前一个节点，其实也就是哨兵卫。end的下一个就是哨兵卫
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  
  //const类型的反向迭代器
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }

  //反向迭代器end（），这个迭代器返回的就是指向最后一个元素
  reverse_iterator rend() { return reverse_iterator(begin()); }
 
  //const类型的end（），
  const_reverse_iterator rend() const { 
    return const_reverse_iterator(begin());
  } 

  //判断链表是否为空，如果只剩下一个哨兵卫，那么就是空链表。
  bool empty() const { return node->next == node; }

  //利用迭代器求出size
  size_type size() const {
	  sad
    size_type result = 0;
    distance(begin(), end(), result);
    return result;
  }

  //因为是无符号整形，所以返回-1，所以这里返回的就相当于是0xffff ffff
  size_type max_size() const { return size_type(-1); }
  
  //返回一个begin（）迭代器的解引用，在这是返回的引用的类型，所以这是个左值，也支持修改
  reference front() { return *begin(); }
  
  //返回const类型的的迭代器，
  const_reference front() const { return *begin(); }
  
  //返回最后一个元素的引用，利用迭代器end实现
  reference back() { return *(--end()); }

  //返回const类型
  const_reference back() const { return *(--end()); }
  
  //交换，利用现代的std::swap实现
  void swap(list<T, Alloc>& x) { __STD::swap(node, x.node); }
  
  //插入，返回位置传入的是迭代器所指向的位置，和x的引用，返回一个迭代器
  iterator insert(iterator position, const T& x) 
  {
    link_type tmp = create_node(x);	//先调用create_node(x)创建一个节点
    
	//把tmp节点插入到position这个位置之前。
	tmp->next = position.node;		
    tmp->prev = position.node->prev;
    (link_type(position.node->prev))->next = tmp;
    position.node->prev = tmp;
    return tmp;
  }

  //
  iterator insert(iterator position) { return insert(position, T()); }
#ifdef __STL_MEMBER_TEMPLATES
  template <class InputIterator>

 
  void insert(iterator position, InputIterator first, InputIterator last);
#else /* __STL_MEMBER_TEMPLATES */
  //插入在两个指针区间之间 
  void insert(iterator position, const T* first, const T* last);
  
  //插入在一个迭代器区间
  void insert(iterator position,
              const_iterator first, const_iterator last);
#endif /* __STL_MEMBER_TEMPLATES */
  //插入在迭代器pos，插入n个x。
  void insert(iterator pos, size_type n, const T& x);

  //重载insert
  void insert(iterator pos, int n, const T& x) {
    insert(pos, (size_type)n, x);
  }

  //重载insert
  void insert(iterator pos, long n, const T& x) {
    insert(pos, (size_type)n, x);
  }

  //头插，利用迭代器，相当于在哨兵卫的后面插入值为x的节点，
  void push_front(const T& x) { insert(begin(), x); }

  //在哨兵卫的前面插入一个值为x的节点，这样也就相当于给尾部插入一个节点。
  void push_back(const T& x) { insert(end(), x); }

  //擦除迭代器所指位置的元素。
  iterator erase(iterator position) 
  {

	//定义两个节点，一个前，一个后
    link_type next_node = link_type(position.node->next);
    link_type prev_node = link_type(position.node->prev);
    prev_node->next = next_node;
    next_node->prev = prev_node;

	//释放节点的空间
    destroy_node(position.node);
    return iterator(next_node);
  }

  //擦除迭代器区间
  iterator erase(iterator first, iterator last);

  //设置size个x
  void resize(size_type new_size, const T& x);

  //设置size
  void resize(size_type new_size) { resize(new_size, T()); }

  //清空链表
  void clear();

  //头删，擦除begin（）迭代器
  void pop_front() { erase(begin()); }

  //尾删
  void pop_back() { 
    iterator tmp = end();

	//擦除最后一个元素
    erase(--tmp);
  }

  //list的构造函数的实现，创建n个value的list
  list(size_type n, const T& value) { fill_initialize(n, value); }

	//重载上面的，现在是int类型的个数
  list(int n, const T& value) { fill_initialize(n, value); }
  
  //重载上面的，现在是long类型的个数
  list(long n, const T& value) { fill_initialize(n, value); }
  
  //创建n个，防止隐式类型转换
  explicit list(size_type n) { fill_initialize(n, T()); }

#ifdef __STL_MEMBER_TEMPLATES
  template <class InputIterator>
  list(InputIterator first, InputIterator last) {
    range_initialize(first, last);
  }

#else /* __STL_MEMBER_TEMPLATES */
  //实现list在一个指针区间之间的构造函数
  list(const T* first, const T* last) { range_initialize(first, last); }
  
  //实现在一个迭代器区间的构造
  list(const_iterator first, const_iterator last) {
    range_initialize(first, last);
  }
#endif /* __STL_MEMBER_TEMPLATES */
  
  //构造迭代器区间
  list(const list<T, Alloc>& x) {
    range_initialize(x.begin(), x.end());
  }
  //析构list
  ~list() {
    clear();	   //调用析构函数
    put_node(node);//释放空间
  }

  //重载赋值语句
  list<T, Alloc>& operator=(const list<T, Alloc>& x);

protected:
	
	//将[first,last)的元素搬移到position前，不包括last
  void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
		
		
		//把原有链表去除，然后安接在position之前。

      (*(link_type((*last.node).prev))).next = position.node;
      (*(link_type((*first.node).prev))).next = last.node;
      (*(link_type((*position.node).prev))).next = first.node;  
      link_type tmp = link_type((*position.node).prev);
      (*position.node).prev = (*last.node).prev;
      (*last.node).prev = (*first.node).prev; 
      (*first.node).prev = tmp;
    }
  }

public:

	//把x链表的元素放到position的前面
  void splice(iterator position, list& x) {
    if (!x.empty()) 
      transfer(position, x.begin(), x.end());
  }
  //把i所指向的元素插入到position之前
  void splice(iterator position, list&, iterator i) {
    iterator j = i;
    ++j;
    if (position == i || position == j) return;
    transfer(position, i, j);
  }

  //把迭代器[first,last)之间的元素插入到position之前
  // position 和[first,last)可指在同一个list，  
  // 但position不能位于[first,last)之內。
  void splice(iterator position, list&, iterator first, iterator last) {
    if (first != last) 
      transfer(position, first, last);
  }

  //删除值为value的元素
  void remove(const T& value);
  
  //去除重复的元素
  void unique();
  
  //归并排序
  void merge(list& x);

  //逆置链表
  void reverse();
  
  //排序
  void sort();

#ifdef __STL_MEMBER_TEMPLATES
  template <class Predicate> void remove_if(Predicate);
  template <class BinaryPredicate> void unique(BinaryPredicate);
  template <class StrictWeakOrdering> void merge(list&, StrictWeakOrdering);
  template <class StrictWeakOrdering> void sort(StrictWeakOrdering);
#endif /* __STL_MEMBER_TEMPLATES */

  friend bool operator== __STL_NULL_TMPL_ARGS (const list& x, const list& y);
};

template <class T, class Alloc>

//判断两个链表是否相等
inline bool operator==(const list<T,Alloc>& x, const list<T,Alloc>& y) {
  typedef typename list<T,Alloc>::link_type link_type;
  link_type e1 = x.node;
  link_type e2 = y.node;
  link_type n1 = (link_type) e1->next;
  link_type n2 = (link_type) e2->next;
  for ( ; n1 != e1 && n2 != e2 ;
          n1 = (link_type) n1->next, n2 = (link_type) n2->next)
    if (n1->data != n2->data)
      return false;
  return n1 == e1 && n2 == e2;
}

template <class T, class Alloc>

//判断两个链表
inline bool operator<(const list<T, Alloc>& x, const list<T, Alloc>& y) {
  return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class T, class Alloc>
inline void swap(list<T, Alloc>& x, list<T, Alloc>& y) {
  x.swap(y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

#ifdef __STL_MEMBER_TEMPLATES

//在position之前插入迭代器区间的元素
template <class T, class Alloc> template <class InputIterator>
void list<T, Alloc>::insert(iterator position,
                            InputIterator first, InputIterator last) {
  for ( ; first != last; ++first)
    insert(position, *first);
}

#else /* __STL_MEMBER_TEMPLATES */

template <class T, class Alloc>

//链表在position插入first和last区间
void list<T, Alloc>::insert(iterator position, const T* first, const T* last) {
  for ( ; first != last; ++first)
    insert(position, *first);
}

template <class T, class Alloc>

//重载list插入函数
void list<T, Alloc>::insert(iterator position,
                            const_iterator first, const_iterator last) {
  for ( ; first != last; ++first)
    insert(position, *first);
}

#endif /* __STL_MEMBER_TEMPLATES */

template <class T, class Alloc>

//在position之前插入n个值为x的节点
void list<T, Alloc>::insert(iterator position, size_type n, const T& x) {
  for ( ; n > 0; --n)
    insert(position, x);
}

template <class T, class Alloc>

//擦除一段迭代器区间的节点。
list<T,Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last) {
  while (first != last) erase(first++);
  return last;
}

template <class T, class Alloc> 

//实现修改size，
void list<T, Alloc>::resize(size_type new_size, const T& x)
{
  iterator i = begin();
  size_type len = 0;
  for ( ; i != end() && len < new_size; ++i, ++len)
    ;
  //如果new_siez小于原来的长度，删除[i,end())这段空间多余的节点
  if (len == new_size)
    erase(i, end());
  else                          // i == end()
	  //如果大于原来的长度，那么久插入节点。
    insert(end(), new_size - len, x);
}

template <class T, class Alloc> 

//清空整个list，利用for循环清空
void list<T, Alloc>::clear()
{
  link_type cur = (link_type) node->next;
  while (cur != node) {
    link_type tmp = cur;
    cur = (link_type) cur->next;
    destroy_node(tmp);	//销毁tmp节点。
  }

  //只剩下哨兵卫
  node->next = node;
  node->prev = node;
}

template <class T, class Alloc>

//进行赋值语句的重载

//把x链表插入到第一个链表的前面，然后把原来第一个链表擦除
list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& x) {
  if (this != &x) {
    iterator first1 = begin();
    iterator last1 = end();
    const_iterator first2 = x.begin();
    const_iterator last2 = x.end();
    while (first1 != last1 && first2 != last2) *first1++ = *first2++;
    if (first2 == last2)
      erase(first1, last1);
    else
      insert(last1, first2, last2);
  }
  return *this;
}

template <class T, class Alloc>

//删除值为value的链表

//first相当于要擦除的节点，从后向前遍历，寻找值为value的节点，进行擦除，
void list<T, Alloc>::remove(const T& value) {
  iterator first = begin();
  iterator last = end();
  while (first != last) {
    iterator next = first;
    ++next;
    if (*first == value) erase(first);
    first = next;
  }
}

template <class T, class Alloc>

//取出重复的结点，会存在一个问题，就是需要排序好的，
//这是在一个区间查找相等的节点，如果碰到不想等的那么久开始重新开始匹配。
void list<T, Alloc>::unique() {
  iterator first = begin();
  iterator last = end();
  if (first == last) return;
  iterator next = first;
  while (++next != last) {
    if (*first == *next)
      erase(next);
    else
      first = next;
    next = first;
  }
}

template <class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();
  while (first1 != last1 && first2 != last2)
    if (*first2 < *first1) {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    }
    else
      ++first1;
  if (first2 != last2) transfer(last1, first2, last2);
}

template <class T, class Alloc>


//逆置链表，利用transfer函数，把每一个节点搬移到old之前，这样下去，最后完成了逆置链表的过程。
void list<T, Alloc>::reverse() {
  if (node->next == node || link_type(node->next)->next == node) return;
  iterator first = begin();
  ++first;
  while (first != end()) {
    iterator old = first;
    ++first;
    transfer(begin(), old, first);
  }
}    

template <class T, class Alloc>
void list<T, Alloc>::sort() {
  if (node->next == node || link_type(node->next)->next == node) return;
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while(i < fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);         
    if (i == fill) ++fill;
  } 

  for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1]);
  swap(counter[fill-1]);
}

#ifdef __STL_MEMBER_TEMPLATES

template <class T, class Alloc> template <class Predicate>
void list<T, Alloc>::remove_if(Predicate pred) {
  iterator first = begin();
  iterator last = end();
  while (first != last) {
    iterator next = first;
    ++next;
    if (pred(*first)) erase(first);
    first = next;
  }
}

template <class T, class Alloc> template <class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
  iterator first = begin();
  iterator last = end();
  if (first == last) return;
  iterator next = first;
  while (++next != last) {
    if (binary_pred(*first, *next))
      erase(next);
    else
      first = next;
    next = first;
  }
}

template <class T, class Alloc> template <class StrictWeakOrdering>
void list<T, Alloc>::merge(list<T, Alloc>& x, StrictWeakOrdering comp) {
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();
  while (first1 != last1 && first2 != last2)
    if (comp(*first2, *first1)) {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    }
    else
      ++first1;
  if (first2 != last2) transfer(last1, first2, last2);
}

template <class T, class Alloc> template <class StrictWeakOrdering>
void list<T, Alloc>::sort(StrictWeakOrdering comp) {
  if (node->next == node || link_type(node->next)->next == node) return;
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while(i < fill && !counter[i].empty()) {
      counter[i].merge(carry, comp);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);         
    if (i == fill) ++fill;
  } 

  for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1], comp);
  swap(counter[fill-1]);
}

#endif /* __STL_MEMBER_TEMPLATES */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#endif

__STL_END_NAMESPACE 

#endif /* __SGI_STL_INTERNAL_LIST_H */

// Local Variables:
// mode:C++
// End:
