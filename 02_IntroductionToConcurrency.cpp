

//concurrency: perform two or more activies at the same time 
//singlke-threaded concurrency: single process 
// interprocess communicaiton is required for exchange data
//e.g. message queue, pipe. semaphore, shared memory, socket
//each process has its own private memory; can run on different computers 

//multi-threaded concurrency 
//a single process is used to perform all the activies 
// concurrency is cahieved by starting a new thread for each activity 
//all the threads shre a common memory space
//light weight process; lack of data protection between threads 

//concurrency and applications 
//server applications: receives many independent requests; handled by speparate threads 
//client applications

//history 
// fork() ; pip() 
// pthreads library: POSIX system ; 
// ACE; BOOST; POCO 



