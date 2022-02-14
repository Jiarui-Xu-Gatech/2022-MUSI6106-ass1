#include <iostream>

class CTimeOfDay 
{ 
public:                                 // publicly accessible
    CTimeOfDay () :                     // initialize to 00:00
        m_iHour(0),
        m_iMin(0)
    {};
    virtual ~CTimeOfDay () {};

    void setTime(int iHour, int iMin)
    {
        m_iHour = iHour; 
        m_iMin  = iMin; 
    }
    void displayTime(void)
    { 
        if (m_iHour > 12)
            std::cout << m_iHour-12 << ":" << m_iMin << "P.M.";
        else
            std::cout << m_iHour << ":" << m_iMin << "A.M."; 
    }
    void displayTime24h(void)
    { 
        std::cout << m_iHour << ":" << m_iMin; 
    }
private:                                // keyword private -> hidden
    int m_iHour;
    int m_iMin;
};
