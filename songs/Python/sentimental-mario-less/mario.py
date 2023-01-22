import cs50


# main function
def main():
    height = get_int("Height: ", 1, 8)
    half_pyramid(height)


# A variation of the get_int function
def get_int(request, min, max):
    while True:
        value = cs50.get_int(f"{request}")
        if value <= max and value >= min:
            break
    return value


# makes a half_pyramid based on input
def half_pyramid(value):
    for i in range(1, value + 1):
        print(" " * (value - i) + "#" * i)


if __name__ == "__main__":
    main()