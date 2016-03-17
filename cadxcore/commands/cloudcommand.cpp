/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <memory>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cloudcommand.h"
#include "comandoincluirhistorial.h"
#include <api/cloud.h>
#include <api/ientorno.h>
#include <api/controllers/ipacscontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/icommandcontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/mensajes.h>
#include <api/internationalization/internationalization.h>

#include <json/json.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/tarstrm.h>
#include <wx/zstream.h>
#include <wx/wfstream.h>

#define CLOUD_PROTOCOL_VERSION "1.1"
////helper...
inline std::string FromBase64(const std::string& input)
{
        char* pOutput = (char*)malloc(input.size());

        BIO *bin, *b64;
        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bin = BIO_new_mem_buf((void*) input.c_str(), input.size());

        bin = BIO_push(b64, bin);

        int size = BIO_read(bin, pOutput, input.size());
        std::string outputstr(pOutput, size);

        BIO_free_all(b64);
        free(pOutput);
        return outputstr;
}

int progress_func_download_cloud(void* ptr, double TotalToDownload, double NowDownloaded, double , double )
{
        GNC::GCS::CloudCommand* pCmd = (GNC::GCS::CloudCommand*)(ptr);
        if (pCmd != NULL && TotalToDownload != 0) {
                if (pCmd->NotificarProgreso((float)NowDownloaded/TotalToDownload, _Std("Downloading files..."))) {
                        return 0;
                } else {
                        return -1;
                }
        } else {
                return 0;
        }
}
//

namespace GNC
{

struct InMemmoryDataHandler {
        char *data;
        size_t bufferlen;
        size_t writepos;
        int initialized;
};

size_t static inMemmoryResponseHandler( void *ptr, size_t size, size_t nmemb, void *userdata)
{
        size_t nbytes = size*nmemb;
        struct InMemmoryDataHandler* dataHandler = (struct InMemmoryDataHandler*) userdata;

        if (!(dataHandler)->initialized) {
                (dataHandler)->data = (char *)malloc(nbytes);
                (dataHandler)->bufferlen = nbytes;
                (dataHandler)->writepos = 0;
                (dataHandler)->initialized = true;
        }

        if ((dataHandler)->bufferlen < ((dataHandler)->writepos + nbytes)) {
                (dataHandler)->bufferlen = (dataHandler)->bufferlen + nbytes;
                (dataHandler)->data = (char*) realloc((dataHandler)->data, (size_t) ((dataHandler)->writepos + nbytes));
        }

        assert((dataHandler)->data != NULL);
        memcpy( (dataHandler)->data + (dataHandler)->writepos, ptr, nbytes);
        (dataHandler)->writepos += nbytes;
        return nbytes;
}

struct FileDataHandler {
        wxFFileOutputStream* fos;
        size_t filelen;
};

size_t static fileResponseHandler( void *ptr, size_t size, size_t nmemb, void *userdata)
{
        size_t nbytes = size * nmemb;
        struct FileDataHandler* dataHandler = (struct FileDataHandler *) userdata;
        dataHandler->fos->Write(ptr, nbytes);
        dataHandler->filelen += nbytes;
        return nbytes;
}
}

// CloudCommandParams

GNC::GCS::CloudCommandParams::CloudCommandParams(GNC::GCS::CloudCommandParams::ActionType action) : m_action(action)
{
}

GNC::GCS::CloudCommandParams::~CloudCommandParams()
{
}

// CloudGetCommandParams

GNC::GCS::CloudGetCommandParams::CloudGetCommandParams(const std::string& url, bool base64) : CloudCommandParams(CloudCommandParams::CC_GET), m_url (url)
{
        if (base64) {
                std::string urlWithoutPrefix = url;
                wxString urlWx = wxString::FromUTF8(url.c_str());
                if (urlWx.StartsWith(wxT("ginkgocadx://cloud:"))) {
                        urlWithoutPrefix = urlWx.SubString(19,urlWx.size()-1).ToUTF8();
                }
                m_url = FromBase64(urlWithoutPrefix);
        } else {
                m_url = url;
        }
}

GNC::GCS::CloudGetCommandParams::~CloudGetCommandParams()
{
}

// CloudFindCommandParams

GNC::GCS::CloudFindCommandParams::CloudFindCommandParams(const std::string& bucketId,const std::string& userNameQuery, const std::string& descriptionQuery, const std::string& modalitiesQuery, const std::string& fromDateQuery, const std::string& toDateQuery) : CloudCommandParams(CloudCommandParams::CC_FIND), m_bucketId(bucketId), m_userNameQuery(userNameQuery), m_descriptionQuery(descriptionQuery), m_modalitiesQuery(modalitiesQuery), m_fromDateQuery(fromDateQuery), m_toDateQuery(toDateQuery)
{
}

