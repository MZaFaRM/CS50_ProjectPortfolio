from cs50 import get_int


def main():

    digits = sum_of_product = sum = 0

    number = get_int("Number: ")

    temp = temp1 = temp2 = number

    while temp != 0:

        # removes last digit
        temp = temp // 10

        # stores second last digit
        rem = temp % 10

        # multiplies it by 2
        product = 2 * rem

        # removes the second last digit
        temp = temp // 10

        # if the product is two digit
        if product >= 10:

            # adds 1 and the other digit to sum_of_product
            # 1 because 18 is the highest possible product
            rem = product % 10
            sum_of_product = rem + sum_of_product + 1

        else:

            # product is one digit
            sum_of_product = sum_of_product + product

        # repeats the loop

    while temp1 != 0:

        # adding the other digits
        rem = temp1 % 10
        temp1 = temp1 // 100
        sum = rem + sum

    Total_sum = sum + sum_of_product
    result = Total_sum % 10

    while temp2 > 100:

        temp2 = temp2 // 10

    if result == 0:

        # finds the number of digits
        while number != 0:

            digits += 1
            number = number // 10

        # Checks for AMEX
        if (temp2 == 34 or temp2 == 37) and digits == 15:

            print("AMEX")

        # Checks for VISA
        elif (digits == 16 or digits == 13) and temp2 // 10 == 4:

            print("VISA")

        # Checks for MASTERCARD
        elif temp2 <= 55 and temp2 >= 51 and digits == 16:

            print("MASTERCARD")

        # If none prints INVALID
        else:

            print("INVALID")

    # if result != 0
    else:

        print("INVALID")


if __name__ == "__main__":
    main()