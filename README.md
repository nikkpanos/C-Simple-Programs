# C-Simple-Programs

1.  Project1.c
    Program to create the digital bit sequence of a given sample.
    Needed Input:    value range of the sample e.g. 0 - 7 (Volts)
                     number of quantization levels (picking between 16 and 32)
                     sample values
    
    Output:          bit sequence for the above sample values and criteria

====================================================================================================================================================================
              
2.  Project2.c
    Program to calculate the delay between two network nodes.
    Needed Inpuut:   Packet size
                     Link bit rate
                     Propagation delay
                     Queueing and processing delay in the form of percentage of total delay

    Output:          Total delay for the packet to be received
                     Number of bits (of the packet) that will be received after a certain period of time

====================================================================================================================================================================

3.  Project3.c
    Program to calculate the delay between two network nodes.
    Needed Input:    Packet Size (no header)
                     Packet header length
                     Number of extra nodes between the sending and receiving node
                     Bit rate for every link
                     Fragment size (in case of fragmentation)

    Output:          Total delay with and without fragmentation

    P.S.  Other kind of delays are not considered and we assume that a node does not wait to receive the whole packet before relaying it

====================================================================================================================================================================

4.  Project4.c
    Program to calculate the routing tables of a node in a network, using either Dijkstra's or Bellman-Ford's algorithm.
    Needed Input:    Number of nodes and links/edges that compose the network
                     Bit rate for every link
                     Starting node

    Output:          All the steps taken by the algorithm
                     The final routing table created

    P.S. For the weight calculation, only bit rate is considered
