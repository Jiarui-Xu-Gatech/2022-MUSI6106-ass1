class CEmployee {
public:
  CEmployee(string theName, float thePayRate);
  virtual float pay(float hoursWorked) const;
};

class CManager : public CEmployee {
public:
  CManager(string theName, float thePayRate, bool isSalaried);
  float pay(float hoursWorked);
};

