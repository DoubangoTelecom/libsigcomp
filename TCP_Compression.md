Only change from
```
#define IS_STREAM       false
```
to
```
#define IS_STREAM       true
```
or call compression function like this:
```
manager->compress(COMPARTMENT_ID, "REGISTER ...", strlen("REGISTER ..."), outputBuffer, MAX_BUFFER_SIZE, true/*tcp*/);
```