# libSigComp with GCC #

  * Go to directory where you would like to checkout source code
  * Checkout from svn
```
 svn checkout http://libsigcomp.googlecode.com/svn/trunk/ libsigcomp-read-only
```
  * Go into libsigcomp directory
```
 cd libsigcomp-read-only/libsigcomp
```
  * Generate **configure** file
```
 ./autogen.sh
```
  * Configure your source code (change /usr with the directory containing pthread header and library)
```
 ./configure --with-pthread=/usr --prefix=/usr/local/libsigcomp
```
  * Compile and install
```
 make && make install
```

# Demo program with GCC #
  * Go into demo directory
```
 cd ../demo
```
  * Compile source code
```
 g++ *.cpp /usr/local/libsigcomp/lib/libsigcomp.a -I/usr/local/libsigcomp/include
```
  * Run demo
```
 ./a.out
```