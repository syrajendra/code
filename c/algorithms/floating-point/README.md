    $  gcc -o sp sp.c -lm
    $ ./sp 12.375
    Single precision floating point number 12.375000 using excess-127 format

    My calculation:
    Decimal part is 12 to binary = 00000000000000000000000000001100
    0.375000 x 2 = 0.750000 = 0 + 0.750000 =>  0
    0.750000 x 2 = 1.500000 = 1 + 0.500000 =>  1
    0.500000 x 2 = 1.000000 = 1 + 0.000000 =>  1
    Fraction part is 0.375000 to binary = 011
    Normalization of decimal part : exponent = 3 binary left = 100

    Exponent : 8 bits [23-30]
     Dec = 127 + 3 = 130
     Hex = 0x82
     Bin = 10000010

    Mantisa : 23 bits [0-22]
     Dec = 4587520
     Hex = 0x460000
     Bin = 10001100000000000000000
    IEEE 754 binary 32 format representation of 12.375000 = 0-10000010-10001100000000000000000

    Actual:
    Sign : 1 bit
     Bit[31] = 0 <+ve number>

    Exponent : 8 bits [23-30]
     Dec = 130
     Hex = 0x82
     Bin = 10000010

    Mantisa : 23 bits [0-22]
     Dec = 4587520
     Hex = 0x460000
     Bin = 10001100000000000000000
    IEEE 754 binary 32 format representation of 12.375000 = 0-10000010-10001100000000000000000

    Convert Binary 11000001010001100000000000000000 to decimal
    Decimal:  12.375000

    $ ./sp 0.25
    Single precision floating point number 0.250000 using excess-127 format

    My calculation:
    Decimal part is 0 to binary = 00000000000000000000000000000000
    0.250000 x 2 = 0.500000 = 0 + 0.500000 =>  0
    0.500000 x 2 = 1.000000 = 1 + 0.000000 =>  1
    Fraction part is 0.250000 to binary = 01
    Denormalization of fractional part : exponent = -2 binary left =

    Exponent : 8 bits [23-30]
     Dec = 127 + -2 = 125
     Hex = 0x7d
     Bin = 01111101

    Mantisa : 23 bits [0-22]
     Dec = 0
     Hex = 0x0
     Bin = 00000000000000000000000
    IEEE 754 binary 32 format representation of 0.250000 = 0-01111101-00000000000000000000000

    Actual:
    Sign : 1 bit
     Bit[31] = 0 <+ve number>

    Exponent : 8 bits [23-30]
     Dec = 125
     Hex = 0x7d
     Bin = 01111101

    Mantisa : 23 bits [0-22]
     Dec = 0
     Hex = 0x0
     Bin = 00000000000000000000000
    IEEE 754 binary 32 format representation of 0.250000 = 0-01111101-00000000000000000000000

    Convert Binary 00111110100000000000000000000000 to decimal
    Decimal:  0.250000

    $ ./sp 0.1
    Single precision floating point number 0.100000 using excess-127 format

    My calculation:
    Decimal part is 0 to binary = 00000000000000000000000000000000
    0.100000 x 2 = 0.200000 = 0 + 0.200000 =>  0
    0.200000 x 2 = 0.400000 = 0 + 0.400000 =>  0
    0.400000 x 2 = 0.800000 = 0 + 0.800000 =>  0
    0.800000 x 2 = 1.600000 = 1 + 0.600000 =>  1
    0.600000 x 2 = 1.200000 = 1 + 0.200000 =>  1
    0.200000 x 2 = 0.400000 = 0 + 0.400000 =>  0
    0.400000 x 2 = 0.800000 = 0 + 0.800000 =>  0
    0.800000 x 2 = 1.600000 = 1 + 0.600000 =>  1
    0.600000 x 2 = 1.200001 = 1 + 0.200001 =>  1
    0.200001 x 2 = 0.400002 = 0 + 0.400002 =>  0
    0.400002 x 2 = 0.800003 = 0 + 0.800003 =>  0
    0.800003 x 2 = 1.600006 = 1 + 0.600006 =>  1
    0.600006 x 2 = 1.200012 = 1 + 0.200012 =>  1
    0.200012 x 2 = 0.400024 = 0 + 0.400024 =>  0
    0.400024 x 2 = 0.800049 = 0 + 0.800049 =>  0
    0.800049 x 2 = 1.600098 = 1 + 0.600098 =>  1
    0.600098 x 2 = 1.200195 = 1 + 0.200195 =>  1
    0.200195 x 2 = 0.400391 = 0 + 0.400391 =>  0
    0.400391 x 2 = 0.800781 = 0 + 0.800781 =>  0
    0.800781 x 2 = 1.601562 = 1 + 0.601562 =>  1
    0.601562 x 2 = 1.203125 = 1 + 0.203125 =>  1
    0.203125 x 2 = 0.406250 = 0 + 0.406250 =>  0
    0.406250 x 2 = 0.812500 = 0 + 0.812500 =>  0
    0.812500 x 2 = 1.625000 = 1 + 0.625000 =>  1
    0.625000 x 2 = 1.250000 = 1 + 0.250000 =>  1
    0.250000 x 2 = 0.500000 = 0 + 0.500000 =>  0
    0.500000 x 2 = 1.000000 = 1 + 0.000000 =>  1
    Fraction part is 0.100000 to binary = 000110011001100110011001101
    Denormalization of fractional part : exponent = -4 binary left = 10011001100110011001101

    Exponent : 8 bits [23-30]
     Dec = 127 + -4 = 123
     Hex = 0x7b
     Bin = 01111011

    Mantisa : 23 bits [0-22]
     Dec = 5033165
     Hex = 0x4ccccd
     Bin = 10011001100110011001101
    IEEE 754 binary 32 format representation of 0.100000 = 0-01111011-10011001100110011001101

    Actual:
    Sign : 1 bit
     Bit[31] = 0 <+ve number>

    Exponent : 8 bits [23-30]
     Dec = 123
     Hex = 0x7b
     Bin = 01111011

    Mantisa : 23 bits [0-22]
     Dec = 5033165
     Hex = 0x4ccccd
     Bin = 10011001100110011001101
    IEEE 754 binary 32 format representation of 0.100000 = 0-01111011-10011001100110011001101

    Convert Binary 00111101110011001100110011001101 to decimal
    Decimal:  0.10000

