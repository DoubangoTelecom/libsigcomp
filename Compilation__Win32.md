I suppose that libsigcomp is under **C:\**

# Microsoft Visual Studio #
Under _C:\libsigcomp_ directory you have **SigComp.sln** which contains both _demo.vcproj_ and _libsigcomp.vcproj_ projects.<br>
Using <b>Configuration Manager</b> (Visual Studio Toolbar) combobox choose Win32 as target platform.<br>
<br>
<h2>Cygwin</h2>
<blockquote>Go to directory where you would like to checkout source code<br>
</blockquote><ul><li>Checkout from svn<br>
<pre><code> svn checkout http://libsigcomp.googlecode.com/svn/trunk/ libsigcomp-read-only<br>
</code></pre>
</li><li>Go into libsigcomp directory<br>
<pre><code> cd libsigcomp-read-only/libsigcomp<br>
</code></pre>
</li><li>Disable cygwin dependencies<br>
<pre><code> export CXXFLAGS=-mno-cygwin<br>
</code></pre>
</li><li>Generate <b>configure</b> file<br>
<pre><code> ./autogen.sh<br>
</code></pre>
</li><li>Configure your source code<br>
<pre><code> ./configure --enable-win32thread --prefix=/usr/local/libsigcomp<br>
</code></pre>
</li><li>Compile and install<br>
<pre><code> make &amp;&amp; make install<br>
</code></pre>