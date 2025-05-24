// big num lol
#include <stdio.h>
#include <stdlib.h>

typedef struct t_bigNum{
    unsigned char *n;
    unsigned int size;
    // sign byte knows if it number is positive or negative and if digit amount is even or odd
    // bit 1 is sign, bit 2 is digit even or odd, if 1 it is odd, so we substract 1 from size
    unsigned char sign;
    unsigned char *str;
}bigNum;

// TODO: Add function to calculate the amount of chars to assign to n depending on the number
void showBits(bigNum a);
void printDecimal(bigNum *a);
void printDebug(unsigned char* s, unsigned int size);
void contCheck(unsigned char *s, unsigned int size, unsigned int *c, bigNum a);
void to_bN(unsigned char *s, unsigned int size, bigNum *a);

bigNum add_bN(bigNum a, bigNum b);
bigNum sub_bN(bigNum a, bigNum b);
bigNum mult_bN(bigNum a, bigNum b);
bigNum div_bN(bigNum a, bigNum b);

int main(int argc, char *argv[])
{
	bigNum var, var2;
	unsigned char buffer[64];
	for (unsigned int i = 0; i < 64; i++)
	{
	    buffer[i] = '\0';
	}
	printf("Insert any number: ");
	scanf("%64s", buffer);
	to_bN(buffer, 64, &var);
	printDecimal(&var);
	for (unsigned int i = 0; i < 64; i++)
	{
	    buffer[i] = '\0';
	}
	printf("Insert any number: ");
	scanf("%64s", buffer);
	to_bN(buffer, 64, &var2);
	printDecimal(&var2);

	add_bN(var, var2);

	free(var.n);
	free(var2.n);
	free(var.str);
	free(var2.str);
	return 0;
}

void showBits(bigNum a)
{
    for(unsigned int i = a.size - 1; i > 0; i--)
    {
        for (unsigned int j = 7; j > 0; j--)
        {
            putchar(((a.n[i] >> j) & 1) + 48);
        }
        putchar((a.n[i] & 1) + 48);
        putchar(' ');
    }
    for (unsigned int j = 7; j > 0; j--)
    {
        putchar(((a.n[0] >> j) & 1) + 48);
    }
    putchar((a.n[0] & 1) + 48);
    putchar('\n');
}

void printDecimal(bigNum *a)
{
    unsigned int digits = (a->size << 1) - (a->sign >> 1) + (a->sign & 1), last = 0;
    unsigned char num[digits], spare[digits];
    for (unsigned int i = 0, j = a->size - 1; i < a->size && !last; i++, j--)
    {
        for (unsigned int k = 7; k > 0; k--)
        {
            if (a->n[j] >> k)
            {
                last = (j << 3) + k;
                break;
            }
        }
        if (a->n[j] && !last)
        {
            last = j << 3;
            break;
        }
    }
    for (unsigned int i = a->sign & 1; i < digits; i++)
    {
        num[i] = 0;
        spare[i] = 0;
    }
    if (a->sign & 1)
    {
        num[0] = '-';
        spare[0] = '-';
    }
    spare[digits - 1] = 1;
    if (a->n[0] & 1)
    {
        num[digits - 1] = 1;
    }
    for (unsigned int i = 1, d = a->sign & 1; i < last + 1; i++)
    {
        for (unsigned int j = a->sign & 1; j < digits; j++)
        {
            spare[j] = spare[j] << 1;
            if (spare[j] > 9)
            {
                spare[j - 1] += spare[j]/10;
                spare[j] %= 10;
            }
        }
        // suma
        for (unsigned int j = a->sign & 1; j < digits && a->n[i >> 3] & (1 << (i & 0x7)); j++)
        {
            num[j] += spare[j];
            if (num[j] > 9)
            {
                num[j - 1] += num[j]/10;
                num[j] %= 10;
            }
        }
    }
    // have to rewrite entirely, only works for 1 byte, better just create addition
    /*for (unsigned int i, j = 0; j < a->size; j++)
    {
        i = digits - 1;
        for (unsigned int k = 0; k < 8; k++)
        {
            if (num[i] && spare[i])
            {
                spare[i] = num[i];
            }
            if (a->n[j] & (1 << k))
            {
                num[i] += spare[i];
                spare[i] = 0;
            }
            num[i] += (a->n[j] & (1 << k)) % 10;
            num[i - 1] += (a->n[j] & (1 << k))/10;
            for (unsigned int p = i; num[p] > 9; p--)
            {
                num[p - 1] += num[p]/10;
                num[p] %= 10;
            }
        }
    }*/
    a->str = malloc(digits + 1);
    a->str[digits] = '\0';
    if (a->sign & 1)
    {
        putchar('-');
        a->str[0] = '0';
    }
    for (unsigned int i = a->sign & 1; i < digits; i++)
    {
        a->str[i] = num[i] + 48;
        printf("%d", num[i]);
    }
    putchar('\n');
}

void printDebug(unsigned char* s, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%d ", s[i]);
    }
    putchar('\n');
}

void contCheck(unsigned char *s, unsigned int size, unsigned int *c, bigNum a)
{
    // check if sum of all digits is bigger than 1
    // optimize later
    *c = 0;
    for (unsigned int i = a.sign & 1; i < size; i++)
    {
        if (size - i > 1 && s[i])
        {
            *c = 1;
            break;
        }
        if (size - i == 1 && s[i] > 1)
        {
            *c = 1;
        }
    }
}

