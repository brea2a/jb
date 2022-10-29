Name:           jo
Version:        1.7
Release:        2%{?dist}
Summary:        jo is a small utility to create JSON objects

License:        GPL2
URL:            https://github.com/jpmens/jo
Source0:        https://github.com/jpmens/jo/releases/download/%{version}/jo-%{version}.tar.gz

BuildRequires:  autoconf
BuildRequires:	pandoc

%description
jo is a small utility to create JSON objects

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}
make check

%install
rm -rf $RPM_BUILD_ROOT
%make_install


%files
%doc
%{_bindir}/*
%{_mandir}/man1/*
%if 0%{?suse_version}
%{_datadir}/bash-completion/completions
%else
%{_sysconfdir}/bash_completion.d/%{name}.bash
%endif


%changelog
* Sat Oct 29 2022 JP Mens <jp@mens.de> 1.7
- bump version -- see Changelog
* Sat Jul 18 2020 JP Mens <jp@mens.de> 1.4
- bump version -- see Changelog
* Tue Apr 28 2020 Christian Albrecht <cal@albix.de> 1.3-2
- Fix broken download url
- Make bash completion work on RHEL based distros
* Tue Apr 7 2020 Kilian Cavalotti <kilian@stanford.edu> 1.3-1
- Bumped to 1.3 release version
- Include bash-completion file in package
* Thu May 18 2017 Fabian Arrotin <fabian.arrotin@arrfab.net> 1.1-1
- Bumped to 1.1 release version
* Wed Mar 15 2017 Fabian Arrotin <fabian.arrotin@arrfab.net> 1.0-1
- initial spec
