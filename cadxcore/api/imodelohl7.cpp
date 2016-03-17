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

#include "imodelohl7.h"
#include <cassert>

std::string GIL::HL7::DefaultEscapeER7(const std::string& str)
{
        std::stringstream ss;
        for (std::string::size_type i = 0; i < str.size(); i++) {
                if (str[i] == '\\') {
                        ss << "\\E\\";
                } else if (str[i] == '|') {
                        ss << "\\F\\";
                } else if (str[i] == '~') {
                        ss << "\\R\\";
                } else if (str[i] == '^') {
                        ss << "\\S\\";
                } else if (str[i] == '&') {
                        ss << "\\T\\";
                } else {
                        ss << str[i];
                }
        }
        return ss.str();

}

GIL::HL7::Field& GIL::HL7::Segment::ParseField(size_t fieldNum, const std::string& record, bool unscape)
{
        Field& field = ( (*this) <<= Field(fieldNum, "", m_CS, m_ES, m_SCS) );
        std::string::size_type l_loc = 0;
        std::string::size_type c_loc = std::string::npos;

        std::string::size_type start = std::string::npos;
        std::string::size_type size  = std::string::npos;
        bool cont = true;

        size_t componentNum = 1;

        while (cont) {
                c_loc = record.find(m_CS, l_loc);
                if (c_loc == std::string::npos) {
                        start = l_loc;
                        size  = c_loc;
                        l_loc = std::string::npos;
                        cont  = false;
                } else {
                        start = l_loc;
                        size  = c_loc - l_loc;
                        l_loc = c_loc + 1;
                }

                const std::string& value = ( unscape?UnEscapeER7(record.substr(start, size)) : record.substr(start, size) );
                if (value.size() == 0) {
                        field << Component(componentNum, "");
                } else if (value == "\"\"") {
                        field << Component(componentNum, "", "");
                } else {
                        field << Component(componentNum, "", value);
                }
                //std::cout << "{" << value << "}";
                componentNum++;
        }
        return field;
}

const std::string GIL::HL7::Field::EscapeER7(const std::string& str) const
{
        std::stringstream ss;
        for (std::string::size_type i = 0; i < str.size(); i++) {
                if (str[i] == m_ES[0]) {
                        ss << "\\E\\";
                } else if (str[i] == m_CS[0]) {
                        ss << "\\S\\";
                } else if (str[i] == m_SCS[0]) {
                        ss << "\\T\\";
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}

const std::string GIL::HL7::Field::UnEscapeER7(const std::string& str) const
{

        if (str.size() < 3) {
                return str;
        }

        std::stringstream ss;
        const std::string::size_type size = str.size();
        const std::string::size_type sizelimit = str.size() - 2;

        bool skipNext = false;
        bool printNext = false;

        for (std::string::size_type i = 0; i < size; i++) {
                if ( str[i] == '\\') {
                        if (skipNext) {
                                skipNext = false;
                                if (printNext) {
                                        ss << str[i];
                                        printNext = false;
                                }
                        } else {
                                skipNext = true;
                                if (i < sizelimit) {
                                        switch(str[i+1]) {
                                        case 'E':
                                                ss << m_ES;
                                                i++;
                                                break;
                                        case 'S':
                                                ss << m_CS;
                                                i++;
                                                break;
                                        case 'T':
                                                ss << m_SCS;
                                                i++;
                                                break;
                                        default:
                                                ss << str[i];
                                                printNext = true;
                                                break;
                                        }
                                } else {
                                        assert(!printNext);
                                }
                        }
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}

const std::string GIL::HL7::Segment::EscapeER7(const std::string& str) const
{
        std::stringstream ss;
        for (std::string::size_type i = 0; i < str.size(); i++) {
                if (str[i] == m_ES[0]) {
                        ss << "\\E\\";
                } else if (str[i] == m_FS[0]) {
                        ss << "\\F\\";
                } else if (str[i] == m_FRS[0]) {
                        ss << "\\R\\";
                } else if (str[i] == m_CS[0]) {
                        ss << "\\S\\";
                } else if (str[i] == m_SCS[0]) {
                        ss << "\\T\\";
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}

const std::string GIL::HL7::Segment::UnEscapeER7(const std::string& str) const
{

        if (str.size() < 3) {
                return str;
        }

        std::stringstream ss;
        const std::string::size_type size = str.size();
        const std::string::size_type sizelimit = str.size() - 2;

        bool skipNext = false;
        bool printNext = false;

        for (std::string::size_type i = 0; i < size; i++) {
                if ( str[i] == '\\') {
                        if (skipNext) {
                                skipNext = false;
                                if (printNext) {
                                        ss << str[i];
                                        printNext = false;
                                }
                        } else {
                                skipNext = true;
                                if (i < sizelimit) {
                                        switch(str[i+1]) {
                                        case 'E':
                                                ss << m_ES;
                                                i++;
                                                break;
                                        case 'F':
                                                ss << m_FS;
                                                i++;
                                                break;
                                        case 'R':
                                                ss << m_FRS;
                                                i++;
                                                break;
                                        case 'S':
                                                ss << m_CS;
                                                i++;
                                                break;
                                        case 'T':
                                                ss << m_SCS;
                                                i++;
                                                break;
                                        default:
                                                ss << str[i];
                                                printNext = true;
                                                break;
                                        }
                                } else {
                                        assert(!printNext);
                                }
                        }
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}

const std::string GIL::HL7::Message::EscapeER7(const std::string& str) const
{
        std::stringstream ss;
        for (std::string::size_type i = 0; i < str.size(); i++) {
                if (str[i] == m_ES[0]) {
                        ss << "\\E\\";
                } else if (str[i] == m_FS[0]) {
                        ss << "\\F\\";
                } else if (str[i] == m_FRS[0]) {
                        ss << "\\R\\";
                } else if (str[i] == m_CS[0]) {
                        ss << "\\S\\";
                } else if (str[i] == m_SCS[0]) {
                        ss << "\\T\\";
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}

const std::string GIL::HL7::Message::UnEscapeER7(const std::string& str) const
{

        if (str.size() < 3) {
                return str;
        }

        std::stringstream ss;
        const std::string::size_type size = str.size();
        const std::string::size_type sizelimit = str.size() - 2;

        bool skipNext = false;
        bool printNext = false;

        for (std::string::size_type i = 0; i < size; i++) {
                if ( str[i] == '\\') {
                        if (skipNext) {
                                skipNext = false;
                                if (printNext) {
                                        ss << str[i];
                                        printNext = false;
                                }
                        } else {
                                skipNext = true;
                                if (i < sizelimit) {
                                        switch(str[i+1]) {
                                        case 'E':
                                                ss << m_ES;
                                                i++;
                                                break;
                                        case 'F':
                                                ss << m_FS;
                                                i++;
                                                break;
                                        case 'R':
                                                ss << m_FRS;
                                                i++;
                                                break;
                                        case 'S':
                                                ss << m_CS;
                                                i++;
                                                break;
                                        case 'T':
                                                ss << m_SCS;
                                                i++;
                                                break;
                                        default:
                                                ss << str[i];
                                                printNext = true;
                                                break;
                                        }
                                } else {
                                        assert(!printNext);
                                }
                        }
                } else {
                        ss << str[i];
                }
        }
        return ss.str();
}
