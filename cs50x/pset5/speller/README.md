# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

An invented long word said to mean a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?

It returns resource usage statistics for processes.

## Per that same man page, how many members are in a variable of type `struct rusage`?

16.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

So we don't have to make a copy of `before` and `after` each time we call `calculate`.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

`main` begins by checking the number of arguments and determines which dictionary to be used for spellchecking.

Once the file is successfully opened, the `for` loop goes through every character in the file with `fgetc()`, 
which returns the character read as an unsigned char cast to an int. 
The returned character is used as the index for the `for` loop, since `fgetc()` advances the position indicator for the file stream. The loop will terminate if `fgetc()` returns an `EOF`.

In the first `if` statement, each character is checked if it's an alphabetical character or an apostrophe.
If so, it'll be added to a character array initialised with the size of `LENGTH + 1`; 
`LENGTH` being the number of letters in the longest possible word and `+1` for the `NUL` character.
If the word is longer than `LENGTH`, we will quickly read the rest of the string with a `while` loop to advance the position indicator. We ignore the word by setting `index` to 0 and start all over again for the next string.

The first `else` statement simply checks for any numerical character in the string. If there is one, we will quickly read the rest of the string with a `while` loop to advance the position indicator. We will also ignore the word by setting `index` to 0 and start over again to read the next string.

Finally, the last `else` statement will tell us that a "valid" word has ended when `fgetc()` does not read an alphabetical; apostrophe or numerical character. In other words, we read a whitespace character. We will append the `NUL` character to the character array, then update the word counter by incrementing it.

Once the `for` loop terminates, we will close the file with `fclose()`, print the benchmark and timing results, then return `0` to end the program.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

Because `fscanf` reads whole strings at a time, using it would mean having to create variables to store every string in memory. Also, we'd never know the maximum length of any string that could be read, making it difficult to allocate memory.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Because we're passing pointers as parameters, we want to avoid changing the value at those addresses by declaring them as `const`.
