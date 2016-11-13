#include "risipglobals.h"

#include <QString>
#include <QList>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QThreadPool>

#include <QDebug>

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

Country::Country(const QString &id, const QString &name,
                 const QString &code, const QString &prefix)
    :countryId(id)
    ,name(name)
    ,code(code)
    ,prefix(prefix)
{

}

Country::Country(const Country &country)
    :countryId(country.countryId)
    ,name(country.name)
    ,code(country.code)
    ,prefix(country.prefix)
{

}

Country &Country::operator=(const Country &country)
{
    countryId = country.countryId;
    name = country.name;
    code = country.code;
    prefix = country.prefix;

    return *this;
}

bool Country::operator!=(const Country &country)
{
    return (code != country.code);
}

bool Country::operator==(const Country &country)
{
    return (code == country.code);
}

class RisipConfigisLoader: public QRunnable
{
public:
    RisipConfigisLoader()
        :QRunnable()
    {}

    ~RisipConfigisLoader()
    {}

private:
    void run()
    {
        //loading country list
        QFile file(QString(":/configs/countries"));
        if(file.open(QFile::ReadOnly)) {
            QString countryLine;
            QTextStream streamReader(&file);
            QHash<QString, Country> countries;

            Country country;
            while (streamReader.readLineInto(&countryLine)) {

                country.countryId = countryLine.left(countryLine.indexOf(";")).trimmed();

                countryLine.remove(0, countryLine.indexOf(";") +1);
                country.name = countryLine.left(countryLine.indexOf(";")).trimmed();

                countryLine.remove(0, countryLine.indexOf(";") +1);
                country.prefix = countryLine.left(countryLine.indexOf(";")).trimmed();

                countryLine.remove(0, countryLine.indexOf(";") +1);
                country.code = countryLine.left(countryLine.indexOf(";")).trimmed();

                countries.insert(country.code, country);
            }

            RisipGlobals::setCountryList(countries);
        } else {
            qDebug()<<"Configs files cannot be found nor be read!!";
        }
    }
};

QHash<QString, Country> RisipGlobals::m_allCountries;
bool RisipGlobals::m_countriesIntialized = false;
RisipGlobals::RisipGlobals()
{

}

QString RisipGlobals::formatToSip(const QString &contact, const QString &server)
{
    QString uri = QString("<sip:")
            +contact
            +QString("@")
            +server
            +QString(">");

    return uri;
}

QList<Country> RisipGlobals::countries()
{
    return m_allCountries.values();
}

const Country &RisipGlobals::country(const QString &code)
{
    if(m_allCountries.contains(code))
        return m_allCountries[code];

    return Country();
}

void RisipGlobals::initializeCountries()
{
    if(!m_countriesIntialized) {
        RisipConfigisLoader *configsLoader = new RisipConfigisLoader;
        configsLoader->setAutoDelete(true);
        QThreadPool::globalInstance()->start(configsLoader);
    }
}

bool RisipGlobals::countriesInitialized()
{
    return m_countriesIntialized;
}

void RisipGlobals::setCountryList(QHash<QString, Country> countryList)
{
    m_allCountries = countryList;
    if(!m_allCountries.isEmpty())
        m_countriesIntialized = true;
    else
        m_countriesIntialized = false;
}
