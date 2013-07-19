// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Help.h"
#include "Config.h"
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
int Help()
{
    TEXTOUT("Usage: \"MediaInfo [-Options...] FileName1 [Filename2...]\"");
    TEXTOUT("");
    TEXTOUT("Options:");
    TEXTOUT("--Help, -h");
    TEXTOUT("                    Display this help and exit");
    TEXTOUT("--Help-Output");
    TEXTOUT("                    Display help for Output= option");
    TEXTOUT("--Help-AnOption");
    TEXTOUT("                    Display help for \"AnOption\"");
    TEXTOUT("--Version");
    TEXTOUT("                    Display MediaInfo version and exit");
    TEXTOUT("");
    TEXTOUT("--Full , -f");
    TEXTOUT("                    Full information Display (all internal tags)");
    TEXTOUT("--Output=HTML");
    TEXTOUT("                    Full information Display with HTML tags");
    TEXTOUT("--Output=XML");
    TEXTOUT("                    Full information Display with XML tags");
    TEXTOUT("--Output=...y");
    TEXTOUT("                    Template defined information Display");
    TEXTOUT("--Info-Parameters");
    TEXTOUT("                    Display list of Inform= parameters");
    TEXTOUT("");
    TEXTOUT("--Language=raw");
    TEXTOUT("                    Display non-translated unique identifiers (internal text)");
    TEXTOUT("--LogFile=...");
    TEXTOUT("                    Save the output in the specified file");
    TEXTOUT("--BOM");
    TEXTOUT("                    Byte order mark for UTF-8 output");
    TEXTOUT("");
    TEXTOUT("--Ssl_CertificateFileName=...");
    TEXTOUT("                    File name of the SSL certificate.");
    TEXTOUT("                    The default format is \"PEM\" and can be changed");
    TEXTOUT("                    with --Ssl_CertificateFormat.");
    TEXTOUT("--Ssl_CertificateFormat=...");
    TEXTOUT("                    File format of the SSL certificate.");
    TEXTOUT("                    Supported formats are \"PEM\" and \"DER\"");
    TEXTOUT("--Ssl_PrivateKeyFileName=...");
    TEXTOUT("                    File name of the SSL private key.");
    TEXTOUT("                    The default format is \"PEM\" and can be changed");
    TEXTOUT("                    with --Ssl_PrivateKeyFormat.");
    TEXTOUT("                    Note: private key with a password is not supported.");
    TEXTOUT("--Ssl_PrivateKeyFormat=...");
    TEXTOUT("                    File format of the SSL private key.");
    TEXTOUT("                    Supported formats are \"PEM\" and \"DER\"");
    TEXTOUT("--Ssl_CertificateAuthorityFileName=...");
    TEXTOUT("                    File name of the SSL certificate authorities");
    TEXTOUT("                    to verify the peer with.");
    TEXTOUT("--Ssl_CertificateAuthorityPath=...");
    TEXTOUT("                    Path of the SSL certificate authorities");
    TEXTOUT("                    to verify the peer with.");
    TEXTOUT("--Ssl_CertificateRevocationListFileName=...");
    TEXTOUT("                    File name of the SSL certificate revocation list.");
    TEXTOUT("                    The format is \"PEM\"");
    TEXTOUT("--Ssl_IgnoreSecurity=...");
    TEXTOUT("                    Does not verify the authenticity of the peer's certificate");
    TEXTOUT("                    Use it at your own risks");
    TEXTOUT("--Ssh_PublicKeyFileName=...");
    TEXTOUT("                    File name of the SSH private key.");
    TEXTOUT("                    Default is $HOME/.ssh/id_rsa.pub or $HOME/.ssh/id_dsa.pub");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"id_rsa.pub\" or \"id_dsa.pub\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("                    Note: you need to set both public and private key.");
    TEXTOUT("--Ssh_PrivateKeyFileName=...");
    TEXTOUT("                    File name of the SSH private key.");
    TEXTOUT("                    Default is $HOME/.ssh/id_rsa or $HOME/.ssh/id_dsa");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"id_rsa\" or \"id_dsa\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("                    Note: you need to set both public and private key.");
    TEXTOUT("                    Note: private key with a password is not supported.");
    TEXTOUT("--Ssh_KnownHostsFileName=...");
    TEXTOUT("                    File name of the known hosts");
    TEXTOUT("                    The format is the OpenSSH file format (libssh2)");
    TEXTOUT("                    Default is $HOME/.ssh/known_hosts");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"known_hosts\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("--Ssh_IgnoreSecurity=...");
    TEXTOUT("                    Does not verify the authenticity of the peer");
    TEXTOUT("                    (you don't need to accept the key with ssh first)");
    TEXTOUT("                    Use it at your own risks");

    return -1;
}

//---------------------------------------------------------------------------
int Help_Nothing()
{
    TEXTOUT("Usage: \"MediaInfo [-Options...] FileName1 [Filename2...]\"");
    TEXTOUT("\"MediaInfo --Help\" for displaying more information");

    return -1;
}

//---------------------------------------------------------------------------
int Help_Output()
{
    TEXTOUT("--Output=...  Specify a template (BETA)");
    TEXTOUT("Usage: \"MediaInfo --Output=[xxx;]Text FileName\"");
    TEXTOUT("");
    TEXTOUT("xxx can be: General, Video, Audio, Text, Chapter, Image, Menu");
    TEXTOUT("Text can be the template text, or a filename");
    TEXTOUT("     Filename must be in the form file://filename");
    TEXTOUT("");
    TEXTOUT("See --Info-Parameters for available parameters in the text");
    TEXTOUT("(Parameters must be surrounded by \"%\" sign)");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Output=Video;%AspectRatio% FileName\"");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Output=Video;file://Video.txt FileName\"");
    TEXTOUT("and Video.txt contains ");
    TEXTOUT("\"%DisplayAspectRatio%\"        for Video Aspect Ratio.");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Output=file://Text.txt FileName\"");
    TEXTOUT("and Text.txt contains");
    TEXTOUT("\"Video;%DisplayAspectRatio%\"  for Video Aspect Ratio.");
    TEXTOUT("\"Audio;%Format%\"              for Audio Format.");

    return -1;
}

//---------------------------------------------------------------------------
int Help_Security()
{
    TEXTOUT("Usage: \"MediaInfo [-Options...] FileName1 [Filename2...]\"");
    TEXTOUT("\"MediaInfo --Help\" for displaying more information");

    return -1;
}

