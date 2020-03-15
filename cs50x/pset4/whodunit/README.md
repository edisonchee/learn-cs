# Questions

## What's `stdint.h`?

It's a header file that implements additional integer types.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

So we can define our own data structures.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

A BYTE is 1 byte, DWORD is 4 bytes, LONG is 4 bytes and WORD is 2 bytes.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

In Hexadecimal, 0x42 and 0x4d.

## What's the difference between `bfSize` and `biSize`?

`biSize` is the required size in bytes of the file header structure only, while `bfSize` is the size in bytes of the entire bitmap file.

## What does it mean if `biHeight` is negative?

It means that the first pixel described by the file begins at the top-left.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

The program might be ran without any arguments supplied.

## Why is the third argument to `fread` always `1` in our code?

The third argument to the `fread` function refers to the number of elements in the data stream to be read. In our code, `fread` is used to read both BITMAPFILEHEADER and BITMAPINFOHEADER separately. Since they occur only once in a bitmap file, the third argument to `fread` will always be 1.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3.

## What does `fseek` do?

It sets the file position of the data stream according to the given offset.

## What is `SEEK_CUR`?

It refers to the current position of the file pointer.

## Whodunit?

It was Professor Plum with the candlestick in the library.
