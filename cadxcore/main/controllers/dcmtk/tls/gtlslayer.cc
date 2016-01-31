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
#include "tls.h"
#include "gtlslayer.h"
#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

BEGIN_EXTERN_C
#ifdef HAVE_WINDOWS_H
	#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <winbase.h>
#endif
#include <openssl/rand.h>
#include <openssl/err.h>
END_EXTERN_C

#include "dcmtk/dcmtls/tlstrans.h"
#include "dcmtk/dcmnet/dicom.h"

OFBool GTLSTransportLayer::setTempDHParametersFromString(const std::string& /*params*/)
{
	#if defined(USE_PATCHED_LIBS)
	if (!transportLayerContext) {
		return OFFalse; //TCS_illegalCall;
	}
	if (params.size()) {
		std::cerr << "No implementado" << std::endl;
	}
	#endif
	return OFFalse;
}


GTLSTransportLayer::~GTLSTransportLayer()
{

}

DcmTransportLayerStatus GTLSTransportLayer::setPrivateKeyFromString(const std::string& /*keyStr*/)
{
	#if defined(USE_PATCHED_LIBS)
	if (!transportLayerContext) {
		return TCS_illegalCall;
	}

	int ret = 0;

	EVP_PKEY* pkey = NULL;

	BIO* bio = BIO_new_mem_buf((void*)keyStr.c_str(), -1);
	PEM_read_bio_PrivateKey(bio, &pkey, NULL, NULL);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free(bio);

	if (pkey == NULL) {
		std::cerr << "Error al leer el clave privada" << std::endl;
		return TCS_tlsError;
	}

	ret = SSL_CTX_use_PrivateKey(transportLayerContext, pkey);
	EVP_PKEY_free(pkey);

	if (ret <= 0) {
		return TCS_tlsError;
	}
	#endif
	return TCS_ok;
}

DcmTransportLayerStatus GTLSTransportLayer::setCertificateFromString(const std::string& /*certStr*/)
{
	#if defined(USE_PATCHED_LIBS)
	if (!transportLayerContext) {
		return TCS_illegalCall;
	}

	int ret = 0;

	X509* cert = NULL;

	BIO* bio = BIO_new_mem_buf((void*)certStr.c_str(), -1);
	PEM_read_bio_X509(bio, &cert, 0, NULL);

	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free(bio);

	if (cert == NULL) {
		std::cerr << "Error al leer el certificado" << std::endl;
		return TCS_tlsError;
	}

	ret = SSL_CTX_use_certificate(transportLayerContext, cert);
	if (ret <= 0 ) {
		return TCS_tlsError;
	}
	#endif
	return TCS_ok;
}

#ifdef _WIN32
//agrega los certificados de windows
void X509_STORE_load_windows_systemstore(X509_STORE *store)
{
	HCERTSTORE hStore;
	PCCERT_CONTEXT pContext = NULL;
	X509 *x509;

	hStore = CertOpenSystemStoreA(0, "ROOT");
	if(!hStore)
		return;

	while ( (pContext = CertEnumCertificatesInStore(hStore, pContext)) != NULL )
	{
		x509 = NULL;
		x509 = d2i_X509(NULL, (const unsigned char**)(&pContext->pbCertEncoded), pContext->cbCertEncoded);
		if (x509)
		{
			X509_STORE_add_cert(store, x509);
			X509_free(x509);
		}
	}

	CertFreeCertificateContext(pContext);
	CertCloseStore(hStore, 0);
}
#endif

void LoadSystemTrustedCertificates(X509_STORE *store)
{
	X509_STORE_set_default_paths(store);
#ifdef _WIN32
	X509_STORE_load_windows_systemstore(store);
#endif
}

DcmTransportLayerStatus GTLSTransportLayer::addSystemTrustedCertificates()
{
	#if defined(USE_PATCHED_LIBS)
	if (!transportLayerContext) {
		return TCS_illegalCall;
	}

	//int ret = 0;

	X509_STORE* almacen = transportLayerContext->cert_store;
	if(almacen != NULL) {
		LoadSystemTrustedCertificates(almacen);
	}
	else {
		return TCS_tlsError;
	}
	#endif

	return TCS_ok;
}


