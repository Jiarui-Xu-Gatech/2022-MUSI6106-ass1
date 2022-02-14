int main ()
{
    const int kiXDim    = 3;            //!< length in x dimension
    const int kiYDim    = 2;            //!< length in y dimension
    float **ppfDblePtr  = 0;            //!< double pointer variable 

    // alloc heap memory (verification omitted here)
    ppfDblePtr = new float* [kiXDim];
    for (int i = 0; i < kiXDim; i++)
    {
        ppfDblePtr[i]   = new float [kiYDim];
    }

    // initialize array
    for (int i = 0; i < kiXDim; i++)
    {
        for (int j = 0; j < kiYDim; j++)
        {
            ppfDblePtr[i][j] = (i+1)*(j+1) + 1.5F;

            // meaningless output
            cout << ppfDblePtr[i][j] << " ";
        }
        cout << endl;
    }

    // free memory (necessary because no smart pointer!)
    for (int i = 0; i < kiXDim; i++)
        delete [] ppfDblePtr[i];
    delete [] ppfDblePtr;
    return 0;
}
