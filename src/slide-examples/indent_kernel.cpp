int power(int x, int y)
{
        int result;
 
        if (y < 0) {
                return 0;
        } else {
                for (result = 1; y; y--)
                        result *= x;
 
                return result;
        }
}