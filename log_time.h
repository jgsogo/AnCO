

#if ((__GNUC__>=4) || (__GNUC__==4 && __GNUC_MINOR__>=8) || (_MSC_VER >= 1700)) // GCC or VisualStudio>=2012
    #include <chrono>

    struct log_time {
        log_time() {
            t1 = std::chrono::high_resolution_clock::now();
            };
        ~log_time() {        
            }
        void tic() {
            t1 = std::chrono::high_resolution_clock::now();
            };
        void toc() {
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            std::cout << "\t|| " << std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " miliseconds ||" << std::endl;  
            };
        std::chrono::high_resolution_clock::time_point t1;
        };
#else
    #include <ctime>
    struct log_time {
        log_time() {
            t1 = clock();
            };
        ~log_time() {        
            }
        void tic() {
            t1 = clock();
            };
        void toc() {
            clock_t t2 = clock();
            std::cout << "\t|| " << (t2-t1)/CLOCKS_PER_SEC*1000 << " miliseconds ||" << std::endl;  
            };
        clock_t t1;
        };
#endif