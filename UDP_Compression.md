It is easy to compress your SIP message and send it over UDP connection. Compression can be safely performed in multithreaded application because **libSigComp** is thread-safe.
**libsigcomp** use _sigcomp_ as default namespace to avoid conflicts. You can change this namespace by redefining `MY_FAVORITE_NS` macro in _libsigcomp\_config.h_ file.

  * Include header file
```
#include <libsigcomp/SigCompManager.h>
```

  * Perform compression
```
#define COMPARTMENT_ID	1983    // Your compartment id
#define IS_STREAM	false	// Not using reliable transport (udp)

#define MAX_BUFFER_SIZE		0xffff

// Output buffer (will hold compressed message to send over udp connection)
char outputBuffer[MAX_BUFFER_SIZE];

// Create your manager
sigcomp::SigCompManager* manager = new sigcomp::SigCompManager();

// Set DMS and SMS
// This step is not mandatory. default values are whose defined in RFC 5049
manager->setDecompression_Memory_Size(8192);
manager->setState_Memory_Size(8192);

// Compress your message
size_t outLen = manager->compress(COMPARTMENT_ID, "REGISTER ...", strlen("REGISTER ..."), outputBuffer, MAX_BUFFER_SIZE, IS_STREAM);

if(outLen){
   // send sigcomp message over UDP connection
   sendto(sock, outputBuffer, outLen , 0, (SOCKADDR *)address, sizeof(address));
}
else{
   // MUST never happen --> Post you feedback ;)
   assert(0);
}

// close compartment to release allocated memory
manager->closeCompartment(COMPARTMENT_ID);
// delete manager
delete manager;
```

et voilà