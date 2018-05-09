
*****************************************************************************************
**************************************** Profiny ****************************************
*****************************************************************************************

Profiny is a profiling helper tool which consists of just one header file. In order to
use Profiny, you should have a compiler supporting c++11.

A sample code using Profiny can be written as follows:

	#include "Profiny.h"

	int f(int n) {
		PROFINY_SCOPE
		int result = 1;
		for (int i=1; i<n; i++) result *= i;
		return result;
	}

	int g(int n) {
		PROFINY_SCOPE
		if (n < 2) return 1;
		return g(n-1) * n;
	}

	int h2(int n);

	int h1(int n) {
		PROFINY_SCOPE
		if (n < 2) return 1;
		return h2(n-1) * n;
	}

	int h2(int n) {
		PROFINY_SCOPE
		if (n < 2)
			return 1;
		return h1(n-1) * n;
	}

	int main() {
		PROFINY_SCOPE
		f(1000000000);
		g(9);
		h1(9);
		return 0;
	}

As you see in the example, every function block starts with PROFINY_SCOPE. This is a
Profiny macro, which means that, these blocks are registered to be profiled by Profiny.
Any scope can be registered to Profiny (does not have to be function block). Also, you
can profile starting from one point (not the beginning) of the scope till the end of it.
Each scope can have at most one Profiny macro, otherwise the code won't compile.


**************************************** COMPILE ****************************************

While compiling the code, the user should select which profiling strategy to use by using
-D argument of the compiler. Either PROFINY_CALL_GRAPH_PROFILER or PROFINY_FLAT_PROFILER
should be defined, and Profiny will act according to these definitions as follows:

	(1) if only PROFINY_CALL_GRAPH_PROFILER is defined, it will work as a call-graph
	      profiler
	(2) if only PROFINY_FLAT_PROFILER is defined, it will work as a flat profiler
	(3) if neither of them is defined, Profiny macros will be set to blank (i.e.
	      profiling will be off)
	(4) if both of them are defined, it will give an error and won't compile

So the above code (main.cpp) can be compiled by either

	g++ -DPROFINY_CALL_GRAPH_PROFILER --std=c++11 -o "profiny_call_graph_test" "../main.cpp"

		or

	g++ -DPROFINY_FLAT_PROFILER --std=c++11 -o "profiny_flat_test" "../main.cpp"

		or, if you want to switch profiling off

	g++ --std=c++11 -o "profiny_not_profiled_test" "../main.cpp"


**************************************** OUTPUT ****************************************

By default (if the profiling is not off), and if your program exits normally, Profiny
will print profiling results in "profinity.out" file. Also, the user can force printing
results at any time by calling:

	profiny::Profiler::printStats("filename")

The results of the above example will be as follows:

	If flat profiling requested:
		../main.cpp:f:8  T:2.58832  #:1  %:99.2922
		../main.cpp:g:19  T:0.0172229  #:1  %:174.186
		../main.cpp:h1:29  T:0.0164029  #:1  %:60.9647
		../main.cpp:h2:37  T:0.0163981  #:1  %:60.9828
		../main.cpp:main:45  T:2.62199  #:1  %:99.5425

	If call-graph profiling requested:
		../main.cpp:main:45  T:2.62712  #:1  %:99.7289
		  ../main.cpp:f:8  T:2.59415  #:1  %:99.4546
		  ../main.cpp:g:19  T:0.0164109  #:1  %:182.805
		  ../main.cpp:h1:29  T:0.0165158  #:1  %:60.5482
		    ../main.cpp:h2:37  T:0.0165097  #:1  %:60.5705

In the above results, each line represents a profile. Second column in each line shows
total time, third one shows total number of executions for that scope, and the last
column shows the CPU usage. As shown in the examples, CPU usage may be invalid if we have
a really fast function (i.e. user and system times cannot be evaluated that precisely).

In call-graph profiling, the indentation shows the call graph. So, the same scope can be
shown by multiple entries (i.e. if function "f" is called from "g" and "h", both calls
will be shown separately). Note that this is not the complete call graph, instead, it is
a call graph of scopes that we registered to Profiny.


**************************************** ADVANCED ****************************************

In the profiling results, first column represents the name of the profiled scope. In this
example we used PROFINY_SCOPE that's why the name simply is "<file>:<func>:<line>". It is
also possible to use these macros:

	PROFINY_SCOPE: This is the simplest macro. The profile name for this macro is as
		follows:
			<file>:<func>:<line>

	PROFINY_SCOPE_WITH_ID(ID): This macro can be used where we want to differentiate
		profiles of a single scope by giving an ID to each. For example, we can
		use this macro if we want to profile every instance of a class separately.
		If we have a unique "m_name" member in the class, we can simply write:
			PROFINY_SCOPE_WITH_ID(m_name)
		to the beginning of each function, and profile methods of the instances
		(not the class' method). The name of each profile will be:
			<file>:<func>:<line>:<ID>
		
	PROFINY_NAMED_SCOPE(NAME): This macro does not automatically generate profile
		name, instead, it takes it from the user. So, the profile name is as
		follows:
			<NAME>

	PROFINY_NAMED_SCOPE_WITH_ID(NAME, ID): This macro uses name and ID given as
		arguments and constructs profile name as follows:
			<NAME>:<ID>

Later, if you chose PROFINY_CALL_GRAPH_PROFILER, you may want to determine whether
recursive calls will be omitted or not (omitted by default) by calling:

	profiny::Profiler::setOmitRecursiveCalls(bool)

If call-graph profiling requested and recursive calls are not omitted the results will be
as follows (including recursive calls):

	../main.cpp:main:45  T:2.58801  #:1  %:99.3041
	  ../main.cpp:f:8  T:2.58789  #:1  %:99.3086
	  ../main.cpp:g:19  T:4.4433e-05  #:1  %:0
	    RECURSIVE@../main.cpp:g:19  T:2.9165e-05  #:1  %:0
	      RECURSIVE@../main.cpp:g:19  T:2.5138e-05  #:1  %:0
	  ../main.cpp:h1:29  T:3.0788e-05  #:1  %:0
	    ../main.cpp:h2:37  T:2.7134e-05  #:1  %:0
	      RECURSIVE@../main.cpp:h1:29  T:2.3411e-05  #:1  %:0
	        RECURSIVE@../main.cpp:h2:37  T:1.9694e-05  #:1  %:0
	          RECURSIVE@../main.cpp:h1:29  T:1.6154e-05  #:1  %:0
	            RECURSIVE@../main.cpp:h2:37  T:1.2546e-05  #:1  %:0

Happy profiling!

