class Singleton
{
    public:
        static Singleton& getInstance()
        {
            static Singleton    instance;
            return instance;
        }
    private:
        Singleton() {}
        Singleton(Singleton const&);      // Don't Implement.
        void operator=(Singleton const&); // Don't implement
 };