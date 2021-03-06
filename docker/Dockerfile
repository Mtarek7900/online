# This file is part of the LibreOffice project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

FROM ubuntu:18.04

# refresh repos otherwise installations later may fail
RUN apt-get update

# install LibreOffice run-time dependencies
# install adduser, findutils, openssl and cpio that we need later
# install an editor
RUN apt-get -y install locales-all libpng16-16 fontconfig adduser cpio findutils nano libpocoxml50 libpocoutil50 libpoconetssl50 libpoconet50 libpocojson50 libpocofoundation50 libpococrypto50 libcap2-bin openssl inotify-tools procps libxcb-shm0 libxcb-render0 libxrender1 libxext6

# tdf#117557 - Add CJK Fonts to LibreOffice Online Docker Image
RUN apt-get -y install fonts-wqy-zenhei fonts-wqy-microhei fonts-droid-fallback fonts-noto-cjk

# copy freshly built LibreOffice master and LibreOffice Online master with latest translations
COPY /instdir /

# copy the shell script which can start LibreOffice Online (loolwsd)
COPY /scripts/run-lool.sh /

# set up LibreOffice Online (normally done by postinstall script of package)
RUN setcap cap_fowner,cap_mknod,cap_sys_chroot=ep /usr/bin/loolforkit
RUN adduser --quiet --system --group --home /opt/lool lool
RUN mkdir -p /var/cache/loolwsd && chown lool: /var/cache/loolwsd
RUN rm -rf /var/cache/loolwsd/*
RUN rm -rf /opt/lool
RUN mkdir -p /opt/lool/child-roots
RUN loolwsd-systemplate-setup /opt/lool/systemplate /opt/libreoffice >/dev/null 2>&1
RUN touch /var/log/loolwsd.log
# Fix permissions
RUN chown lool:lool /var/log/loolwsd.log
RUN chown -R lool:lool /opt/
RUN chown -R lool:lool /etc/loolwsd

EXPOSE 9980

# switch to lool user (use numeric user id to be compatible with Kubernetes Pod Security Policies)
USER 101

CMD bash /run-lool.sh
