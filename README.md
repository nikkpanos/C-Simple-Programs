# C-Simple-Programs

1.  Project1.c<br>
    Program to create the digital bit sequence of a given sample.<br>
    Needed Input:    value range of the sample e.g. 0 - 7 (Volts)<br>
                     number of quantization levels (picking between 16 and 32)<br>
                     sample values<br>
    <br>
    Output:          bit sequence for the above sample values and criteria<br>
<br>
====================================================================================================================================================================<br>
  <br>            
2.  Project2.c<br>
    Program to calculate the delay between two network nodes.<br>
    Needed Inpuut:   Packet size<br>
                     Link bit rate<br>
                     Propagation delay<br>
                     Queueing and processing delay in the form of percentage of total delay<br>
<br>
    Output:          Total delay for the packet to be received<br>
                     Number of bits (of the packet) that will be received after a certain period of time<br>
<br>
====================================================================================================================================================================<br>
<br>
3.  Project3.c<br>
    Program to calculate the delay between two network nodes.<br>
    Needed Input:    Packet Size (no header)<br>
                     Packet header length<br>
                     Number of extra nodes between the sending and receiving node<br>
                     Bit rate for every link<br>
                     Fragment size (in case of fragmentation)<br>
<br>
    Output:          Total delay with and without fragmentation<br>
<br>
    P.S.  Other kind of delays are not considered and we assume that a node does not wait to receive the whole packet before relaying it<br>
<br>
====================================================================================================================================================================<br>
<br>
4.  Project4.c<br>
    Program to calculate the routing tables of a node in a network, using either Dijkstra's or Bellman-Ford's algorithm.<br>
    Needed Input:    Number of nodes and links/edges that compose the network<br>
                     Bit rate for every link<br>
                     Starting node<br>
<br>
    Output:          All the steps taken by the algorithm<br>
                     The final routing table created<br>
<br>
    P.S. For the weight calculation, only bit rate is considered<br>