void to_bN(unsigned char *ori, unsigned int size, bigNum *a)
{
    unsigned char s[size], d[size];
    unsigned int offset, c = 0;
    if (ori[0] == '-')
    {
        a->sign = 1;
    }
    else
    {
        a->sign = 0;
    }
    for (a->size = a->sign; a->size < size && ori[a->size] != '\0'; a->size++)
    {
        s[a->size] = ori[a->size] - 48;
        d[a->size] = s[a->size];
        c += s[a->size];
    }
    /* La cantidad de digitos se recorre en el patron de 4, 3, 3 hasta que llega a 100 digitos, donde se recorre a 3, 4, 3,
    y este patron define cuantos digitos en binario necesitas para que cantidad de digitos decimales.
    En otras palabras, puedo tener un contador donde se sumara en ese patron, y cada vez que alcanze 8
    sumara un char mas al tamaño, o mejor cada 2 digitos sumare 1 char por si acaso y despues optimizo.
    */
    size = a->size;
    // size = amount of decimal digits
    // a->size = amount of chars / bytes
    if (s[a->size] & 1)
    {
        s[a->size]--;
    }
    if (a->size & 1)
    {
        a->size++;
        a->sign |= 2;
    }
    a->size = a->size >> 1;
    a->n = (unsigned char*) calloc(a->size, 1);
    if (!c)
    {
        return;
    }
    while (c)
    {
        offset = 0;
        while (c)
        {
            contCheck(s, size, &c, *a);
            // division entre 2
            for (unsigned int i = a->sign & 1; i < size; i++)
            {
                if (s[i] & 1)
                {
                    s[i + 1] += 10;
                    s[i] = s[i] >> 1;
                }
                else
                {
                    s[i] = s[i] >> 1;
                }
            }
            offset++;
        }
        offset--;
        a->n[offset >> 3] |= 1 << (offset & 0x7);
        s[size - 1] = 1;
        // multiplicacion
        for (unsigned int i = 0; i < offset; i++)
        {
            for (unsigned int j = 0; j < size; j++)
            {
                s[j] = s[j] << 1;
                if (s[j] > 9)
                {
                    s[j - 1] += s[j]/10;
                    s[j] = s[j] % 10;
                }
            }
        }
        // resta
        for (unsigned int i = size - 1, j = a->sign & 1; j < size; --i, j++)
        {
            if (d[i] < s[i])
            {
                d[i] += 10 - s[i];
                for (unsigned int k = i - 1; j < size; k--, j++)
                {
                    if (d[k])
                    {
                        d[k]--;
                        for (unsigned int p = k + 1; p < i; p++)
                        {
                            d[p] = 9;
                        }
                        d[i - 1]++;
                        break;
                    }
                }
                d[i - 1]--;
            }
            else
            {
                d[i] -= s[i];
            }
        }
        contCheck(d, size, &c, *a);
        for (unsigned int i = 0; i < size; i++)
        {
            s[i] = d[i];
        }
    }
    a->n[0] |= s[size - 1];
    showBits(*a);
    /* Para pasar el numero a binario, deberia empezar por el numero completo y dividirlo entre 2 hasta llegar a 1, dependiendo de cuantas veces tome, el numero tendra su primer bit esa cantidad de espacios a la izquierda, y si la primer division que ocurre tiene un decimal, se le suma 1 a la cantidad de bits.
    o sea, si el primer digito es impar, sumare 1 al final y despues redondeare al numero mas chico, si no, solo seguire dividiendo entre 2, pero ya mañana.
    */
    // multiplicacion
    /* patron
    16 = 10 + 8 - 2
    32 = 20 + 16 - 4
    64 = 40 + 32 - 8
    128 = 80 + 64 - 16
    */
}

bigNum add_bN(bigNum a, bigNum b)
{
    unsigned int digits = (a.size << 1) - (a.sign >> 1) + (a.sign & 1),
                    digits2 = (b.size << 1) - (b.sign >> 1) + (b.sign & 1);
    bigNum c;
    // finish later so it returns bigNum c for better thingy
    for (unsigned int i = 0; i < digits; i++)
    {
        a.str[i] -= 48;
    }
    for (unsigned int i = 0; i < digits2; i++)
    {
        b.str[i] -= 48;
    }
    if (digits < digits2)
    {
        digits = digits2;
        c = a;
        a = b;
        b = c;
    }
    for (unsigned int j = a.sign & 1; j < digits; j++)
    {
        a.str[j] += b.str[j];
        if (a.str[j] > 9)
        {
            a.str[j - 1] += a.str[j]/10;
            a.str[j] %= 10;
        }
    }
    for (unsigned int i = 0; i < digits; i++)
    {
        a.str[i] += 48;
    }
    printf("res: %s\n", a.str);
}

bigNum sub_bN(bigNum a, bigNum b)
{
    unsigned int digits = (a.size << 1) - (a.sign >> 1) + (a.sign & 1);
}

bigNum mult_bN(bigNum a, bigNum b)
{
    unsigned int digits = (a.size << 1) - (a.sign >> 1) + (a.sign & 1);
}

bigNum div_bN(bigNum a, bigNum b)
{
    unsigned int digits = (a.size << 1) - (a.sign >> 1) + (a.sign & 1);
}
