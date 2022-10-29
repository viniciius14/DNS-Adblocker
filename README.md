# DNS Proxy AdBlocker  

## Plan for the App  

* ### HTTP Server  

  * Make an HTTP server that no matter what returns a 1 x 1 image of a transparent pixel  
  * Possibly change it in the future so that it also replies a 0-second video if video platforms (like YouTube) require videos in the ad designated space  

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

## TODO  

* [ ] Fill the links in on the sources section
* [ ] Make a test server
* [ ] Make test client
* [ ] Test

## Sources  

* Computer Networking A Top-Down Approach 7th edition
* [Beej's Guide to C Programming](https://beej.us/guide/bgc/pdf/bgc_a4_c_2.pdf)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_2.pdf)
* [Let's code a TCP/IP stack, 1: Ethernet & ARP](https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/)
* [Let's code a TCP/IP stack, 2: IPv4 & ICMPv4](https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/)
* [Let's code a TCP/IP stack, 3: TCP Basics & Handshake](https://www.saminiir.com/lets-code-tcp-ip-stack-3-tcp-handshake/)
