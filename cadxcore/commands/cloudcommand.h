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

#pragma once
#include <api/globals.h>
#include <api/icommand.h>
#include <api/dicom/idicom.h>
#include <vector>

namespace GNC
{
namespace GCS
{
class CloudCommandParams : public ::GNC::GCS::IComandoParams
{

public:

        typedef enum ActionType {
                CC_GET,
                CC_FIND
        } ActionType;

protected:

        CloudCommandParams(ActionType action);
        ~CloudCommandParams();

public:
        const ActionType m_action;


public:
        std::string m_error;
};

class CloudGetCommandParams : public CloudCommandParams
{
public:

        CloudGetCommandParams(const std::string& url, bool base64);
        ~CloudGetCommandParams();

        std::string m_url;
        std::string m_baseDir;
};

class CloudFindCommandParams : public CloudCommandParams
{
public:

        typedef std::map<std::string, std::string> ResultTuple;
        typedef std::list<ResultTuple> Results;

        CloudFindCommandParams(const std::string& bucketId, const std::string& userNameQuery, const std::string& descriptionQuery, const std::string& modalitiesQuery, const std::string& fromDateQuery, const std::string& toDateQuery);
        ~CloudFindCommandParams();

        const std::string m_bucketId;
        const std::string m_userNameQuery;
        const std::string m_descriptionQuery;
        const std::string m_modalitiesQuery;
        const std::string m_fromDateQuery;
        const std::string m_toDateQuery;

        Results m_pResults;

};


class CloudCommand : public GNC::GCS::IComando
{
public:
        CloudCommand(::GNC::GCS::CloudCommandParams* pParams);

protected:
        virtual void Execute();
        virtual void Update();

private:
        void doGet(GNC::GCS::CloudGetCommandParams* pParams);
        void doFind(GNC::GCS::CloudFindCommandParams* pParams);

public:

        ::GNC::GCS::CloudCommandParams* m_pCloudParams;
};
}
}
