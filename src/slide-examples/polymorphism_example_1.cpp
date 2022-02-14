CEmployee *pCEmpl;
if (condition1) {
  pCEmpl = new CEmployee(...);
} else if (condition2) {
  pCEmpl = new CManager(...);
}

float fPayRate = pCEmpl->getPayRate ();