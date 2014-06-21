sails
=====

sails is a platform for network applications. sails uses an event-driven I/O model.

c++

server
------

because of multiqueue netcard, so sails will make network threads number is configurable, make data queue no lock.


    +----------------------------------------------------------------------------------------------------------------+
	|			  			   	  			 					  		   										 	 |
	|			  			      	  	   	 					  		   		   +-----------+  				 	 |
	|			  			   	   +----------+	   	   	   	   	   	   	   	   	   |           |   	   	   		 	 |
	|             		  	   	   |  Decode -+	   	   	   	   	   	  	   		   |  Logic    +------------+	 	 |
	|				  	   	 	   +---+------+----------->+----+----+------------>+  thread1  |  			|	 	 |
	|				   	    	       	^      			 / |       	 \	   		   +-----------+  			|	 	 |
	|	   +------------+---------------+	 		 	/  |  Queue  |\    									|	 	 |
	|	   |   	  	    | 	  	 			 		   /   +---------+ \   									|	 	 |
	|	   | Recv/Send 	+<---------+----------+		  /   		  	    \  		   +------------+			|	 	 |
	|	   +------------+  	 	   |  Encode  |		 / 	  		  		 \  	   |   	   	 	+-----------+	 	 |
	|					   	   	   +----------+		/ 	  		  		  \  	   |  ........	|			|	 	 |
	|					      	       ^       	   / 	  		  		   \  	   +------------+			|	 	 |
	|                                  |          /       		  		   	\   							|	 	 |
	|					 	  	 	   +-------------------+---------+	   	 \    							|	 	 |
	|					 	   	 				/ 		   |   	   	 |	   	  \    +------------+			|	 	 |
	|					 	    			   / 		   |  Queue  |<----+   \   |  Logic	    |	    	|	 	 |
	|					 		   +----------/			   +---------+	   |	\  |  threadn   +-----------+	 	 |
	|					 		   |  Decode  |			  		  		   |	   +------------+			|	 	 |
	|      +------------+--------->+----------+                   		   |	   							|	 	 |
	|      |            |                                  +---------+     |								|	 	 |
	|	   | Recv/Send	|								   |     	 | 	   |								|	 	 |
	|	   +------------+ <--------+----------+	  		   |  Queue	 |<----+--------------------------------+	 	 |
	|	                 		   |  Encode  <------------+---------+ 										 	 	 |
	|							   +----------+			               											 	 |
	|							              																	 	 |
	|                                                                                                            	 |
	|                                                                                                                |
	|                                                                                                                |
	|                                                                                                                |
	|                                                                                                                |
	|																												 |
	|                                                                                                                |
	+----------------------------------------------------------------------------------------------------------------+



client
------

* sync(easy): each request must rebuild a new connect with server
* sync(hard): reuse a connect for many request in multithread, manager a struct{ticketid, condition_variable}, and interface wait for condition variable, when recv ticketid from server, and get this struct
and notify condition variable.

* async:manager a struct{ticketid, function(void *)}, when get ticketid from server, invoke function


server status
-------------

with ctemplate, sails build-in status report display by html file


build
=====

because the use of c++11 regex features, so need gcc 4.9 or above


performance
===========

test computer:

* cpu  :   T6400  @ 2.00GHz, cpu num:1, cpu cores:2

* memory : 5G

* server and client run on this computer

* build with -O2

**one client sync call method 10000 times**


    sails@xu:~/workspace/sails/example/echo_sync$ time ./client 
    clients thread:1
    
    real	0m1.112s
    user	0m0.182s
    sys	    0m0.348s
and in the server two thread to handle message, in this case will reach 9000 tps.

**two client sync call method 10000 times**


    sails@xu:~/workspace/sails/example/echo_sync$ time ./client 8000 2
    clients thread:2
    
    real	0m0.802s
    user	0m0.205s
    sys	    0m0.456s
25000 tps.


**five client thread sync call, and each 10000 times**

    sails@xu:~/workspace/sails/example/echo_sync$ time ./client 8000 5
    clients thread:5
    
    real	0m1.676s
    user	0m0.420s
    sys	    0m1.028s

30000 tps

**twenty client thread sync call, and each 10000 times**

    sails@xu:~/workspace/sails/example/echo_sync$ time ./client 8000 30
    clients thread:5
    
    real	0m6.460s
    user	0m1.717s
    sys	    0m4.074s

31000 tps

test computer2:

* cpu  :   E5-2630  @ 2.30GHz, cpu cores:8

* memory : 8G, 6G free

* server and client run on this computer

* build with debug

**one client sync call method 10000 times**

    clients thread:1
    
    real	0m0.792s
    user	0m0.248s
    sys	    0m0.056s
12,600 tps

**eight client sync call method 10000 times(8*10000)**

    clients thread:8
    
    real	0m1.360s
    user	0m0.856s
    sys	    0m1.684s

52,000 tps

**30 client sync call method 10000 times(30*10000)**

    clients thread:30
    
    real	0m3.653s
    user	0m2.716s
    sys	    0m5.784s

82,000 tps

**1000 client sync call method 10000 times(1000*10000)**

    clients thread:1000
    
    real	0m6.268s
    user	0m6.224s
    sys	    0m12.701s

1,600,000 tps
