CriticalSection objectLock;

void foo()
{
    const ScopedLock myScopedLock (objectLock);
    // objectLock is now locked..
    ...do some thread-safe work here...
    // ..and objectLock gets unlocked here, as myScopedLock goes out of
    // scope at the end of the block
}