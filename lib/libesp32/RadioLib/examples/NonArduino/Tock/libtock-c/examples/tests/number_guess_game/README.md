Number Guessing Game
===========

Load this Tock app to play a thrilling game of guess the number!

Example Output
--------------

```
Welcome to the number guessing game!

OK I have selected a random number between 0 and 65535
Enter your guesses, and I will let you know when you are correct!
Enter a number: 50000
Not quite. Please choose a lower number.
Enter a number: 6000
Nope. You are too low
Enter a number: 40000
Not quite. Please choose a lower number.
Enter a number: 30000
Not quite. Please choose a lower number.
Enter a number: 20000
Nope. You are too low
Enter a number: 25000
Nope. You are too low
Enter a number: 28000
Nope. You are too low
Enter a number: 29000
Not quite. Please choose a lower number.
Enter a number: 28500
Nope. You are too low
Enter a number: 28700
Nope. You are too low
Enter a number: 28900
Nope. You are too low
Enter a number: 28950
Not quite. Please choose a lower number.
Enter a number: 28925
Hooray! You guessed my number.
You are very good at this.
Thank you for playing the number guessing game.
```

Notes
-----

Receiving on the console with multiple apps is currently undefined behavior, so
this app should be run on its own.
