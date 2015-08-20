**libSigComp** project is a complete and compliant SigComp API to speed-up SigComp integration in Open-Source IMS projects. **libSigComp** is released under [LGPLv3 license](http://www.gnu.org/licenses/lgpl.html).
<br>
<br>
As many operators have begun to commercially deploy <a href='http://en.wikipedia.org/wiki/IP_Multimedia_Subsystem'>IMS</a>, the relevance of using SigComp to lower bandwidth usage will come quickly. In my own opinion I think that most operators (especially those using <a href='http://en.wikipedia.org/wiki/Rich_Communication_Suite'>RCS</a>) will question how to reduce SIP signaling (registration, billing, presence, messaging …) bandwidth usage (who will pay bits?).<br>
<br>
These questions will especially concern using SIP (or all other text-based protocols) in wireless handsets as part of 2.5G and 3G cellular networks.<br>
<br>
<br>
SigComp stands for Signaling Compression and has been defined in <a href='http://www.ietf.org/rfc/rfc3320.txt'>RFC 3320</a> by the Internet Engineering Task Force (IETF) ROHC working group.<br>
<br>
<br>
<a href='http://upload.wikimedia.org/wikipedia/en/8/8f/SigComp_Architecture.png'><img src='http://upload.wikimedia.org/wikipedia/en/8/8f/SigComp_Architecture.png' /></a>
<br>
<i>SigComp Architecture (From wikimedia)</i><br>
<br>
Many application protocols used for multimedia communications are text-based and engineered for bandwidth rich links.  As a result the messages have not been optimized in terms of size.  For example, typical IMS/SIP messages range from a few hundred bytes up to two thousand bytes or more. For this reason, SigComp is mandatory for 3GPP IMS netwoks and <a href='http://en.wikipedia.org/wiki/Push_to_Talk_over_Cellular'>PoC</a> systems.<br>
<br>
<br>
SigComp could also be useful for RCS (Rich Communication Suite) networks because of the size of the SIP packets (more than three thousand bytes for presence publication). Using SigComp in IMS/RCS context will reduce the round-trip over slow radio links.<br>
<br>
<br>
<h2>Supported OS</h2>
<ul><li>Windows Xp/Vista (Visual Studio 2005 or Mingw32)<br>
</li><li>Windows Mobile 5 and later (Visual Studio 2005 or <a href='http://cegcc.sourceforge.net/docs/build-toolchain.html'>Mingw32ce/cegcc toolchain</a>)<br>
</li><li><a href='http://en.wikipedia.org/wiki/S60_Platform'>Symbian S60</a> (Carbide.c++ v2.0 with S60_3rd_FP2_SDK_v1.1)<br>
</li><li>All Linux, FreeBSD, ... (GCC 4.x)<br>
<br>
<h2>FEATURES</h2>
The goal of this project is to provide a SigComp framework which:<br>
<br>
<br>
</li><li>Could be used as an external API or Framework<br>
</li><li>Highly portable (Coded in C/C++ without any external dependencies)<br>
</li><li>Easily configurable (memory usage, priorities in static dictionaries, stateful/stateless modes, dynamic/static/shared compression types …)<br>
</li><li>Easy to integrate with any existing SIP/IMS stack, Proxy-CSCF, PoC client …<br>
</li><li>Allow to easily plug your own compressor (DEFLATE –<a href='http://www.ietf.org/rfc/rfc1951.txt'>RFC 1951</a>- will be the default)<br></li></ul>

<ul><li>Robust<br>
</li><li>Efficiently run on mobile handsets (small footprint)<br>
</li><li>Use small memory (UDVM decompression)<br>
</li><li>Run fast without high CPU usage<br></li></ul>

<ul><li>Supports both TCP and UDP compression modes<br>
</li><li>Thread-safe<br>
<br>
<h2>COMPLIANCE</h2>
</li><li><a href='http://www.ietf.org/rfc/rfc3320.txt'>RFC 3320</a>: Signaling Compression (SigComp)<br>
</li><li><a href='http://www.ietf.org/rfc/rfc3321.txt'>RFC 3321</a>: Signaling Compression (SigComp) - Extended Operations<br>
</li><li><a href='http://www.ietf.org/rfc/rfc3485.txt'>RFC 3485</a>: The Session Initiation Protocol (SIP) and Session Description Protocol (SDP) Static Dictionary for Signaling Compression (SigComp)<br>
</li><li><a href='http://www.ietf.org/rfc/rfc4070.txt'>RFC 4077</a>: A Negative Acknowledgement Mechanism for Signaling Compression<br>
</li><li><a href='http://www.ietf.org/rfc/rfc4464.txt'>RFC 4464</a>: Signaling Compression (SigComp) Users' Guide<br>
</li><li><a href='http://www.ietf.org/rfc/rfc4465.txt'>RFC 4465</a>: Signaling Compression (SigComp) Torture Tests<br>
</li><li><a href='http://www.ietf.org/rfc/rfc4896.txt'>RFC 4896</a>:Signaling Compression (SigComp) Corrections and Clarifications<br>
</li><li><a href='http://www.ietf.org/rfc/rfc5049.txt'>RFC 5049</a>: Applying Signaling Compression (SigComp) to the Session Initiation Protocol (SIP)<br>
</li><li><a href='http://www.ietf.org/rfc/rfc5112.txt'>RFC 5112</a>: The Presence-Specific Static Dictionary for Signaling Compression (Sigcomp)<br>
</li><li><a href='http://www.ietf.org/rfc/rfc1662.txt'>RFC 1662</a>: PPP in HDLC-like Framing<br>
</li><li><a href='http://www.ietf.org/rfc/rfc1951.txt'>RFC 1951</a>: DEFLATE Compressed Data Format Specification version<br>
</li><li><a href='http://www.ietf.org/rfc/rfc3174.txt'>RFC 3174</a>: US Secure Hash Algorithm 1 (SHA1)<br>
</li><li>3GPP TR23.979 Annex C: Required SigComp performance<br>
<br>
<h2>MORE INFORMATION</h2>
</li><li>If you have any problem please look at <a href='Getting_Started.md'>Getting started guide</a>
</li><li>To have feedbacks or if you are interested for IOTs don’t hesitate to contact me at diopmamadou{at yahoo}dot fr. <br>
</li><li>libSigComp homepage: <a href='https://github.com/DoubangoTelecom/libsigcomp/'>https://github.com/DoubangoTelecom/libsigcomp/</a><br>
<br>
<b>© 2009-2015 [Doubango Telecom](https://doubango.org)</b><br>
<br>
<br>
<a href='http://www.gnu.org/graphics/lgplv3-88x31.png'><img src='http://www.gnu.org/graphics/lgplv3-88x31.png' /></a>
