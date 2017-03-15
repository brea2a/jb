Name:           jo
Version:        1.0
Release:        1%{?dist}
Summary:        jo is a small utility to create JSON objects

License:        GPL2
URL:            https://github.com/jpmens/jo
Source0:        https://github.com/jpmens/jo/releases/download/v%{version}/jo-%{version}.tar.gz

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


%changelog
* Wed Mar 15 2017 Fabian Arrotin <fabian.arrotin@arrfab.net> 1.0-1
- initial spec
