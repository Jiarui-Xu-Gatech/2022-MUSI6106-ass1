class CEmployee {
public:
  CEmployee(string theName, float thePayRate);

  string getName() const;
  float getPayRate() const;
  float pay(float hoursWorked) const;
protected:
  string name;
  float payRate;
};

class CManager : public CEmployee {
public:
  CManager(string theName, float thePayRate, bool isSalaried);

  bool getSalaried() const;
  float pay(float hoursWorked) const {
    if (salaried)
        return payRate;
    /* else */
    return CEmployee::pay(hoursWorked);
  }
protected:
  bool salaried;
};

