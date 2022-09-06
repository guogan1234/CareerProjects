package customLock;

import java.util.HashMap;
import java.util.Map;

/**
 * A custom reentrant read/write lock that allows:
 * 1) Multiple readers (when there is no writer). Any thread can acquire multiple read locks
 * (if nobody is writing).
 * 2) One writer (when nobody else is writing or reading).
 * 3) A writer is allowed to acquire a read lock while holding the write lock.
 * 4) A writer is allowed to acquire another write lock while holding the write lock.
 * 5) A reader can NOT acquire a write lock while holding a read lock.
 *
 * Use ReentrantReadWriteLockTest to test this class.
 * The code is modified from the code of Prof. Rollins.
 */
public class ReentrantReadWriteLock {
    // FILL IN CODE:
    // Add instance variables:
    // for each threadId, you may want to store the number of read locks and write locks currently held
    private int readers = 0;
    private int writers = 0;
    private long lastWriterThreadId = 0;
    private Map<Long, Integer> monitorReaderMap;
    private Map<Long, Integer> monitorWriterMap;

    /**
     * Constructor for ReentrantReadWriteLock
     */
    public ReentrantReadWriteLock() {
        // FILL IN CODE: initialize instance variables
        monitorReaderMap = new HashMap<>();
        monitorWriterMap = new HashMap<>();
    }

    /**
     * Return true if the current thread holds a read lock.
     *
     * @return true or false
     */
    public synchronized boolean isReadLockHeldByCurrentThread() {
       // FILL IN CODE
        long threadId = Thread.currentThread().getId();
        Integer value = monitorReaderMap.get(threadId);
        if(value != null && value > 0){
            return true;
        }
        return false; // do not forget to change
    }

    /**
     * Return true if the current thread holds a write lock.
     *
     * @return true or false
     */
    public synchronized boolean isWriteLockHeldByCurrentThread() {
       // FILL IN CODE
        long threadId = Thread.currentThread().getId();
        Integer value = monitorWriterMap.get(threadId);
        if(value != null && value > 0){
            return true;
        }
        return false; // do not forget to change
    }

    /**
     * Non-blocking method that attempts to acquire the read lock and acquires it, if it can.
     * Returns true if successful.
     * Checks conditions (whether it can acquire the read lock), and if they are true,
     * acquires the lock (updates readers info).
     *
     * Note that if conditions are false (can not acquire the read lock at the moment), this method
     * does NOT wait, just returns false
     * @return
     */
    public synchronized boolean tryAcquiringReadLock() {
        // FILL IN CODE
        long threadId = Thread.currentThread().getId();
        if(lastWriterThreadId == 0 || lastWriterThreadId == threadId || writers == 0){
            readers++;
            // update monitor map about reader
            if(monitorReaderMap.keySet().contains(threadId)){
                Integer value = monitorReaderMap.get(threadId);
                value++;
                monitorReaderMap.put(threadId, value);
            }else {
                monitorReaderMap.put(threadId, 1);
            }
            return true;
        }
        return false; // do not forget to change
    }

    /**
     * Non-blocking method that attempts to acquire the write lock, and acquires it, if it is available.
     * Returns true if successful.
     * Checks conditions (whether it can acquire the write lock), and if they are true,
     * acquires the lock (updates writers info).
     *
     * Note that if conditions are false (can not acquire the write lock at the moment), this method
     * does NOT wait, just returns false
     *
     * @return
     */
    public synchronized boolean tryAcquiringWriteLock() {
       // FILL IN CODE
        long threadId = Thread.currentThread().getId();
        boolean result = false;
        if(threadId == lastWriterThreadId){
            if(readers == 0) {
                writers++;
                result = true;
            }
        }else {
            if(readers == 0 && writers == 0){
                writers++;
                result = true;
            }
        }
        if(result){
            // update monitor map about writer
            lastWriterThreadId = Thread.currentThread().getId();
            if(monitorWriterMap.keySet().contains(threadId)){
                Integer value = monitorWriterMap.get(threadId);
                value++;
                monitorWriterMap.put(threadId, value);
            }else {
                monitorWriterMap.put(threadId, 1);
            }
        }
       return result; // do not forget to change this
    }

    /**
     * Blocking method that will return only when the read lock has been
     * acquired.
     * Calls tryAcquiringReadLock, and as long as it returns false, waits.
     * Catches InterruptedException.
     *
     */
    public synchronized void lockRead() {
        // FILL IN CODE
        while (!tryAcquiringReadLock()){
            try {
                this.wait();
            }catch (InterruptedException e){
                System.out.println(e);
            }
        }
    }

    /**
     * Releases the read lock held by the calling thread. Other threads might
     * still be holding read locks. If no more readers after unlocking, calls notifyAll().
     */
    public synchronized void unlockRead() {
        long currentThreadId = Thread.currentThread().getId();
        // FILL IN CODE
        if(isReadLockHeldByCurrentThread()) {
            Integer value = monitorReaderMap.get(currentThreadId);
            value--;
            monitorReaderMap.put(currentThreadId, value);
            readers--;
            if (readers < 0) {
                readers = 0;
                return;
            }
            this.notifyAll();
        }
    }

    /**
     * Blocking method that will return only when the write lock has been
     * acquired.
     * Calls tryAcquiringWriteLock, and as long as it returns false, waits.
     * Catches InterruptedException.
     */
    public synchronized void lockWrite() {
        // FILL IN CODE
        while (!tryAcquiringWriteLock()){
            try{
                this.wait();
            }catch (InterruptedException e){
                System.out.println(e);
            }
        }

    }

    /**
     * Releases the write lock held by the calling thread. The calling thread
     * may continue to hold a read lock.
     * If the number of writers becomes 0, calls notifyAll.
     */

    public synchronized void unlockWrite() {
       // FILL IN CODE
        if(isWriteLockHeldByCurrentThread()) {
            writers--;
            long threadId = Thread.currentThread().getId();
            Integer value = monitorWriterMap.get(threadId);
            value--;
            monitorWriterMap.put(threadId, value);
            if (writers < 0) {
                writers = 0;
                return;
            }
            lastWriterThreadId = 0;
            this.notifyAll();
        }
    }
}
