template <class T>
class CScopedPtrIf
{
public:
    CScopedPtrIf (): m_pObject(NULL)
    {
    }

    CScopedPtrIf (T* const pNewObject) : m_pObject (pNewObject)
    {
    }

    virtual ~CScopedPtr()
    {
        delete m_pObject;
    }

    virtual CScopedPtr& operator= (T* const pNewObject)
    {
        if (m_pObject != pNewObject)
        {
            delete m_pObject;
            m_pObject   = pNewObject;
        }

        return *this;
    }   

    operator T*() const
    {
      return m_pObject;
    }

    /*Return the objects address*/
    T& operator*() const
    { 
      return *m_pObject;
    }

    T* operator->() const 
    {
      return m_pObject; 
    }

    bool operator!() const
    {
      return !m_pObject;
    }

    // Releases the object and returns it
    T* release()
    {
        if(m_pObject != NULL)
        {
            T* const pReleasedObject = m_pObject;
            m_pObject = NULL;
            return pReleasedObject; 
        }		
    }

protected:
    T* m_pObject;

};

template <class T>
bool operator== (const CScopedPtrIf<T>& pointer1, T* const pointer2) 
{
    return static_cast <T*> (pointer1) == pointer2;
}

template <class T>
bool operator!= (const CScopedPtrIf<T>& pointer1, T* const pointer2) 
{
    return static_cast <T*> (pointer1) != pointer2;
}
