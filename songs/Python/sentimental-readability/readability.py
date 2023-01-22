# Gets user input in lower case
text = input("text: ").lower()

# words count starts from 0 because there is atleast one word
# Hence this ensures reading last word
words = 1
sentences = 0
letters = 0


# loops through the text
for char in text:

    # Letters
    if char >= 'a' and char <= 'z':

        letters += 1

    # Word end
    elif char == ' ':

        words += 1

    # Sentence end
    elif char == '!' or char == '?' or char == '.':

        sentences += 1

# Calculates L and S
L = letters / words * 100
S = sentences / words * 100

# Coleman-Liau formula
index = 0.0588 * L - 0.296 * S - 15.8
index = round(index)

# Scenarios
if index < 1:

    print("Before Grade 1")

elif index >= 16:

    print("Grade 16+")

else:

    print(f"Grade {index}")