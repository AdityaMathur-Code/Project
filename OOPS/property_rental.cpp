// Property Rental Management System
// OOP Lab Project
//Modules:
// 1. Property listings  (add, show, update rent, remove)
// 2. Search             (by rent range and location)
// 3. Tenants and agreements
// 4. Rent payments and dues
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Property {
private:
    int id;
    string type;       // Apartment, House or Shop
    string location;   // address with area and city
    int rent;          // monthly rent
    bool rented;       // true when an agreement is running

public:
    Property(int i, string t, string loc, int r) {
        id = i;
        type = t;
        location = loc;
        rent = r;
        rented = false;
    }

    int getId() { return id; }
    string getLocation() { return location; }
    int getRent() { return rent; }
    bool isRented() { return rented; }

    void setRent(int r) { rent = r; }
    void setRented(bool r) { rented = r; }

    void show() {
        cout << id << "\t" << type << "\t" << location << "\t" << rent << "\t";
        if (rented) cout << "RENTED";
        else cout << "AVAILABLE";
        cout << endl;
    }
};

// ---------------- Tenant class ----------------
class Tenant {
private:
    int id;
    string name;
    string phone;

public:
    Tenant(int i, string n, string p) {
        id = i;
        name = n;
        phone = p;
    }

    int getId() { return id; }
    string getName() { return name; }

    void show() {
        cout << id << "\t" << name << "\t" << phone << endl;
    }
};

// ---------------- Agreement class ----------------
class Agreement {
private:
    int id;
    int propertyId;
    int tenantId;
    int months;          // how long the agreement lasts
    int rent;            // rent copied from the property when the agreement is made
    int deposit;
    bool active;
    vector<int> paid;    // paid[0] = money paid for month 1, paid[1] for month 2, ...

public:
    Agreement(int i, int pId, int tId, int m, int r, int d) {
        id = i;
        propertyId = pId;
        tenantId = tId;
        months = m;
        rent = r;
        deposit = d;
        active = true;
        for (int k = 0; k < m; k++)
            paid.push_back(0);
    }

    int getId() { return id; }
    int getPropertyId() { return propertyId; }
    int getTenantId() { return tenantId; }
    int getMonths() { return months; }
    int getRent() { return rent; }
    int getDeposit() { return deposit; }
    bool isActive() { return active; }
    int getPaid(int month) { return paid[month - 1]; }

    void addPayment(int month, int amount) {
        paid[month - 1] = paid[month - 1] + amount;
    }

    void endEarly(int lastMonth) {
        months = lastMonth;
        active = false;
    }
    int outstanding(int upTo) {
        int last = upTo;
        if (last > months) last = months;

        int total = 0;
        for (int m = 1; m <= last; m++)
            total = total + (rent - paid[m - 1]);
        return total;
    }
};

// ---------------- Global lists ----------------
vector<Property> properties;
vector<Tenant> tenants;
vector<Agreement> agreements;

int nextPropertyId = 1;
int nextTenantId = 1;
int nextAgreementId = 1;

// ---------------- Helper functions ----------------
// Each one returns the position of the record in its vector,
// or -1 if no record has that ID.
int findProperty(int id) {
    for (int i = 0; i < (int)properties.size(); i++)
        if (properties[i].getId() == id) return i;
    return -1;
}

int findTenant(int id) {
    for (int i = 0; i < (int)tenants.size(); i++)
        if (tenants[i].getId() == id) return i;
    return -1;
}

int findAgreement(int id) {
    for (int i = 0; i < (int)agreements.size(); i++)
        if (agreements[i].getId() == id) return i;
    return -1;
}

