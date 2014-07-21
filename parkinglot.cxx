/* An object-oriented parking lot. */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class lot;
class spot;
class vehicle;

class lot {
  public:
    lot(int n);
    ~lot();
    void print();
    int park(vehicle*);
    int leave(vehicle*);
    void setVerbose(bool verbose) {
      verbose_ = verbose;
    }
  private:
    vector<spot> spots;
    bool verbose_;
};

class spot {
  public:
    spot() : v(NULL) {}
    ~spot() { leave(); }
    int park(vehicle*);
    int leave();
    vehicle* getVehicle() { return v; }
  private:
    vehicle *v;
};

class vehicle {
  public:
    vehicle(std::string newdescription) : description(newdescription) {}
    ~vehicle() {}
    std::string getDescription() { return description; }
  private:
    std::string description;
};

lot::lot(int n) : spots(n, spot()) {}

lot::~lot() {
  for (vector<spot>::iterator it = spots.begin(); it != spots.end(); ++it) {
    it->leave();
  }
}

void lot::print() {
  if (spots.size() == 0) {
    cout << "Zero sized lot.";
  } else {
    for (int i = 0 ; i < spots.size() ; ++i) {
      if (spots[i].getVehicle() == NULL)
        cout << "Spot " << i << " empty; ";
      else
        cout << "Spot " << i << " has " << spots[i].getVehicle()->getDescription() << "; ";
    }
  }
  cout << endl;
}

int lot::park(vehicle *v) {
  for (vector<spot>::iterator it = spots.begin(); it != spots.end(); ++it) {
    if (it->park(v)) {
      if(verbose_)
        cout << "Successfully parked " << v->getDescription() << "." << endl;
      return true;
    }
  }
  if(verbose_)
    cout << "Failed to park " << v->getDescription() << "." << endl;
  return false;
}

int lot::leave(vehicle *v) {
  int retval;
  std::string description = v->getDescription();
  for (vector<spot>::iterator it = spots.begin(); it != spots.end(); ++it) {
    if((it->getVehicle() != NULL) && (v->getDescription().compare(it->getVehicle()->getDescription()) == 0)) {
      retval = it->leave();
      if(verbose_) {
        if(retval)
          cout << description << " successfully left." << endl;
        else
          cout << description << " cannot leave: internal error." << endl;
      }
      return retval;
    }
  }
  if(verbose_)
    cout << v->getDescription() << " not found in the parking lot." << endl;
  return false;
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

int main() {
  lot *myLot = new lot(3);
  myLot->setVerbose(true);
  vehicle *v1 = new vehicle("Prius");
  vehicle *v2 = new vehicle("Insight");
  vehicle *v3 = new vehicle("Leaf");
  myLot->print();
  myLot->park(v1);
  myLot->print();
  myLot->park(v2);
  myLot->print();
  myLot->park(v3);
  myLot->print();
  myLot->leave(v2);
  myLot->print();
  myLot->leave(v3);
  myLot->print();
  myLot->leave(v1);
  myLot->print();
  myLot->leave(v2);
  myLot->print();
  delete myLot;
  myLot = new lot(1);
  myLot->print();
  myLot->park(v1);
  myLot->print();
  myLot->park(v2);
  myLot->print();
  myLot->leave(v2);
  myLot->print();
  myLot->leave(v1);
  myLot->print();
  delete myLot;
  myLot = new lot(0);
  myLot->print();
  myLot->park(v1);
  myLot->print();
  myLot->leave(v1);
  myLot->print();
  myLot->leave(v2);
  myLot->print();
  delete myLot;
  return 0;
}
