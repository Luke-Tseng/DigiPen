#include <math.h>

double circle_pi(int rectangles)
{
    double result = 0;
    double width = 2.0 / (double) rectangles; /* width of each rectangle */
    double xSum = width/2.0; /* midpoint */ 
    double height;
    int i;
    for(i = 1; i<= rectangles;++i) 
    {
        
        height = sqrt(4-(xSum*xSum)); /* height formula */
        xSum = width*i+width/2.0; /* x of the height formula */
        result += width * height; /* adding to the area */
    }
    return result;
}

double leibniz_pi(int iterations) {
    double count = 1;
    double result = 0;
    int i;
    for(i = 0; i<iterations;++i) { /* how many time to loop */
        if(i%2 == 0) {
            result += 1/count;
        } else if (i%2 == 1) {
            result -= 1/count;
        }
        count += 2; /* all the numbers are divided by an odd number */
    }
    return result*4; /* multiply by 4 at the end */
}   