GNC::GCS::CloudFindCommandParams::~CloudFindCommandParams()
{
}

// CloudCommand

GNC::GCS::CloudCommand::CloudCommand(::GNC::GCS::CloudCommandParams* pParams): GNC::GCS::IComando(pParams, _Std("CloudCommand"))
{
        m_pCloudParams = pParams;
}

void GNC::GCS::CloudCommand::Execute()
{
        try {

                if (m_pCloudParams->m_action == CloudCommandParams::CC_FIND) {
                        doFind(static_cast<CloudFindCommandParams*>(m_pParams));
                } else if (m_pCloudParams->m_action == CloudCommandParams::CC_GET) {
                        doGet(static_cast<CloudGetCommandParams*>(m_pParams));
                } else {
                        m_pCloudParams->m_error = _Std("Unknown operation");
                }
        } catch (std::exception& ex) {
                std::ostringstream ostr;
                ostr << _Std("Error in cloud operation. See log to get more details...") << std::endl;
                ostr << ex.what();
                m_pCloudParams->m_error = ostr.str();
        }
}

void GNC::GCS::CloudCommand::doGet(GNC::GCS::CloudGetCommandParams* pParams)
{

        // Step 1: Setup handler
        FileDataHandler responseDataHandler;
        memset(&responseDataHandler, 0, sizeof(FileDataHandler));

        std::ostringstream ostr;

        ostr << GNC::GCS::IEntorno::Instance()->CreateGinkgoTempFile() << time(NULL) << ".tgz";
        std::string tgzFile = ostr.str();

        wxString tmpFile(FROMPATH(tgzFile));

        responseDataHandler.fos = new wxFFileOutputStream(tmpFile);

        // Step 2: Download

        CURL *curl;
        CURLcode res;
        double speed_upload, total_time;

        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, pParams->m_url.c_str());

                std::ostringstream userAgent;
                userAgent << "Mozilla/5.0 (compatible; GinkgoCADx " << GNC::GCS::IEntorno::Instance()->GetGinkgoVersionString()  << " )";
                curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.str().c_str());

                //proxy settings
                GNC::GCS::ProxySettings settings = GNC::GCS::ConfigurationController::Instance()->GetProxySettings();
                if (settings.useProxy) {
                        curl_easy_setopt(curl, CURLOPT_PROXY, settings.hostName.c_str());
                        curl_easy_setopt(curl, CURLOPT_PROXYPORT, settings.port);
                        if (settings.requiresAuth) {
                                curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, settings.userName.c_str());
                                curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, settings.password.c_str());
                        }
                        curl_easy_setopt(curl, CURLOPT_NOPROXY, settings.exceptions.c_str());
                }

                curl_easy_setopt(curl, CURLOPT_HTTPGET, TRUE);

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileResponseHandler);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &responseDataHandler);
                //progress
                curl_easy_setopt(curl, CURLOPT_PROGRESSDATA , this);
                curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func_download_cloud);
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

                res = curl_easy_perform(curl);

                // Check for errors
                if(res != CURLE_OK) {
                        std::ostringstream os;
                        os << _Std("Could to retrieve study.") << " " << curl_easy_strerror(res);
                        m_pCloudParams->m_error = os.str();
                        LOG_ERROR("CloudGet", m_pCloudParams->m_error);
                } else {
                        long http_code = 0;
                        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
                        if (http_code != 200) {
                                std::ostringstream os;
                                if (http_code == 410) {
                                        //version is not supported
                                        os << _Std("This version of Ginkgo CADx is not compatible with current version of Ginkgo Cloud.");
                                } else {
                                        os << _Std("Error retrieving study.") << " Http error code: " << http_code;
                                }
                                m_pCloudParams->m_error = os.str();
                                LOG_ERROR("CloudGet", m_pCloudParams->m_error);
                        } else {
                                // now extract transfer info
                                curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
                                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
                                std::ostringstream ostr;
                                ostr << _Std("Study successfully retrieved.") << " " << _Std("Speed") << " " << responseDataHandler.filelen << " " <<  _Std("bytes/sec during") << " " << total_time << " " << _Std("seconds");
                                LOG_INFO("CloudGet", ostr.str());
                                if (responseDataHandler.filelen == 0) {
                                        std::ostringstream os;
                                        os << _Std("Empty Response");
                                        m_pCloudParams->m_error = os.str();
                                        LOG_ERROR("CloudGet", m_pCloudParams->m_error);
                                }
                        }
                }
                // always cleanup
                curl_easy_cleanup(curl);
        }
        responseDataHandler.fos->Close();
        delete responseDataHandler.fos;

        //untar...
        wxFileName fileIn ( tmpFile );
        NotificarProgreso(0.95f, _Std("Extracting files..."));
        {
                wxFFileInputStream fsIn(fileIn.GetFullPath());
                wxZlibInputStream gzIn(fsIn, wxZLIB_GZIP);
                if (gzIn.IsOk()) {
                        wxTarInputStream tarIn(gzIn);
                        if (tarIn.IsOk()) {
                                pParams->m_baseDir = GNC::GCS::IEntorno::Instance()->CrearDirectorioTemporal();
                                wxString baseTargetDir(FROMPATH(pParams->m_baseDir));

                                for (wxTarEntry* entry = tarIn.GetNextEntry(); entry != NULL; entry = tarIn.GetNextEntry()) {
                                        if (entry->IsDir()) {
                                                wxMkdir(baseTargetDir + wxFileName::GetPathSeparator() + entry->GetName());
                                        } else {
                                                wxFFileOutputStream fileOut(baseTargetDir + wxFileName::GetPathSeparator() +  entry->GetName());
                                                tarIn.Read(fileOut);
                                                fileOut.Close();
                                                tarIn.CloseEntry();
                                        }
                                        delete entry;
                                }
                        } else {
                                pParams->m_error = _Std("Invalid file has been downloaded");
                        }
                } else {
                        pParams->m_error = _Std("Invalid file has been downloaded");
                }
        }

        wxRemoveFile(fileIn.GetFullPath());
        wxRmdir(fileIn.GetPath());

}

