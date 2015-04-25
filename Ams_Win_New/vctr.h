#ifndef __GAND_VECTOR__
#define __GAND_VECTOR__

namespace gcl {

enum VTYPE { VT_NULL=0,VT_DATE,VT_NUM,VT_INT,VT_ROWID,VT_STRING };

template <class T>
class vector
{
  public:
    //
    // Types.
    //
    typedef T*       iterator;
    typedef const T* const_iterator;

    typedef T        value_type;
    typedef T&       reference;
    typedef T const& const_reference;

  protected:


    iterator start;
    iterator finish;
    iterator end_of_storage;

    void insert_aux (iterator position, const T& x);

  public:
    //
    // construct/copy/destroy
    //
    vector () : start(0), finish(0), end_of_storage(0) {}
    //
    // Build a vector of size n with each element set to default for type T.
    // This requires that T have a default constructor.
    //
    explicit vector (int n)
    {
        start = new T[n];
        finish = start + n;
        end_of_storage = finish;
    }
    ~vector ()
    {
        if(start) delete[] start;
    }

//    vector (const vector<T>& x);
    vector<T>& operator= (const vector<T>& x)
    {
      if(start) delete[] start;
      start = x.start;
      finish = x.finish;
      end_of_storage = x.end_of_storage;
    }

    //
    // Iterators.
    //
    iterator       begin ()       { return start;  }
    const_iterator begin () const { return start;  }
    iterator       end ()         { return finish; }
    const_iterator end ()   const { return finish; }

    //
    // Capacity.
    //
    int size ()     const { return (end() - begin()); }
    void resize (int new_size);
    void resize (int new_size, T value);

    int       capacity () const { return (end_of_storage - begin()); }
    bool      empty ()    const { return begin() == end();                    }

    void reserve (int n)
    {
        if (capacity() < n)
        {
          if(start)
          {
            iterator tmp = new T[n];
            iterator t=start;
            iterator t1=tmp;
            for(;t<finish;) *t1++=*t++;
            finish = tmp + size();
            delete[] start;
            start = tmp;
          }
          else start=finish=new T[n];
          end_of_storage = start + n;
        }
    }

    //
    // Element access.
    //
    reference       operator[] (int n)       { return *(begin() + n); }
    const_reference operator[] (int n) const { return *(begin() + n); }
    reference       at (int n)               { return *(begin() + n); }
    const_reference at (int n)         const { return *(begin() + n); }
    reference       front ()                       { return *begin();       }
    const_reference front ()                 const { return *begin();       }
    reference       back ()                        { return *(end() - 1);   }
    const_reference back ()                  const { return *(end() - 1);   }

    //
    // Modifiers.
    //
    void push_back (const T& x)
    {
        if (finish == end_of_storage)  reserve(size()*3/2+2);
        *finish++=x;
    }

    void pop_back()
    {
        T def;
        *(--finish)=def;
    }

    void erase (iterator position)
    {
        T def;
        if (position + 1 != end())
         for(;position+1<finish;) *position++=position[1];
        *(--finish)=def;
    }

    void erase (iterator first, iterator last)
    {
        T def;
        iterator t=first,i=last;
        for(;i<finish;) *t++=*i++;
        t=finish - (last - first);
        for(;t<finish;) *t++=def;
        finish = finish - (last - first);
    }
    void eraseAll() { erase(begin(),end()); }
};

};
using gcl::VTYPE;
#endif /*__GAND_VECTOR__*/
