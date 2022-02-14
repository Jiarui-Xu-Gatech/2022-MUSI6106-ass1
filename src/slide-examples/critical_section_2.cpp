ReadWriteLock myLock;
for (;;)
{
    const ScopedReadLock myScopedLock (myLock);
    // myLock is now locked
    ...do some stuff...
    // myLock gets unlocked here.
}
