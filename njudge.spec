Summary: Internet Diplomacy Adjudicator
Name: njudge
Version: 1.5.3
Release: 1
Group: Games
Copyright: free for non-commercial use.  See COPYING for details.
Packager: Jaldhar H. Vyas <judgekeeper@braincells.com>
URL: http://www.njudge.org/
Source: %{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
The judge adjudicates email games of the Hasbro boardgame Diplomacy. In
Diplomacy players take the role of one of the 7 great powers of Europe at
the turn of the 20th Century.  They must vie through conquest and
negotiation to dominate the continent. This package will install in /home/judge

%prep
%setup -n %{name}-%{version}

%build
./configure --mandir=/usr/share/man --with-dir=/home/judge --with-user=root
make 

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT docdir=/usr/share/doc/njudge mandir=/usr/share/man
rm $RPM_BUILD_ROOT/home/judge/newlogs
install -m 0755 debian/newlogs $RPM_BUILD_ROOT/home/judge/newlogs
install -D -m 0755 debian/cron.weekly $RPM_BUILD_ROOT/etc/cron.weekly/njudge
echo After installing this package, you must edit /home/judge/dip.conf

%post
/usr/sbin/useradd -d /home/judge -M -c"Diplomacy Adjudicator" judge || true
chown -R judge:judge /home/judge
perl -pi -e 's!\.\./!\./!g;' /home/judge/dip.conf || true
perl -pi -e 's/root/judge/g;' /home/judge/dip.conf || true
mv /home/judge/root-forward /home/judge/.forward || true
perl -pi -e 's/root/judge/g;' /home/judge/smail || true

%postun
userdel -r judge || true;
rm -rf /home/judge || true;

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%config /etc/cron.weekly/njudge
%config /home/judge/dip.conf
%config /home/judge/dip.msg
%config /home/judge/dip.footer
%config /home/judge/dip.master
%config /home/judge/dip.addr
%config /home/judge/dip.whois
%config /home/judge/dip.blist
%config /home/judge/smail
/home/judge
/usr/share
