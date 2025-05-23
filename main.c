// big num lol
#include <stdio.h>
#include <stdlib.h>

typedef struct t_bigNum{
    unsigned char *n;
    unsigned int size;
    unsigned char sign;
}bigNum;

// TODO: Add function to calculate the amount of chars to assign to n depending on the number
void showBits(bigNum a);
void printDecimal(bigNum a);
void to_bN(unsigned char *s, unsigned int size, bigNum *a);

int main(int argc, char *argv[])
{
	unsigned long long a = 0;
	bigNum var;
	unsigned char buffer[64];
	for (unsigned int i = 0; i < 64; i++)
	{
	    buffer[i] = '\0';
	}
	//printf("Insert size: ");
	//scanf("%d", &var.size);
	//var.n = (unsigned char*) malloc(var.size);
	printf("Insert any number: ");
	scanf("%64s", buffer);
	to_bN(buffer, 64, &var);

	free(var.n);
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

void printDecimal(bigNum a)
{

}

void printDebug(unsigned char* s, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%d", s[i]);
    }
    putchar('\n');
}

void contCheck(unsigned char *s, unsigned int size, unsigned int *c, bigNum a)
{
    // check if sum of all digits is bigger than 1
    // optimize later
    *c = 0;
    for (unsigned int i = a.sign; i < size; i++)
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
            for (unsigned int i = a->sign; i < size; i++)
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
            //getchar();
            printDebug(s, size);
            printf("Current offset: %d\n", offset);
            //getchar();
        }
        offset--;
        a->n[offset >> 3] |= 1 << (offset & 0x7);
        s[size - 1] = 1;
        printf("decimal digits: %d\n", size);
        showBits(*a);
        // multiplicacion
        for (unsigned int i = 0; i < offset; i++)
        {
            for (unsigned int curr = size - 1, j = a->sign; j < size; curr--, j++)
            {
                if (s[curr] << 1 > 9)
                {
                    s[curr] = s[curr] << 1;
                    s[curr - 1] = s[curr - 1] << 1;
                    s[curr - 1] += s[curr]/10;
                    // fix the fact that this skips one full doubling and breaks because of it
                    for (unsigned int curr2 = curr - 1; j < size; curr2--, j++)
                    {
                        if (s[curr2] > 9)
                        {
                            s[curr2 - 1] = s[curr2 - 1] << 1;
                            s[curr2 - 1] += s[curr2]/10;
                            s[curr2] = s[curr2] % 10;
                            j++;
                            curr2--;
                        }
                        else
                        {
                            break;
                        }
                    }
                    s[curr] = s[curr] % 10;
                    curr--;
                    j++;
                }
                else
                {
                    s[curr] = s[curr] << 1;
                }
            }
            for (unsigned int k = 0; k < size; k++)
            {
                printf("%d ", s[k]);
            }
            putchar('\n');
            //printDebug(s, size);
        }
        // resta
        printf("resta\nd: ");
        printDebug(d, size);
        printf("s: ");
        printDebug(s, size);
        for (unsigned int i = size - 1, j = a->sign; j < size; --i, j++)
        {
            if (d[i] < s[i])
            {
                d[i] += 10 - s[i];
                d[i - 1]--;
            }
            else
            {
                d[i] -= s[i];
            }
            printDebug(d, size);
        }
        contCheck(d, size, &c, *a);
        for (unsigned int i = 0; i < size; i++)
        {
            s[i] = d[i];
        }
        printf("next, c: %d\n", c);
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
