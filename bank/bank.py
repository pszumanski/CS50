def main():
    greeting = input("Greeting: ")
    greeting = greeting.lower()
    greeting = greeting.strip()
    if greeting.startswith("hello"):
        dollars = 0
    elif greeting.startswith('h'):
        dollars = 20
    else:
        dollars = 100
    print(f"${dollars}")


main()