// ---------------- Main program ----------------
int main() {
    int choice;

    while (true) {
        cout << "\n===== PROPERTY RENTAL SYSTEM =====\n";
        cout << "1.  Add property\n";
        cout << "2.  Show all properties\n";
        cout << "3.  Update rent\n";
        cout << "4.  Remove property\n";
        cout << "5.  Search properties\n";
        cout << "6.  Add tenant\n";
        cout << "7.  Show all tenants\n";
        cout << "8.  Create rental agreement\n";
        cout << "9.  Show all agreements\n";
        cout << "10. End agreement early\n";
        cout << "11. Record rent payment\n";
        cout << "12. Payment history of an agreement\n";
        cout << "13. Rent schedule of an agreement\n";
        cout << "14. Outstanding dues report\n";
        cout << "0.  Exit\n";
        cout << "Enter choice: ";

        cin >> choice;
        if (cin.fail()) {
            if (cin.eof()) break;              // no more input
            cout << "Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');                // remove the Enter key

        switch (choice) {
        case 0:
            cout << "Bye!\n";
            return 0;

        case 1: {
            string type, location;
            int rent;
            cout << "Type (Apartment / House / Shop): ";
            getline(cin, type);
            cout << "Address (with area and city): ";
            getline(cin, location);
            cout << "Monthly rent: ";
            cin >> rent;

            if (rent <= 0) {
                cout << "Rent must be more than 0.\n";
                break;
            }
            properties.push_back(Property(nextPropertyId, type, location, rent));
            cout << "Property added. Property ID = " << nextPropertyId << endl;
            nextPropertyId++;
            break;
        }

        case 2:
            cout << "ID\tType\tLocation\tRent\tStatus\n";
            for (int i = 0; i < (int)properties.size(); i++)
                properties[i].show();
            break;

        case 3: {
            int id, newRent;
            cout << "Property ID: ";
            cin >> id;
            cout << "New monthly rent: ";
            cin >> newRent;

            int p = findProperty(id);
            if (p == -1)
                cout << "Property not found.\n";
            else if (newRent <= 0)
                cout << "Rent must be more than 0.\n";
            else {
                properties[p].setRent(newRent);
                cout << "Rent updated.\n";
            }
            break;
        }

        case 4: {
            int id;
            cout << "Property ID: ";
            cin >> id;

            int p = findProperty(id);
            if (p == -1)
                cout << "Property not found.\n";
            else if (properties[p].isRented())
                cout << "This property is rented. End its agreement first.\n";
            else {
                properties.erase(properties.begin() + p);
                cout << "Property removed.\n";
            }
            break;
        }

        case 5: {
            int minRent, maxRent, found = 0;
            string place;
            cout << "Minimum rent (0 for no minimum): ";
            cin >> minRent;
            cout << "Maximum rent (0 for no maximum): ";
            cin >> maxRent;
            cin.ignore(1000, '\n');
            cout << "Location (leave blank for any): ";
            getline(cin, place);

            cout << "ID\tType\tLocation\tRent\tStatus\n";
            for (int i = 0; i < (int)properties.size(); i++) {
                if (properties[i].isRented()) continue;
                if (properties[i].getRent() < minRent) continue;
                if (maxRent > 0 && properties[i].getRent() > maxRent) continue;
                if (place != "" && properties[i].getLocation().find(place) == string::npos)
                    continue;

                properties[i].show();
                found++;
            }
            if (found == 0)
                cout << "No available properties match.\n";
            else
                cout << found << " property(ies) found.\n";
            break;
        }

        case 6: {
            string name, phone;
            cout << "Tenant name: ";
            getline(cin, name);
            cout << "Phone number: ";
            getline(cin, phone);

            if (name == "") {
                cout << "Name cannot be empty.\n";
                break;
            }
            tenants.push_back(Tenant(nextTenantId, name, phone));
            cout << "Tenant added. Tenant ID = " << nextTenantId << endl;
            nextTenantId++;
            break;
        }

        case 7:
            if (tenants.size() == 0) cout << "No tenants yet.\n";
            cout << "ID\tName\tPhone\n";
            for (int i = 0; i < (int)tenants.size(); i++)
                tenants[i].show();
            break;

        case 8: {
            int propId, tId, months, deposit;
            cout << "Property ID: ";
            cin >> propId;
            cout << "Tenant ID: ";
            cin >> tId;
            cout << "Number of months: ";
            cin >> months;
            cout << "Security deposit: ";
            cin >> deposit;

            int p = findProperty(propId);
            int t = findTenant(tId);
            if (p == -1)
                cout << "Property not found.\n";
            else if (t == -1)
                cout << "Tenant not found.\n";
            else if (properties[p].isRented())
                cout << "This property is already rented.\n";
            else if (months <= 0)
                cout << "Number of months must be more than 0.\n";
            else {
                agreements.push_back(Agreement(nextAgreementId, propId, tId, months,
                                               properties[p].getRent(), deposit));
                properties[p].setRented(true);
                cout << "Agreement created. Agreement ID = " << nextAgreementId << endl;
                nextAgreementId++;
            }
            break;
        }

        case 9:
            if (agreements.size() == 0) cout << "No agreements yet.\n";
            for (int i = 0; i < (int)agreements.size(); i++) {
                int t = findTenant(agreements[i].getTenantId());
                cout << "Agreement " << agreements[i].getId()
                     << " | Property " << agreements[i].getPropertyId()
                     << " | Tenant " << tenants[t].getName()
                     << " | Months " << agreements[i].getMonths()
                     << " | Rent " << agreements[i].getRent()
                     << " | Deposit " << agreements[i].getDeposit();
                if (agreements[i].isActive()) cout << " | ACTIVE\n";
                else cout << " | ENDED\n";
            }
            break;

        case 10: {
            int id, lastMonth;
            cout << "Agreement ID: ";
            cin >> id;
            cout << "Last month to pay for (month number): ";
            cin >> lastMonth;

            int a = findAgreement(id);
            if (a == -1)
                cout << "Agreement not found.\n";
            else if (!agreements[a].isActive())
                cout << "This agreement has already ended.\n";
            else if (lastMonth < 1 || lastMonth > agreements[a].getMonths())
                cout << "Month must be between 1 and " << agreements[a].getMonths() << ".\n";
            else {
                agreements[a].endEarly(lastMonth);
                int p = findProperty(agreements[a].getPropertyId());
                properties[p].setRented(false);
                cout << "Agreement ended. The property is available again.\n";
            }
            break;
        }

        case 11: {
            int id, month, amount;
            cout << "Agreement ID: ";
            cin >> id;
            cout << "Month number (1 = first month): ";
            cin >> month;
            cout << "Amount paid: ";
            cin >> amount;

            int a = findAgreement(id);
            if (a == -1)
                cout << "Agreement not found.\n";
            else if (month < 1 || month > agreements[a].getMonths())
                cout << "That month is not part of this agreement.\n";
            else if (amount <= 0)
                cout << "Amount must be more than 0.\n";
            else {
                int balance = agreements[a].getRent() - agreements[a].getPaid(month);
                if (balance <= 0)
                    cout << "Rent for that month is already paid.\n";
                else if (amount > balance)
                    cout << "Too much. Only " << balance << " is due for that month.\n";
                else {
                    agreements[a].addPayment(month, amount);
                    cout << "Payment saved. Still to pay for month " << month
                         << ": " << balance - amount << endl;
                }
            }
            break;
        }

        case 12: {
            int id;
            cout << "Agreement ID: ";
            cin >> id;

            int a = findAgreement(id);
            if (a == -1) {
                cout << "Agreement not found.\n";
                break;
            }
            for (int m = 1; m <= agreements[a].getMonths(); m++) {
                if (agreements[a].getPaid(m) > 0)
                    cout << "Month " << m << ": paid " << agreements[a].getPaid(m) << endl;
            }
            break;
        }

        case 13: {
            int id, now;
            cout << "Agreement ID: ";
            cin >> id;
            cout << "Show up to month number: ";
            cin >> now;

            int a = findAgreement(id);
            if (a == -1) {
                cout << "Agreement not found.\n";
                break;
            }

            int last = now;
            if (last > agreements[a].getMonths()) last = agreements[a].getMonths();

            cout << "Month\tRent\tPaid\tStatus\n";
            for (int m = 1; m <= last; m++) {
                int paid = agreements[a].getPaid(m);
                cout << m << "\t" << agreements[a].getRent() << "\t" << paid << "\t";
                if (paid >= agreements[a].getRent()) cout << "PAID\n";
                else if (paid > 0) cout << "PARTIAL\n";
                else cout << "UNPAID\n";
            }
            cout << "Total outstanding: " << agreements[a].outstanding(now) << endl;
            break;
        }

        case 14: {
            int now, total = 0, count = 0;
            cout << "Show dues up to month number: ";
            cin >> now;

            cout << "Agreement\tTenant\tProperty\tDue\n";
            for (int i = 0; i < (int)agreements.size(); i++) {
                int due = agreements[i].outstanding(now);
                if (due <= 0) continue;

                int t = findTenant(agreements[i].getTenantId());
                cout << agreements[i].getId() << "\t" << tenants[t].getName()
                     << "\t" << agreements[i].getPropertyId() << "\t" << due << endl;
                total = total + due;
                count++;
            }

            if (count == 0)
                cout << "No dues. Everyone is paid up.\n";
            else
                cout << "Total outstanding: " << total << endl;
            break;
        }

        default:
            cout << "That is not a valid choice.\n";
        }
    }

    return 0;
}
