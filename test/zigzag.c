#include <stdbool.h>
#include <stdio.h>

void zigzag(int w, int h, char z)
{
    int pos = 0;
    bool isUp = true;
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (j == pos)
            {
                printf("%c ", z);
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
        if (pos == w - 1)
        {
            isUp = false;
        }
        else if (pos == 0)
        {
            isUp = true;
        }
        pos = isUp ? pos + 1 : pos - 1;
    }
}

int main(void)
{
    zigzag(3, 7, 'z');
    zigzag(4, 17, '#');
    return 0;
}