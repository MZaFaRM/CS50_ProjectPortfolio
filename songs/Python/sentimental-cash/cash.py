import cs50


coins = 0


def main():

    dollars = get_dollar("Change owed: ")

    # Converts dollars to cents
    cents = dollars * 100

    # returns number of quarters
    quarter = get_quarter(cents)
    cents = cents - (quarter * 25)

    # returns number of dimes
    dime = get_dime(cents)
    cents = cents - (dime * 10)

    # returns number of nickels
    nickel = get_nickel(cents)
    cents = cents - (nickel * 5)

    # returns number of pennies
    penny = get_penny(cents)
    cents = cents - (1 * penny)

    # total coins
    coins = int(penny + nickel + dime + quarter)

    print(coins)


# returns required pennies to get the input value
def get_penny(value):

    coins = int(value / 1)

    return coins


# returns required nickels to get the input value
def get_nickel(value):

    coins = int(value / 5)

    return coins


# returns required dimes to get the input value
def get_dime(value):

    coins = int(value / 10)

    return coins


# returns required quarters to get the input value
def get_quarter(value):

    coins = int(value / 25)

    return coins


# A variation of the get_float function
def get_dollar(request):
    while True:
        value = cs50.get_float(f"{request}")
        if value >= 0:
            break
    return value


if __name__ == '__main__':
    main()