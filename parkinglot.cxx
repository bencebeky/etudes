/* An object-oriented parking lot. */
#include <iostream>
#include <string>

using namespace std;

class lot;
class spot;
class vehicle;

class lot {
  public:
    lot(int n);
    ~lot();
    void print();
    int park(vehicle*, bool);
    int leave(vehicle*, bool);
  private:
    spot *first;
};

class spot {
  public:
    spot();
    ~spot();
    int park(vehicle*);
    int leave();
    spot* getNext();
    void setNext(spot*);
    vehicle* getVehicle();
  private:
    spot *next;
    vehicle *v;
};

class vehicle {
  public:
    vehicle(std::string);
    ~vehicle();
    std::string getDescription();
  private:
    std::string description;
};

lot::lot(int n) {
  if(n==0) {
    first = NULL;
  } else {
    int i;
    spot *next, *curr;
    first = new spot();
    next = first;
    for(i=1; i<n; i++) {
      curr = next;
      next = new spot();
      curr->setNext(next);
    }
    next->setNext(NULL);
  }
}

lot::~lot() {
  spot *curr, *next;
  curr = first;
  while (curr != NULL) {
    next = curr->getNext();
    delete curr;
    curr = next;
  }
}

void lot::print() {
  int i;
  spot *s;
  for(i=0, s=first; s != NULL; s = s->getNext(), i++) {
    if (s->getVehicle() == NULL)
      cout << "Spot " << i << " empty; ";
    else
      cout << "Spot " << i << " has " << s->getVehicle()->getDescription() << "; ";
  }
  if (first==NULL)
    cout << "Zero sized lot.";
  cout << endl;
}

int lot::park(vehicle *v, bool verbose) {
  spot *s;
  for(s = first; s != NULL; s = s->getNext())
    if (s->park(v)) {
      if(verbose)
        cout << "Successfully parked " << v->getDescription() << "." << endl;
      return true;
    }
  if(verbose)
    cout << "Failed to park " << v->getDescription() << "." << endl;
  return false;
}

int lot::leave(vehicle *v, bool verbose) {
  spot *s;
  int retval;
  std::string description = v->getDescription();
  for(s = first; s != NULL; s = s->getNext()) {
    if((s->getVehicle() != NULL) && (v->getDescription().compare(s->getVehicle()->getDescription()) == 0)) {
      retval = s->leave();
      if(verbose) {
        if(retval)
          cout << description << " successfully left." << endl;
        else
          cout << description << " cannot leave: internal error." << endl;
      }
      return retval;
    }
  }
  if(verbose)
    cout << v->getDescription() << " not found in the parking lot." << endl;
  return false;
}

spot::spot() {
  v = NULL;
  next = NULL;
}

spot::~spot() {
  leave();
}

int spot::park(vehicle *newvehicle) {
  if (v == NULL) {
    v = newvehicle;
    return true;
  } else {
    return false;
  }
}

int spot::leave() {
  if (v != NULL) {
    v = NULL;
    return true;
  } else {
    return false;
  }
}

spot *spot::getNext() {
  return next;
}

void spot::setNext(spot *newNext) {
  next = newNext;
}

vehicle* spot::getVehicle() {
  return v;
}

vehicle::vehicle(std::string newdescription) {
  description = newdescription;
}

vehicle::~vehicle() {
}

std::string vehicle::getDescription() {
  return description;
}

int main() {
  lot *myLot = new lot(3);
  vehicle *v1 = new vehicle("Prius");
  vehicle *v2 = new vehicle("Insight");
  vehicle *v3 = new vehicle("Leaf");
  myLot->print();
  myLot->park(v1, true);
  myLot->print();
  myLot->park(v2, true);
  myLot->print();
  myLot->park(v3, true);
  myLot->print();
  myLot->leave(v2, true);
  myLot->print();
  myLot->leave(v3, true);
  myLot->print();
  myLot->leave(v1, true);
  myLot->print();
  myLot->leave(v2, true);
  myLot->print();
  delete myLot;
  myLot = new lot(1);
  myLot->print();
  myLot->park(v1, true);
  myLot->print();
  myLot->park(v2, true);
  myLot->print();
  myLot->leave(v2, true);
  myLot->print();
  myLot->leave(v1, true);
  myLot->print();
  delete myLot;
  myLot = new lot(0);
  myLot->print();
  myLot->park(v1, true);
  myLot->print();
  myLot->leave(v1, true);
  myLot->print();
  myLot->leave(v2, true);
  myLot->print();
  delete myLot;
  return 0;
}
