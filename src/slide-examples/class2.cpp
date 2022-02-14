class CRectangle 
{
public:
    CRectangle () :  				// publicly accessible                  
        m_iWidth(0),
        m_iHeight(0)
    {};
    virtual ~CRectangle () {};
    void setValues (int,int);
    int getArea() 
    {
        return m_iWidth*m_iHeight;
    }
private:							// hidden
    int m_iWidth, 
        m_iHeight;

};

void CRectangle::setValues (int iWidth, int iHeight)
{
	if ((iWidth < 0) || (iHeigth < 0))
		throw "Exception: width or height negative!";
	
	m_iWidth 	= iWidth;
	m_iHeight	= iHeight;
}