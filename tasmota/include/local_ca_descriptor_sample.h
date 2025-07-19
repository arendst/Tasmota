/*
  local-ca-sample.h - sample file for embedding a local CA certificate

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
To generate a version of this file containing data for your root certificate,
run the following command from a Linux or Cygwin bash shell, assuming that a
copy of brssl (or brssl.exe) is in the directory where the EasyRSA shell script
is located.

./brssl ta pki/ca.crt | sed -e '1,/br_x509/ d' -e '/};/,+999 d' >local_ca_descriptor.h

Then copy local_ca_descriptor.h into directory include.

Add this line to user_config_override.h:

#define INCLUDE_LOCAL_CERT

Be sure to generate both files: local_ca_data.h, and local_ca_descriptor.h
*/

//
// this is what the result will look like
//
	{
		{ (unsigned char *)TA0_DN, sizeof TA0_DN },
		BR_X509_TA_CA,
		{
			BR_KEYTYPE_RSA,
			{ .rsa = {
				(unsigned char *)TA0_RSA_N, sizeof TA0_RSA_N,
				(unsigned char *)TA0_RSA_E, sizeof TA0_RSA_E,
			} }
		}
	}
