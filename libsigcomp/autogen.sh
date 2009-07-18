echo "aclocal"
aclocal

echo "autoheader"
autoheader

echo "create NEWS, README, AUTHORS and ChangeLog"
touch NEWS README AUTHORS ChangeLog

echo "automake"
automake -a

echo "autoconf"
autoconf

#echo "configure"
#./configure --enable-win32thread --prefix=/usr/local/libsigcomp
#./configure --with-pthread=/usr --prefix=/usr/local/libsigcomp

#echo " make clean"
#make clean 

#echo "make"
#make

#echo "make install"
#make install