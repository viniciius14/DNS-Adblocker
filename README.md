# DNS Proxy AdBlocker  

## Plan for the App  

* ### HTTP Server  

  * Make an HTTP server that no matter what returns a 1 x 1 image of a transparent pixel

* ### DNS Query  

  * Input: dns query < ip of dns-server > < hostname >  
  * Output everything in readable form (slide 11)  

* ### DNS Redirector  

  * Act as a DNS "server" and receive requests  
  * Check if a local hosts file for corresponding domain names and returns their IP if found  
  * If not makes a dns request to a "real" DNS server and returns that response to the received request  
  * Support multiple requests  
  * Cache Results  

## Note  

* The code has to work in the LusiadaLab VM 2.0  


## Sources  

* Computer Networking A Top-Down Approach 7th edition
* [Beej's Guide to C Programming](https://beej.us/guide/bgc/pdf/bgc_a4_c_2.pdf)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_2.pdf)
* [DNS RFC](https://www.ietf.org/rfc/rfc1035.txt)
* [Beej's Demo Server](https://beej.us/guide/bgnet/source/examples/server.c)
* [Beej's Demo Client](https://beej.us/guide/bgnet/source/examples/client.c)

