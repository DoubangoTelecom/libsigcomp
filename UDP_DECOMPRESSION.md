Decompression is as easy as compression (thread-safe).

  * Include header file
```
#include <libsigcomp/SigCompManager.h>
```

  * Perform decompression
```
#define COMPARTMENT_ID  1983    // Your compartment identifier
#define IS_STREAM       false   // Not using reliable transport (udp) 
 
#define MAX_BUFFER_SIZE         0xffff 
 
// Inpput buffer (will hold compressed message) 
char inputBuffer[MAX_BUFFER_SIZE]; 
// Output buffer (will hold uncompressed message) 
char outputBuffer[MAX_BUFFER_SIZE]; 

// Create your manager 
sigcomp::SigCompManager* manager = new sigcomp::SigCompManager(); 

// Create result holder
sigcomp::DecompressionResult result; 
// Set compartment identifier
result.setCompartmentId(COMPARTMENT_ID);
// Set output buffer (buffer holding uncompressed message)
result.setOutputBuffer(outputBuffer, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID);

// Receive SigComp message from network
int addressSize = sizeof(address);
size_t inputLen = recvfrom(sock, inputBuffer, MAX_BUFFER_SIZE, 0, (SOCKADDR *)&address, &addressSize);

// Decompress SigComp message
size_t outLen = manager->decompress(inputBuffer, inputLen, &result);
if(outLen)
{
   // outputBuffer now contains the result and outLen is the result length
   // You should pass the outputBuffer (which contains SIP message) and outLen to the application layer
   // IMPORTANT: provide the compartment id --> save temp states
   manager->provideCompartmentId(&result);
}
else if(result2.getIsNack())
{
   // Our buffer contains NACK (see RFC 4077) message because decompression failed
   // You MUST send this NACK message to the remote party
   sendto(sock, result.getNackInfo()->getBuffer(), result.getNackInfo()->getSize() , 0, (SOCKADDR *)address, sizeof(address));
}
else
{
   // MUST never happen --> post feedback ;)
   assert(0);
}

// close compartment to release allocated memory 
manager->closeCompartment(COMPARTMENT_ID); 
// delete manager 
delete manager;
```

et voilà