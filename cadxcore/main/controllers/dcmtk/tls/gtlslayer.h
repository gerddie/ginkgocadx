/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#include "tls.h"

#include <dcmtk/dcmtls/tlslayer.h>

/** 
* Class derived from DcmTLSTransportLayer to accept inline certs/keys
**/
class GTLSTransportLayer : public DcmTLSTransportLayer
{
public:

	/** constructor.
	*  @param networkRole network role to be used by the application, influences
	*    the choice of the secure transport layer code.
	*  @param randFile path to file used to feed the random generator
	*/
	GTLSTransportLayer(int networkRole, const char *randFile) : DcmTLSTransportLayer(networkRole, randFile) {}

	/// destructor
	virtual ~GTLSTransportLayer();

	/** loads the private key used for authentication of this application from a string.
	*  @param key The private key
	*  @return TCS_ok if successful, an error code otherwise
	*/
	DcmTransportLayerStatus setPrivateKeyFromString(const std::string& key);

	/** loads the certificate (public key) used for authentication of this application from a string.
	*  @param cert The certificate
	*  @return TCS_ok if successful, an error code otherwise
	*/
	DcmTransportLayerStatus setCertificateFromString(const std::string& cert);

	/** loads a certificate from a string and adds it to the pool of trusted certificates.
	*  @param cert The certificate
	*  @return TCS_ok if successful, an error code otherwise
	*/
	DcmTransportLayerStatus addSystemTrustedCertificates();


	/** loads a set of Diffie-Hellman parameters from a string.
	*  These parameters are required for DH, DHE or DSS ciphersuites.
	*  @param params DH parameter
	*  @return OFTrue if successful, OFFalse otherwise.
	*/
	OFBool setTempDHParametersFromString(const std::string& params);

private:

};
