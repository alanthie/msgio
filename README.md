# msgio
libevent thin c++ messaging layer

Moving CryptoChat toward a libevent messaging layer, to handle thousand of connections.

msgio is a logical model sample of the messaging layer required

TEST results:

400 clients, Message in+out rate msg/sec = 121,141
400 clients, Bytes in+out rate bytes/sec = 186,072,000 (100MB/sec bidirectional)
CPU utilization (single thread): 20% of one cpu
