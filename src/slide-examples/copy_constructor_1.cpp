class Class {
public:
    Class( const int *piData, int iDataLength )
    {
        m_piData = new int [iDataLength];
        CUtil::copyBuff(m_piData, piData, iDataLength);
        m_iDataLength   = iDataLength;
    }
    ~Class()
    {
        delete[] m_piData;
    }
private:
    int *m_piData;
    int m_iDataLength;
};