void GNC::GCS::CloudCommand::doFind(GNC::GCS::CloudFindCommandParams* pParams)
{

        // Step 1: Setup handler
        InMemmoryDataHandler responseDataHandler;
        memset(&responseDataHandler, 0, sizeof(InMemmoryDataHandler));

        // Step 2: Invoke

        CURL *curl;
        CURLcode res;
        double speed_upload, total_time;

        curl = curl_easy_init();
        if(curl) {
                std::ostringstream baseUrl;
                baseUrl << GINKGO_CLOUD_URI << "/find/";
                baseUrl << pParams->m_bucketId;
                baseUrl << "/";
                curl_easy_setopt(curl, CURLOPT_URL, baseUrl.str().c_str());

                std::ostringstream userAgent;
                userAgent << "Mozilla/5.0 (compatible; GinkgoCADx " << GNC::GCS::IEntorno::Instance()->GetGinkgoVersionString()  << " )";
                curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.str().c_str());

                //proxy settings
                GNC::GCS::ProxySettings settings = GNC::GCS::ConfigurationController::Instance()->GetProxySettings();
                if (settings.useProxy) {
                        curl_easy_setopt(curl, CURLOPT_PROXY, settings.hostName.c_str());
                        curl_easy_setopt(curl, CURLOPT_PROXYPORT, settings.port);
                        if (settings.requiresAuth) {
                                curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, settings.userName.c_str());
                                curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, settings.password.c_str());
                        }
                        curl_easy_setopt(curl, CURLOPT_NOPROXY, settings.exceptions.c_str());
                }

                struct curl_httppost* post = NULL;
                struct curl_httppost* last = NULL;

                curl_formadd(&post, &last, CURLFORM_COPYNAME, "userName", CURLFORM_COPYCONTENTS, pParams->m_userNameQuery.c_str(), CURLFORM_END);

                curl_formadd(&post, &last, CURLFORM_COPYNAME, "description", CURLFORM_COPYCONTENTS, pParams->m_descriptionQuery.c_str(), CURLFORM_END);
                curl_formadd(&post, &last, CURLFORM_COPYNAME, "modalities", CURLFORM_COPYCONTENTS, pParams->m_modalitiesQuery.c_str(), CURLFORM_END);
                curl_formadd(&post, &last, CURLFORM_COPYNAME, "fromDate", CURLFORM_COPYCONTENTS, pParams->m_fromDateQuery.c_str(), CURLFORM_END);
                curl_formadd(&post, &last, CURLFORM_COPYNAME, "toDate", CURLFORM_COPYCONTENTS, pParams->m_toDateQuery.c_str(), CURLFORM_END);
                curl_formadd(&post, &last, CURLFORM_COPYNAME, "versionId", CURLFORM_COPYCONTENTS, CLOUD_PROTOCOL_VERSION, CURLFORM_END);

                curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GNC::inMemmoryResponseHandler);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &responseDataHandler);

                res = curl_easy_perform(curl);

                // Check for errors
                // Check for errors
                if(res != CURLE_OK) {
                        std::ostringstream os;
                        os << _Std("Could to perform query.") << " " << curl_easy_strerror(res);
                        m_pCloudParams->m_error = os.str();
                        LOG_ERROR("CloudFind", m_pCloudParams->m_error);
                } else {
                        long http_code = 0;
                        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
                        if (http_code != 200) {
                                std::ostringstream os;
                                if (http_code == 410) {
                                        //version is not supported
                                        os << _Std("This version of Ginkgo CADx is not compatible with current version of Ginkgo Cloud.");
                                } else {
                                        os << _Std("Error perfoming the query.") << " Http error code: " << http_code;
                                }
                                m_pCloudParams->m_error = os.str();
                                LOG_ERROR("CloudFind", m_pCloudParams->m_error);
                        } else {
                                // now extract transfer info
                                curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
                                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
                                std::ostringstream ostr;
                                ostr << _Std("Query successfully performed.") << " " << _Std("Speed") << " " << speed_upload << " " <<  _Std("bytes/sec during") << " " << total_time << " " << _Std("seconds");
                                LOG_INFO("CloudFind", ostr.str());
                        }
                }
                // always cleanup

                if (responseDataHandler.bufferlen > 0 && m_pCloudParams->m_error.empty()) {

                        if (GNC::GCS::IControladorLog::Instance()->IsEnabledFor(GNC::GCS::IControladorLog::TraceLog)) {
                                std::string jsonData = std::string(responseDataHandler.data, responseDataHandler.bufferlen);
                                LOG_TRACE("CloudFind", _Std("JSon Response") << ": " << jsonData);
                        }
                        Json::Value root;
                        Json::Reader reader;
                        bool parsingSuccessful = reader.parse( responseDataHandler.data, responseDataHandler.data + responseDataHandler.bufferlen, root );
                        if (parsingSuccessful) {

                                for (Json::ValueIterator it1 = root.begin(); it1 != root.end(); ++it1) {
                                        CloudFindCommandParams::ResultTuple rtuple;
                                        Json::Value& tuple = (*it1);

                                        Json::Value::Members m = tuple.getMemberNames();
                                        for (Json::Value::Members::iterator it2 = m.begin(); it2 != m.end(); ++it2) {
                                                const std::string& name = (*it2);
                                                Json::Value& value = tuple[name];
                                                if (value.isString()) {
                                                        rtuple[name] = tuple.get(name, "").asCString();
                                                } else if (value.isBool()) {
                                                        if (value.asBool()) {
                                                                rtuple[name] = "true";
                                                        } else {
                                                                rtuple[name] = "false";
                                                        }
                                                } else if (value.isInt()) {
                                                        std::ostringstream os;
                                                        os << value.asInt();
                                                        rtuple[name] = os.str();
                                                } else if (value.isDouble()) {
                                                        std::ostringstream os;
                                                        os << value.asInt();
                                                        rtuple[name] = os.str();
                                                } else if (value.isArray()) {
                                                        if (name == "modalities") {
                                                                bool start = true;
                                                                Json::Value& modalities = tuple[name];
                                                                std::ostringstream os;
                                                                for (Json::ValueIterator it3 = modalities.begin(); it3 != modalities.end(); ++it3) {
                                                                        Json::Value& modality = (*it3);
                                                                        if (modality.isString()) {
                                                                                if (start) {
                                                                                        start = false;
                                                                                } else {
                                                                                        os << "/";
                                                                                }
                                                                                os << modality.asCString();
                                                                        }
                                                                }
                                                                rtuple[name] = os.str();
                                                        }
                                                } else {
                                                        LOG_TRACE("CloudFind", _Std("Unknown JSon attribute:") << ": " << name);
                                                }

                                        }
                                        pParams->m_pResults.push_back(rtuple);
                                }

                        } else {
                                std::ostringstream os;
                                os << _Std("Error parsing JSon: ");
                                os << reader.getFormattedErrorMessages();
                                m_pCloudParams->m_error = os.str();
                                LOG_ERROR("CloudFind", m_pCloudParams->m_error);
                        }


                } else {
                        LOG_ERROR("CloudFind", _Std("Null Response"));
                }
                if (responseDataHandler.data != NULL) {
                        free(responseDataHandler.data);
                }
                curl_formfree(post);
                curl_easy_cleanup(curl);
        }

}

void GNC::GCS::CloudCommand::Update()
{
        if (m_pCloudParams->m_error != "") {
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, m_pCloudParams->m_error, GNC::GCS::Events::EventoMensajes::PopUpMessage, false, GNC::GCS::Events::EventoMensajes::Error));
        } else {
                if (m_pCloudParams->m_action == CloudCommandParams::CC_GET) {

                        CloudGetCommandParams* pParams = static_cast<CloudGetCommandParams*>(m_pCloudParams);

                        if (pParams != NULL) {
                                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pIncluirParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(pParams->m_baseDir, true, GNC::GCS::IHistoryController::TAA_MOVE);
                                pIncluirParams->m_abrirDespuesDeCargar = true;
                                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pIncluirParams);
                                GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Storing in the history..."), pCmd, this->GetOwner());
                        }
                }
        }
}
