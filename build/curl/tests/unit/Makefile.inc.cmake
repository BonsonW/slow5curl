#***************************************************************************
#                                  _   _ ____  _
#  Project                     ___| | | |  _ \| |
#                             / __| | | | |_) | |
#                            | (__| |_| |  _ <| |___
#                             \___|\___/|_| \_\_____|
#
# Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at https://curl.se/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
# SPDX-License-Identifier: curl
#
###########################################################################

# these files are used in every single unit test program

SET(UNITFILES curlcheck.h 
 ../libtest/test.h 
 ../libtest/first.c)

# These are all unit test programs
SET(UNITPROGS unit1300          unit1302 unit1303 unit1304 unit1305 unit1307 
 unit1308 unit1309 unit1323 
 unit1330 unit1394 unit1395 unit1396 unit1397 unit1398 
 unit1399 
 unit1600 unit1601 unit1602 unit1603 unit1604 unit1605 unit1606 unit1607 
 unit1608 unit1609 unit1610 unit1611 unit1612 unit1614 
 unit1620 unit1621 
 unit1650 unit1651 unit1652 unit1653 unit1654 unit1655 
 unit1660 unit1661 
 unit2600 unit2601 unit2602 unit2603 
 unit3200)

SET(unit1300_SOURCES unit1300.c ${UNITFILES})

SET(unit1302_SOURCES unit1302.c ${UNITFILES})

SET(unit1303_SOURCES unit1303.c ${UNITFILES})

SET(unit1304_SOURCES unit1304.c ${UNITFILES})

SET(unit1305_SOURCES unit1305.c ${UNITFILES})

SET(unit1307_SOURCES unit1307.c ${UNITFILES})

SET(unit1308_SOURCES unit1308.c ${UNITFILES})

SET(unit1309_SOURCES unit1309.c ${UNITFILES})

SET(unit1323_SOURCES unit1323.c ${UNITFILES})

SET(unit1330_SOURCES unit1330.c ${UNITFILES})

SET(unit1394_SOURCES unit1394.c ${UNITFILES})
SET(unit1394_LDADD ${CURL_BINARY_DIR}/lib/libcurl.la ${LIBCURL_LIBS})
SET(unit1394_LDFLAGS ${CURL_BINARY_DIR}/src/libcurltool.la)
SET(unit1394_LIBS )

SET(unit1395_SOURCES unit1395.c ${UNITFILES})

SET(unit1396_SOURCES unit1396.c ${UNITFILES})

SET(unit1397_SOURCES unit1397.c ${UNITFILES})

SET(unit1398_SOURCES unit1398.c ${UNITFILES})

SET(unit1399_SOURCES unit1399.c ${UNITFILES})

SET(unit1600_SOURCES unit1600.c ${UNITFILES})

SET(unit1601_SOURCES unit1601.c ${UNITFILES})

SET(unit1602_SOURCES unit1602.c ${UNITFILES})

SET(unit1603_SOURCES unit1603.c ${UNITFILES})

SET(unit1604_SOURCES unit1604.c ${UNITFILES})

SET(unit1605_SOURCES unit1605.c ${UNITFILES})

SET(unit1606_SOURCES unit1606.c ${UNITFILES})

SET(unit1607_SOURCES unit1607.c ${UNITFILES})

SET(unit1608_SOURCES unit1608.c ${UNITFILES})

SET(unit1609_SOURCES unit1609.c ${UNITFILES})

SET(unit1610_SOURCES unit1610.c ${UNITFILES})

SET(unit1611_SOURCES unit1611.c ${UNITFILES})

SET(unit1612_SOURCES unit1612.c ${UNITFILES})

SET(unit1614_SOURCES unit1614.c ${UNITFILES})

SET(unit1620_SOURCES unit1620.c ${UNITFILES})

SET(unit1621_SOURCES unit1621.c ${UNITFILES})
SET(unit1621_LDADD ${CURL_BINARY_DIR}/src/libcurltool.la ${CURL_BINARY_DIR}/lib/libcurl.la ${NSS_LIBS})

SET(unit1650_SOURCES unit1650.c ${UNITFILES})

SET(unit1651_SOURCES unit1651.c ${UNITFILES})

SET(unit1652_SOURCES unit1652.c ${UNITFILES})

SET(unit1653_SOURCES unit1653.c ${UNITFILES})

SET(unit1654_SOURCES unit1654.c ${UNITFILES})

SET(unit1655_SOURCES unit1655.c ${UNITFILES})

SET(unit1660_SOURCES unit1660.c ${UNITFILES})

SET(unit1661_SOURCES unit1661.c ${UNITFILES})

SET(unit2600_SOURCES unit2600.c ${UNITFILES})

SET(unit2601_SOURCES unit2601.c ${UNITFILES})

SET(unit2602_SOURCES unit2602.c ${UNITFILES})

SET(unit2603_SOURCES unit2603.c ${UNITFILES})

SET(unit3200_SOURCES unit3200.c ${UNITFILES})
