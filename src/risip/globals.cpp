#include "risipglobals.h"

#include <QString>


const char *RisipSettingsParam::QmlUri="Risip";

const QString RisipSettingsParam::FirstRun("firstRun");
const QString RisipSettingsParam::DefaultAccount("defaultAccount");
const QString RisipSettingsParam::AutoSignIn("autoSignIn");

const QString RisipSettingsParam::AccountGroup("SIP_Accounts");
const QString RisipSettingsParam::TotalAccounts("totalAccounts");
const QString RisipSettingsParam::Uri("uri");
const QString RisipSettingsParam::Username("username");
const QString RisipSettingsParam::Password("password");
const QString RisipSettingsParam::ServerAddress("serveraddress");
const QString RisipSettingsParam::NetworkType("networktype");
const QString RisipSettingsParam::Scheme("scheme");
const QString RisipSettingsParam::ProxyServer("proxyServer");
const QString RisipSettingsParam::LocalPort("localPort");
const QString RisipSettingsParam::RandomLocalPort("randomLocalPort");

QString RisipGlobals::formatToSip(const QString &contact, const QString &server)
{
    QString uri = QString("<sip:")
            +contact
            +QString("@")
            +server
            +QString(">");

    return uri;
}
