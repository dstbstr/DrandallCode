#include <iostream>
using namespace std;

class CDummystar {
  public:
    int isitme (CDummystar *param);
};

int CDummystar::isitme (CDummystar *param)
{
  if (param == this) return true;
  else return false;
}

class CDummyamp {
  public:
    int isitme (CDummyamp &param);
};

int CDummyamp::isitme (CDummyamp &param)
{
  if (&param == this) return true;
  else return false;
}


class CDummyOpOverload {
public:
  bool operator==(const CDummyOpOverload& other) {
    return &other == this;
  }
};

int main ()
{
  CDummystar a;
  CDummystar* b = &a;
  if ( b->isitme(&a) )
    cout << "yes, &a is b\n";
  CDummyamp c;
  CDummyamp* d = &c;
  if ( d->isitme(c) )
    cout << "yes, &c is d\n";

    CDummyOpOverload e;
    CDummyOpOverload f = e;
    if( e == f )
      cout << "yes, e and f are equal";

  return 0;
}
