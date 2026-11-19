# Property Rental Management System

A console program written in C++ for the OOP lab. It keeps track of rental properties, tenants, rental agreements and the rent they pay.

The whole program is in one file: `rental_basic.cpp`.

## What it can do

1. Add a property, show all properties, change the rent, remove a property
2. Search properties by rent range and location
3. Add tenants, create a rental agreement, show agreements, end an agreement early
4. Record rent payments, see payment history, see the rent schedule and outstanding dues

## How to compile and run

You need any C++ compiler (g++ is fine).

```
g++ rental_basic.cpp -o rental
./rental
```

On Windows with MinGW:

```
g++ rental_basic.cpp -o rental.exe
rental.exe
```

## How the program is organised

There are three classes:

- **Property**: id, type, location, rent, and whether it is rented
- **Tenant**: id, name, phone number
- **Agreement**: which property and tenant, how many months, the rent, the deposit, and how much has been paid for each month

The records are kept in three `vector`s (`properties`, `tenants`, `agreements`). There are also three small helper functions (`findProperty`, `findTenant`, `findAgreement`) that return the position of a record, or -1 if the ID does not exist. Everything else is done in `main()` using a menu with a `switch`.

## Things to know before you use it

- **Months are numbers.** Month 1 is the first month of the agreement, month 2 is the second, and so on. You type the month number, not a date.
- **Money is in whole numbers** (no paise or decimals).
- **Location search is case-sensitive.** If you added a property with "Kothrud", search for "Kothrud".
- **The "Show dues up to month number" question** asks how many months have passed. For example, if the agreement is in its third month, type 3.
- **Nothing is saved.** When the program closes, all data is lost.

## Example session

Add a property (option 1) with the rent 14000:

```
Enter choice: 1
Type (Apartment / House / Shop): Apartment
Address (with area and city): 12 Park Street, Kothrud, Pune
Monthly rent: 14000
Property added. Property ID = 1
```

Add a tenant (option 6), then create an agreement for 12 months with a deposit of 28000 (option 8):

```
Enter choice: 8
Property ID: 1
Tenant ID: 1
Number of months: 12
Security deposit: 28000
Agreement created. Agreement ID = 1
```

Record payments (option 11). Month 1 is paid in full, and month 2 gets 5000:

```
Enter choice: 11
Agreement ID: 1
Month number (1 = first month): 1
Amount paid: 14000
Payment saved. Still to pay for month 1: 0

Enter choice: 11
Agreement ID: 1
Month number (1 = first month): 2
Amount paid: 5000
Payment saved. Still to pay for month 2: 9000
```

Check the rent schedule up to month 3 (option 13):

```
Enter choice: 13
Agreement ID: 1
Show up to month number: 3
Month	Rent	Paid	Status
1	14000	14000	PAID
2	14000	5000	PARTIAL
3	14000	0	UNPAID
Total outstanding: 23000
```

## Files

- `property_rental.cpp`: the program
- `README.md`: this file
