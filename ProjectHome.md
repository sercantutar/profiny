Profiny is a lightweight profiler written in C++. It supports both flat profiling and call-graph profiling. Even though it doesn't use sampling, its overhead is almost negligible. Plus, its usage is fairly simple. All you have to do is include the header file and write a macro to every scope you want to profile.

It is this simple to use Profiny (see Wiki):

```
        int f(int n)
        {
                PROFINY_SCOPE   // just add this to profile
                int result = 1;
                for (int i=1; i<n; i++) result *= i;
                return result;
        }
```


---


Downloads section moved to Google Drive.

You can download compressed archives of releases here: [DOWNLOADS](http://cppip.blogspot.com/2013/05/profiny-downloads.html).